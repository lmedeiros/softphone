#include "headers/controller.h"
#include <QFile>
#include <QTextStream>

void customMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("Debug: %1").arg(msg);
        break;

    case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg);
    break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg);
    break;
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(msg);
        abort();
    }

    QFile outFile("o2c3log.txt");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}

Controller::Controller(int argc, char *argv[], QObject *parent) :
    QObject(parent)
{
    qInstallMessageHandler(customMessageHandler);
    app = new QGuiApplication(argc, argv);
    qmlviewer = new QQuickView();
    setScreenControllers();
    setQmlProperties();
    startUp();
    setMainWindow();
}

void Controller::startUp()
{
    if(telApi->init()==0)
    {
        screenSettings->setSettings();

        if(!settings->m_active_account.isEmpty() && settings->m_active_account.toInt() > 0)
        {
            qDebug() << "All good, registering account: " << settings->m_active_account;
            screenSettings->setActiveAccount(settings->m_active_account);
        }
        else
        {
            screenPhone->setIs_registered(0);
        }
    }
}

void Controller::setMainWindow()
{
    qmlviewer->setResizeMode(QQuickView::SizeRootObjectToView);
    app->setApplicationDisplayName("O2C");
    app->setApplicationName("O2C");
    qmlviewer->setTitle("O2C");
    app->setApplicationVersion("3.0.0");
    qmlviewer->setMaximumWidth(APP_WIDTH);
    qmlviewer->setMinimumHeight(APP_HEIGHT);
    qmlviewer->setMinimumWidth(APP_WIDTH);
    qmlviewer->setMaximumHeight(APP_HEIGHT);
    setQml("screens/phone.qml", "phone");
    qmlviewer->setGeometry(100,100,700,420);
    qmlviewer->show();
}

void Controller::setQml(const QString &qmlfile, const QString &screenName)
{
    QQmlContext *qmlcontext = qmlviewer->rootContext();
    QString file = qmlfile;
    QString qmlfullpath = file.prepend(QML_PATH);
    const QUrl qmlurl(qmlfullpath);

    qmlcontext->setContextProperty("activeScreen", screenName);

    QMetaObject::invokeMethod(qmlviewer, "setSource", Qt::QueuedConnection, Q_ARG(QUrl, qmlurl));
}

void Controller::setQmlProperties()
{
    QQmlContext *qmlcontext = qmlviewer->rootContext();

    qmlcontext->setContextProperty("app_height", APP_HEIGHT);
    qmlcontext->setContextProperty("app_width", APP_WIDTH);
    qmlcontext->setContextProperty("Controller", this);
    qmlcontext->setContextProperty("QmlViewer", qmlviewer);
    qmlcontext->setContextProperty("telapi", telApi);
    qmlcontext->setContextProperty("screenSettings", screenSettings);
    qmlcontext->setContextProperty("screenLog", screenLog);
    qmlcontext->setContextProperty("screenAccounts", screenAccounts);
    qmlcontext->setContextProperty("screenPhone", screenPhone);
    qmlcontext->setContextProperty("screenContacts", screenContacts);
    qmlcontext->setContextProperty("settings", screenContacts);
    qmlcontext->setContextProperty("recFilePath", QGuiApplication::applicationDirPath().append("/recs"));
    qmlcontext->setContextProperty("keypadText", "");
    qmlcontext->setContextProperty("lastDial", "");
    qmlcontext->setContextProperty("activeAccountUser", "");
    qmlcontext->setContextProperty("activeAccountMailbox", "");
    qmlcontext->setContextProperty("numbersModel",  "");
    qmlcontext->setContextProperty("numbersNewModel",  "");
    qmlcontext->setContextProperty("suggestionModel",  "");
    qmlcontext->setContextProperty("activeCall1Contact",  "");
    qmlcontext->setContextProperty("activeCall2Contact",  "");
    qmlcontext->setContextProperty("activeAccountDescription",  "");
}

void Controller::setScreenControllers()
{
    db = new Database();
    screenLog = new ScreenLog(this);
    screenAccounts = new ScreenAccounts(this);
    screenContacts = new ScreenContacts(this);
    screenPhone = new ScreenPhone(this);
    screenSettings = new ScreenSettings(this);
    settings = new SettingsManager(this);
    telApi = new Telephony(this);
}

void Controller::quitApp()
{
    app->exit(0);
}
