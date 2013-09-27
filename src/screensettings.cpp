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
    if(!validateSetting(controller->settings->m_active_account)) controller->settings->m_active_account = "0";
    if(!validateSetting(controller->settings->m_sip_port)) controller->settings->m_sip_port = "5061";

    controller->qmlviewer->rootContext()->setContextProperty("audioInputDeviceId", controller->settings->m_inputDevice.m_id);
    controller->qmlviewer->rootContext()->setContextProperty("audioInputDeviceName", controller->settings->m_inputDevice.m_name);
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
    controller->settings->m_ringingDevice.m_name =ringingDevice;
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

void ScreenSettings::getSettings()
{
    getInputDeviceList();
    getOutputDeviceList();
    setSettings();
}

QString ScreenSettings::getActiveAccount()
{
    return controller->settings->m_active_account;
}
