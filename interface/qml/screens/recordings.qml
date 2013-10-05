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
            id: listRecs
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

        Text
        {
            anchors
            {
                left: listRecs.right
                top: parent.top
                margins: 20
            }

            width: 180
            height: 200
            elide: Text.ElideRight
            color: "#DDD"
            font.pixelSize: 12
            text: 'Folder: ' + recFilePath + '\n' +
                  'Filename: ' + activeFileName + '\n' +
                  'Date: ' + setDate(activeFileName) + '\n' +
                  'Destination: ' + activeFileName.split('_')[1].toString().split('-')[0].toString();
        }
    }

    function setDate(fileName)
    {
        var date = fileName.split('-');
        var date1 = date[1]
        var date2 = date1.split('.');

        date = date2[0];

        var year = date.substr(0,4);
        var month = date.substr(4,2);
        var day = date.substr(6,2);
        var hour = date.substr(8,2);
        var minute = date.substr(10,2);
        var sec = date.substr(12,2)

        return day + '/' + month + '/' + year + " " + hour + ":" + minute + ":" + sec
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
                        var url = "file:///" + recFilePath + "/" + modelData
                        messageBox.text = 'Opening audio on external application';
                        messageBox.m_color = 'white';
                        messageBox.state = 'show';
                        console.debug('Playing file: ' + url);
                        screenPhone.playbackAudio(url);
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
