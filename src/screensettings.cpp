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
    if(!validateSetting(controller->settings->m_ringingDevice.m_id)) controller->settings->m_ringingDevice.m_id = "Unknown ID";
    if(!validateSetting(controller->settings->m_ringingDevice.m_name)) controller->settings->m_ringingDevice.m_name = "Unknown Device";
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
    controller->qmlviewer->rootContext()->setContextProperty("audioRingingDeviceId", controller->settings->m_ringingDevice.m_id);
    controller->qmlviewer->rootContext()->setContextProperty("audioRingingDeviceName", controller->settings->m_ringingDevice.m_name);
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

bool ScreenSettings::setAudioDevices(const QString &inputDevice, const QString &outputDevice, const QString &ringingDevice)
{
    controller->settings->m_inputDevice.m_name = inputDevice;
    controller->settings->m_outputDevice.m_name = outputDevice;
    controller->settings->m_ringingDevice.m_name = ringingDevice;
    controller->telApi->ring_dev =-2;
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

        if (audio_device_info[i].output_count)
        {
            if (!outputDevice.compare(audio_device_info[i].name))
            {
                controller->telApi->output_dev = i;
                qDebug() << "Setting output device" << audio_device_info[i].name;
            }

            if (!ringingDevice.compare(audio_device_info[i].name))
            {
                controller->telApi->ring_dev = i;
                qDebug() << "Setting ringing device" << audio_device_info[i].name;
            }
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
    controller->qmlviewer->rootContext()->setContextProperty("audioDeviceRingingModel", QVariant::fromValue(model));
}

void ScreenSettings::getVideoDeviceList()
{
    QHash<QString, QVariant> deviceList = this->controller->settings->videoDeviceList();

    QList<QVariant> model = deviceList.values();

    controller->qmlviewer->rootContext()->setContextProperty("videoDeviceModel", QVariant::fromValue(model));
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

void ScreenSettings::showPreviewWindow(qint64 id)
{
//    pjsua_vid_win_info vid_win_info;
//    pjsua_vid_win_id vid_win_id = pjsua_vid_preview_get_win(this->m_video_idx);

//    this->m_video_idx = (pjmedia_vid_dev_index)dev_id;

//    //this->m_video_idx = PJMEDIA_VID_DEFAULT_CAPTURE_DEV;

//    qDebug() << "1 - Showing preview video from device id: " << QString::number(this->m_video_idx);

//    pjsua_vid_preview_param vid_preview_param;
//    pjsua_vid_preview_param_default(&vid_preview_param);

//    qDebug() << "2 - Showing preview video from device id: " << QString::number(this->m_video_idx);

//    pjsua_vid_preview_start((int)this->m_video_idx, &vid_preview_param);

//    qDebug() << "3 - Showing preview video from device id: " << QString::number(this->m_video_idx);

//    vid_win_id = pjsua_vid_preview_get_win(this->m_video_idx);
//    pjsua_vid_win_get_info(vid_win_id, &vid_win_info);

//    qDebug() << "4 - Showing preview video from device id: " << QString::number(this->m_video_idx);

//    const pjmedia_coord vid_win_pos = {0, 0};

//    pjsua_vid_win_set_pos(vid_win_id,&vid_win_pos);

//    qDebug() << "5 - Showing preview video from device id: " << QString::number(this->m_video_idx);
//    pjsua_vid_win_set_show(vid_win_id, PJ_TRUE);

//    qDebug() << "6 - Showing preview video from device id: " << QString::number(this->m_video_idx);

//    const pjmedia_rect_size vid_win_size = {320, 240};
//    pjsua_vid_win_set_size(vid_win_id, &vid_win_size);

//    qDebug() << "¨7 - Showing preview video from device id: " << QString::number(this->m_video_idx);

    pjmedia_vid_dev_index widx;
    pjsua_vid_win_info wi;
    pjsua_vid_win_id wid = pjsua_vid_preview_get_win(widx);
    if (wid != PJSUA_INVALID_ID) {
        if (widx != id) {
            pjsua_vid_preview_stop(widx);
        } else {
            return;
        }
    }
    widx = id;

    pjsua_vid_preview_param pre_param;

    pre_param.show = PJ_TRUE;

    pj_status_t status;
    const pjmedia_coord pos = {20, 20};

    pjsua_vid_preview_param_default(&pre_param);
    pre_param.show = PJ_TRUE;
    pre_param.wnd_flags = PJMEDIA_VID_DEV_WND_BORDER | PJMEDIA_VID_DEV_WND_RESIZABLE;
    status = pjsua_vid_preview_start(widx, &pre_param);
    if (status != PJ_SUCCESS) {
        qDebug() << "Fail to show video";
        return;
    }
    wid = pjsua_vid_preview_get_win(widx);
    pjsua_vid_win_get_info(wid, &wi);

    pjsua_vid_win_set_pos(wid,&pos);
    pjsua_vid_win_set_show(wid, PJ_TRUE);

    const pjmedia_rect_size size = {320, 240};
    pjsua_vid_win_set_size( wid, &size);
}
