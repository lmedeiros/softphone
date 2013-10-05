#ifndef SCREENSETTINGS_H
#define SCREENSETTINGS_H
#include <pjsua-lib/pjsua.h>
#include "videoviewer.h"
#include <QObject>
class Controller;

class ScreenSettings : public QObject
{
    Q_OBJECT
public:
    explicit ScreenSettings(Controller *ctrl, QObject *parent = 0);

    Controller *controller;

    bool preview_on;

    Q_INVOKABLE void showPreviewWindow(const QString &id);
    Q_INVOKABLE QString getActiveAccount();
    Q_INVOKABLE void setSettings();
    Q_INVOKABLE bool setActiveAccount(const QString &account);
    Q_INVOKABLE void getSettings();
    Q_INVOKABLE bool setSipPort(const QString &port);
    Q_INVOKABLE bool setAudioDevices(const QString &inputDevice, const QString &outputDevice);
    Q_INVOKABLE bool setVideoDevice(const QString &videoDeviceID, const QString &videoDeviceName);
    Q_INVOKABLE bool stopPreview();

    void getInputDeviceList();
    void getOutputDeviceList();
    void getVideoDeviceList();
    bool validateSetting(const QString& settingTag);

    pjmedia_vid_dev_index m_vid_dev_index;
    pjsua_vid_win_info m_vid_win_info;
    pjsua_vid_win_id m_vid_win_id;
    VidWin* m_video_viewer;

signals:
    
public slots:
    
};

#endif // SCREENSETTINGS_H
