import QtQuick 2.1
import QtMultimedia 5.0

import "parts/"
import "components/"

Rectangle
{
    id: mainScreen
    width: app_width
    height: app_height
    color: "#212421"

    property alias m_fontMain: fontMain
    property alias m_menu: menu
    property int contentStart: menu.height + line.height

    property alias messageBox: mbox

    Column
    {
        Menu
        {
            id: menu
        }

        Rectangle
        {
            id: line
            width: app_width
            height: 3
            color: "#ff5800"
        }
    }

    FontLoader
    {
        id: fontMain
        source: "../assets/OpenSans-Regular.ttf"
    }

    Rectangle
    {
        color: "black"
        opacity: 0.7
        width: app_width
        height: app_height
        z: 1000000

        visible: mbox.state !== "hide"

        MouseArea
        {
            cursorShape: "PointingHandCursor"
            anchors.fill: parent
            onClicked:
            {
                mbox.state = "hide";
            }
        }
    }

    Messagebox
    {
        id: mbox
        z: 1000001
        anchors.centerIn: parent
        state: "hide"
        boxHeight: 150
        boxWidth: 380
    }
}
