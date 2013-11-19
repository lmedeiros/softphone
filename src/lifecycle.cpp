#include "lifecycle.h"

Lifecycle::Lifecycle()
{
    qmlviewer = new QtQuick2ApplicationViewer();
    (qmlviewer) ? setAppWindow() : quitApp();
}

void Lifecycle::setAppWindow()
{
    qmlviewer->setMainQmlFile(QUrl(QStringLiteral("qrc:/qml/sphone/softphone.qml"));
}

void Lifecycle::quitApp()
{
    QGuiApplication::exit(0);
}
