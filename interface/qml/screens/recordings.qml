import QtQuick 2.0
import QtMultimedia 5.0
import "../components"

MainScreen
{
    id: mainScreen
    property string activeFileName

    Component.onCompleted:
    {
        screenPhone.setFileModel();
    }

    Text
    {
        text: "No Recordings"
        color: "white"
        visible: logListView.count <= 0
        anchors.centerIn: parent
        font.pixelSize: 30
    }

    Row
    {
        height: app_height - contentStart
        anchors
        {
            top: parent.top
            topMargin: contentStart
            left: parent.left
            leftMargin: 1
        }

        Item
        {
            width: 320
            height: parent.height

            ListView
            {
                id: logListView
                anchors.fill: parent
                clip: true
                focus: true
                model: screenPhone.rec_list
                interactive: true
                delegate: listViewRow;
                visible: logListView.count > 0

                ScrollBar
                {
                    target: parent
                }
            }
        }

        Item
        {
            width: app_width - 320
            height: parent.height
            visible: activeFileName !== ""

           Text
            {
                text: "Play"
                color: "#FFF"
                font.pixelSize: 24
                width: 150
                height: 80
                anchors.centerIn: parent
                MouseArea
                {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: "PointingHandCursor"
                    onClicked:
                    {
                        recordFile.source = "file:///" + recFilePath + "/" + activeFileName
                        recordFile.play();
                    }
                }
            }
        }
    }

    Audio
    {
        id: recordFile


        onPlaying:
        {
            playbackRect.visible = true
        }

        onStopped:
        {
            playbackRect.visible = false
            recordFile.destroy();
        }
    }

    Rectangle
    {
        id: playbackRect
        color: "#111"
        opacity: 0.8
        visible: false
        anchors.fill: mainScreen

        Text
        {
            text: "Playback file: " + activeFileName + "\n\n Click to stop";
            color: "#FFF"
            font.pixelSize: 14
            anchors.centerIn: parent
        }

        MouseArea
        {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: "PointingHandCursor"
            onClicked:
            {
                 recordFile.stop();
                 recordFile.destroy();
            }
        }
    }

    Component
    {
        id: listViewRow

        Item
        {
            id:listViewRowWrapper
            width: logListView.width;
            height: 70

            Rectangle
            {
                gradient: (modelData===activeFileName) ? selectedItem : (mouseArea.containsMouse) ? overItem : normalItem
                anchors.fill: parent

                Rectangle
                {
                    width: parent.width
                    height: 1
                    color: "#3c3c3c"
                    anchors.bottom: parent.bottom
                }

                MouseArea
                {
                    id: mouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: "PointingHandCursor"
                    onClicked:
                    {
                        activeFileName = modelData;
                    }
                }

                Text
                {
                    text: modelData;
                    color: "White"
                    font.pixelSize: 16
                    font.bold: true
                    width: parent.width - 20
                    elide: Text.ElideRight
                    anchors.centerIn: parent
                }
            }
        }
    }

    Gradient
    {
        id: selectedItem
        GradientStop {position: 0; color: "#FF7800"}
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
