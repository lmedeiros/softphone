import QtQuick 2.0
import "../components"

Rectangle
{
    anchors.fill: parent

    property string version_url: "http://www.dcit.com.br/othos/version.html"
    property string activeAcc: ""

    color: "transparent"

    FontLoader
    {
        id: fontMain
        source: "../../assets/OpenSans-Regular.ttf"
    }

    Text
    {
        id: appTitle
        text: "O2C 3.0"
        font.bold: true
        font.family: fontMain.name
        font.pixelSize: 32
        color: "#FFF"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 20
    }

    Column
    {
        spacing: 8
        width: parent.width
        anchors.top: appTitle.bottom
        anchors.topMargin: 40
        anchors.horizontalCenter: parent.horizontalCenter
        Text
        {
            width: parent.width
            font.pixelSize: 14
            horizontalAlignment: Text.AlignHCenter
            text: "Qt License 5.1 (www.qt-project.org)"
            color: "#CCC"
        }

        Text
        {
            width: parent.width
            font.pixelSize: 14
            horizontalAlignment: Text.AlignHCenter
            text: "PJSIP License 2.1 (www.pjsip.org)"
            color: "#CCC"
        }

        Text
        {
            width: parent.width
            font.pixelSize: 14
            horizontalAlignment: Text.AlignHCenter
            text: "Devel by dcit.com.br"
            color: "#4444DD"
        }

        Text
        {
            width: parent.width
            font.pixelSize: 14
            horizontalAlignment: Text.AlignHCenter
            text: "Design by othos.co.za"
            color: "#FF6600"
        }

        Text
        {
            width: parent.width
            font.pixelSize: 14
            horizontalAlignment: Text.AlignHCenter
            text: "Sponsor by othos.co.za"
            color: "#FF9900"
        }

        Item
        {
            width: 20
            height: 40
        }

        Button
        {
            id: btnCheckVersion
            width: parent.width
            height: 25
            Text
            {
                anchors.centerIn: parent
                text: "Check for New Version"
                font.pixelSize: 18
                font.bold: true
                color: !parent.m_clicker.containsMouse ? "#FF6600" : "#FF9900"
            }

            onButtonClicked:
            {
                console.debug("Checking version");
                checkVersion();
            }
        }
    }

    function showRequestInfo(text) {
        console.log(text)
    }

    function checkVersion()
    {
        var doc = new XMLHttpRequest();
                         doc.onreadystatechange = function() {
                             if (doc.readyState == XMLHttpRequest.DONE) {
                                 var a = doc.responseXML
                                 showRequestInfo("Reponse text: " + a);
                             }
                         }

                         doc.open("GET", version_url);
                         doc.send();

        messageBox.state = "show";
        messageBox.m_color = "white";
        messageBox.text = "This version is the latest available";
    }
}
