#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QThread>
#include <QApplication>
#include <QtGui/QGuiApplication>
#include <qtquick2applicationviewer.h>
#include <QQmlContext>
#include "database.h"
#include "screenaccounts.h"
#include "screencontacts.h"
#include "screenlog.h"
#include "screenphone.h"
#include "screensettings.h"
#include "telephony.h"
#include "settingsmanager.h"

#define QML_PATH "qrc:/interface/qml/"
#define APP_HEIGHT 420
#define APP_WIDTH 700

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(int argc, char *argv[], QObject *parent = 0);
    QApplication *app;
    QQuickView *qmlviewer;

    Database *db;
    ScreenSettings *screenSettings;
    ScreenLog *screenLog;
    ScreenContacts *screenContacts;
    ScreenAccounts *screenAccounts;
    ScreenPhone *screenPhone;
    Telephony *telApi;
    SettingsManager *settings;


    Q_INVOKABLE void setQml(const QString &qmlfile, const QString &screenName);
    Q_INVOKABLE void quitApp();
    Q_INVOKABLE void startUp();

private:
    void setMainWindow();
    void setQmlProperties();
    void setScreenControllers();

signals:
    
public slots:
    
};

#endif // CONTROLLER_H
