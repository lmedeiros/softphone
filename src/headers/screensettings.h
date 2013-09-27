#ifndef SCREENSETTINGS_H
#define SCREENSETTINGS_H

#include <QObject>
class Controller;

class ScreenSettings : public QObject
{
    Q_OBJECT
public:
    explicit ScreenSettings(Controller *ctrl, QObject *parent = 0);

    Controller *controller;

    Q_INVOKABLE QString getActiveAccount();
    Q_INVOKABLE void setSettings();
    Q_INVOKABLE bool setActiveAccount(const QString &account);
    Q_INVOKABLE void getSettings();
    Q_INVOKABLE bool setSipPort(const QString &port);
    Q_INVOKABLE bool setAudioDevices(const QString &inputDevice, const QString &outputDevice, const QString &ringingDevice);
    void getInputDeviceList();
    void getOutputDeviceList();
    bool validateSetting(const QString& settingTag);
signals:
    
public slots:
    
};

#endif // SCREENSETTINGS_H
