import QtQuick 2.0
import "../components"

MainScreen
{
    id: mainScreen

    Component.onCompleted:
    {
        screenContacts.getFavourites();
    }

    Text
    {
        text: "No favourite numbers"
        color: "white"
        visible: listViewNum.count <= 0
        font.family: mainScreen.m_fontMain.name
        anchors.centerIn: parent
        font.pixelSize: 30
    }

    Item
    {
        height: app_height - contentStart
        width: parent.width

        anchors
        {
            top: parent.top
            topMargin: contentStart
        }

        GridView
        {
            id: listViewNum
            anchors.fill: parent
            cellHeight: 118
            cellWidth: 118
            clip: true
            interactive: true
            model: favouritesModel
            delegate: cellNumbers

            ScrollBar
            {
                target: parent
            }
        }
    }

    Component
    {
        id: cellNumbers

        Image
        {
            MouseArea
            {
                id: mouseArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: "PointingHandCursor"
                onClicked:
                {
                    screenPhone.setDialText(number);
                    Controller.setQml("screens/phone.qml", "phone");
                }
            }

            width: 116
            height: 116
            source: pic_path!=="" ? pic_path : "../../assets/buddy.png"
            fillMode: Image.PreserveAspectCrop

            Rectangle
            {
                id: tooltip
                anchors.centerIn: parent
                z: 1000
                visible: number !== "" && mouseArea.containsMouse
                clip: false
                color: "#EEE"
                border.width: 1
                border.color: "#111"
                width: tip.width
                height: tip.height
                opacity: 0.8
                Text
                {
                    id: tip
                    text: " " + number + " "
                    color: "#000"
                    font.bold: true
                    anchors.margins: 6
                    anchors.centerIn: parent
                }
            }

            Rectangle
            {
                width: parent.width
                anchors.top: parent.top
                color: "#333"
                opacity: 0.8
                height: 18
            }

            Rectangle
            {
                width: parent.width
                anchors.bottom: parent.bottom
                color: "#000"
                opacity: 0.8
                height: 18
            }

            Text
            {
                text: name + " " + surname
                color: "white"
                font.pixelSize: 12
                font.family: mainScreen.m_fontMain.name
                font.bold: true
                width: parent.width - 10
                anchors.top: parent.top
                height: 18
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text
            {
                text: description
                color: "white"
                font.pixelSize: 12
                font.family: mainScreen.m_fontMain.name
                font.bold: true
                width: parent.width - 10
                anchors.bottom: parent.bottom
                elide: Text.ElideRight
                height: 18
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }

    Gradient
    {
        id: selectedItem
        GradientStop {position: 0; color: "#FF9900"}
        GradientStop {position: 1; color: "#FF5800"}
    }

    Gradient
    {
        id: normalItem
        GradientStop { position: 1; color: "#303030" }
        GradientStop { position: 0; color: "#303030" }
    }

    Gradient
    {
        id: overItem
        GradientStop {position: 0; color: "#444"}
        GradientStop {position: 1; color: "#222"}
    }
}
