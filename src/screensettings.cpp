#include "headers/screensettings.h"
#include <QDebug>
#include "headers/controller.h"

ScreenSettings::ScreenSettings(Controller *ctrl, QObject *parent) :
    QObject(parent)
{
    controller = ctrl;
}

void ScreenSettings::setSettings()
{
    if(!validateSetting(controller->settings->m_inputDevice.m_id)) controller->settings->m_inputDevice.m_id = "Unknown ID";
    if(!validateSetting(controller->settings->m_inputDevice.m_name)) controller->settings->m_inputDevice.m_name = "Unknown Device";

    if(!validateSetting(controller->settings->m_outputDevice.m_id)) controller->settings->m_outputDevice.m_id = "Unknown ID";
    if(!validateSetting(controller->settings->m_outputDevice.m_name)) controller->settings->m_outputDevice.m_name = "Unknown Device";

    if(!validateSetting(controller->settings->m_videoDevice.m_id)) controller->settings->m_videoDevice.m_id = "Unknown ID";
    if(!validateSetting(controller->settings->m_videoDevice.m_name)) controller->settings->m_videoDevice.m_name = "Unknown Device";

    if(!validateSetting(controller->settings->m_active_account)) controller->settings->m_active_account = "0";
    if(!validateSetting(controller->settings->m_sip_port)) controller->settings->m_sip_port = "5061";

    controller->qmlviewer->rootContext()->setContextProperty("audioInputDeviceId", controller->settings->m_inputDevice.m_id);
    controller->qmlviewer->rootContext()->setContextProperty("audioInputDeviceName", controller->settings->m_inputDevice.m_name);

    controller->qmlviewer->rootContext()->setContextProperty("videoDeviceId", controller->settings->m_videoDevice.m_id);
    controller->qmlviewer->rootContext()->setContextProperty("videoDeviceName", controller->settings->m_videoDevice.m_name);

    controller->qmlviewer->rootContext()->setContextProperty("audioOutputDeviceId", controller->settings->m_outputDevice.m_id);
    controller->qmlviewer->rootContext()->setContextProperty("audioOutputDeviceName", controller->settings->m_outputDevice.m_name);

    controller->qmlviewer->rootContext()->setContextProperty("activeAccount", controller->settings->m_active_account);
    controller->qmlviewer->rootContext()->setContextProperty("sipPort", controller->settings->m_sip_port);
}

bool ScreenSettings::validateSetting(const QString& settingTag)
{
    if(settingTag.isEmpty() || settingTag.isNull())
    {
        return false;
    }

    if(settingTag.isSimpleText())
    {
        return true;
    }

    return false;
}

bool ScreenSettings::setActiveAccount(const QString& account)
{
    if(account.toInt()<=0)
        return false;
    controller->settings->m_active_account = account;
    setSettings();
    controller->telApi->unregisterAccount();
    bool accountIsOk = controller->telApi->registerAccount(account);

    controller->settings->writeSettings();

    return accountIsOk;
}

bool ScreenSettings::setAudioDevices(const QString &inputDevice, const QString &outputDevice)
{
    controller->settings->m_inputDevice.m_name = inputDevice;
    controller->settings->m_outputDevice.m_name = outputDevice;

    controller->telApi->input_dev =-1;
    controller->telApi->output_dev =-2;

    unsigned count = 64;
    pjmedia_aud_dev_info audio_device_info[64];
    pjsua_enum_aud_devs(audio_device_info, &count);
    for (unsigned i=0;i<count;i++)
    {
        if (audio_device_info[i].input_count && !inputDevice.compare(audio_device_info[i].name))
        {
            controller->telApi->input_dev = i;
            qDebug() << "Setting input devices" << audio_device_info[i].name;
        }

        if (audio_device_info[i].output_count && !outputDevice.compare(audio_device_info[i].name))
        {
            controller->telApi->output_dev = i;
            qDebug() << "Setting output device" << audio_device_info[i].name;
        }
    }

    int in,out;
    if (pjsua_get_snd_dev(&in,&out)!=PJ_SUCCESS || controller->telApi->input_dev != in || controller->telApi->output_dev != out )
    {
        qDebug() << "Setting audio devices";
        pjsua_set_snd_dev(controller->telApi->input_dev, controller->telApi->output_dev);
    }

    setSettings();
    return controller->settings->writeSettings();
}

bool ScreenSettings::setVideoDevice(const QString &videoDeviceID, const QString &videoDeviceName)
{
    controller->settings->m_videoDevice.m_id = videoDeviceID;
    controller->settings->m_videoDevice.m_name = videoDeviceName;

    setSettings();
    return controller->settings->writeSettings();
}

bool ScreenSettings::setSipPort(const QString &port)
{
    controller->settings->m_sip_port = port;
    return controller->settings->writeSettings();
}

void ScreenSettings::getInputDeviceList()
{
    QHash<QString, QVariant> deviceList = controller->settings->deviceList(true);

    QStringList model = deviceList.keys();

    controller->qmlviewer->rootContext()->setContextProperty("audioDeviceInputModel",  QVariant::fromValue(model));
}

void ScreenSettings::getOutputDeviceList()
{
    QHash<QString, QVariant> deviceList = controller->settings->deviceList(false);

    QStringList model = deviceList.keys();

    controller->qmlviewer->rootContext()->setContextProperty("audioDeviceOutputModel", QVariant::fromValue(model));
}

void ScreenSettings::getVideoDeviceList()
{
    QList<QVariant> deviceList = this->controller->settings->videoDeviceList();

    controller->qmlviewer->rootContext()->setContextProperty("videoDeviceModel", deviceList);
}

void ScreenSettings::getSettings()
{
    getInputDeviceList();
    getOutputDeviceList();
    getVideoDeviceList();
    setSettings();
}

QString ScreenSettings::getActiveAccount()
{
    return controller->settings->m_active_account;
}

bool ScreenSettings::stopPreview()
{
    if(this->m_vid_win_info.show == PJ_TRUE && this->m_vid_dev_index != -100 && controller->screenPhone->c1_call_id()==-1)
    {
        qDebug() << "Stopping video preview";
        pjsua_vid_win_set_show(this->m_vid_win_id, PJ_FALSE);
        pjsua_vid_preview_stop(this->m_vid_dev_index);
        this->m_vid_dev_index = -100;
        return true;
    }
    return false;
}

void ScreenSettings::showPreviewWindow(const QString& id)
{
    QStringList devtmp = id.split("::::");

    QString device_name = devtmp[0];
    int device_id = devtmp[1].toInt();

    qDebug() << "Show preview for device: " << device_name;

    if(this->stopPreview()) return;

    this->m_vid_dev_index = device_id;

    pj_status_t status;
    pjsua_vid_preview_param pre_param;

    pjsua_vid_preview_param_default(&pre_param);
    pre_param.wnd_flags = PJMEDIA_VID_DEV_WND_BORDER;

    status = pjsua_vid_preview_start(this->m_vid_dev_index, &pre_param);

    this->m_vid_win_id = pjsua_vid_preview_get_win(this->m_vid_dev_index);

    const pjmedia_rect_size size = {240, 180};
    const pjmedia_coord pos = {(controller->qmlviewer->x()+controller->qmlviewer->width()),controller->qmlviewer->y()};
    pjsua_vid_win_set_size(m_vid_win_id, &size);
    pjsua_vid_win_set_pos(m_vid_win_id, &pos);

    pjsua_vid_win_get_info(m_vid_win_id, &this->m_vid_win_info);

    if (this->m_vid_win_id != PJSUA_INVALID_ID) {
        if (this->m_vid_dev_index != device_id) {
            qDebug() << "Stopping video preview Different ID";
            pjsua_vid_preview_stop(this->m_vid_dev_index);
        } else {
            qDebug() << "Stopping video preview no ID";
            return;
        }
    }

    this->m_video_viewer = new VidWin(&this->m_vid_win_info.hwnd);

    if (status != PJ_SUCCESS) {
        qDebug() << "Fail to show video";
        return;
    }
}
