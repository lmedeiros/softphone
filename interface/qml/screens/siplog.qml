import QtQuick 2.0
import FileIO 1.0
import "../components"


Rectangle
{
    anchors.fill: parent

    property string activeAcc: ""

    color: "transparent"

    FontLoader
    {
        id: fontMain
        source: "../../assets/OpenSans-Regular.ttf"
    }

    FileIO
    {
        id: logFile
        source: "o2csip.log"
        onError: console.log(msg)
    }

    Text
    {
        id: appTitle
        text: "SIP Logging"
        font.bold: true
        font.family: fontMain.name
        font.pixelSize: 24
        color: "#FFF"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 20
    }

    Column
    {
            spacing: 8
            width: parent.width
            height: parent.height
            anchors.top: appTitle.bottom
            anchors.topMargin: 40
            anchors.left: parent.left
            anchors.leftMargin: 20

         Flickable
         {
            id: flickArea
            width: 330
            height: 230
            contentWidth: txtLog.width;
            contentHeight: txtLog.height;
            flickableDirection: Flickable.VerticalFlick
            clip: true

            Text
            {
                id: txtLog
                width: 330
                wrapMode: Text.WordWrap
                font.pixelSize: 9
                text: logFile.read();
                color: "#FFF"
                elide: Text.ElideRight
            }
        }

        Button
        {
            id: btnCheckVersion
            width: parent.width
            height: 25
            Text
            {
                anchors.centerIn: parent
                text: "Reload Log"
                font.pixelSize: 14
                font.bold: true
                color: !parent.m_clicker.containsMouse ? "#FF6600" : "#FF9900"
            }

            onButtonClicked:
            {
                console.debug("Reloading Log...");
                reloadFile();
            }
        }
    }

    function reloadFile()
    {
        txtLog.text = logFile.read();
    }
}
