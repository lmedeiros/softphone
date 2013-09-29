#ifndef SCREENSETTINGS_H
#define SCREENSETTINGS_H
#include <pjsua-lib/pjsua.h>
#include <QObject>
class Controller;

class ScreenSettings : public QObject
{
    Q_OBJECT
public:
    explicit ScreenSettings(Controller *ctrl, QObject *parent = 0);

    Controller *controller;

    Q_INVOKABLE void showPreviewWindow(qint64 id);
    Q_INVOKABLE QString getActiveAccount();
    Q_INVOKABLE void setSettings();
    Q_INVOKABLE bool setActiveAccount(const QString &account);
    Q_INVOKABLE void getSettings();
    Q_INVOKABLE bool setSipPort(const QString &port);
    Q_INVOKABLE bool setAudioDevices(const QString &inputDevice, const QString &outputDevice, const QString &ringingDevice);
    void getInputDeviceList();
    void getOutputDeviceList();
    void getVideoDeviceList();
    bool validateSetting(const QString& settingTag);

    pjmedia_vid_dev_index m_video_idx;
signals:
    
public slots:
    
};

#endif // SCREENSETTINGS_H
