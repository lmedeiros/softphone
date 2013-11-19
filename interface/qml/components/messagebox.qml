import QtQuick 2.0

Item
{
    id: container
    z: 100001
    width: 380;
    height: 150;

    anchors.verticalCenter: parent.verticalCenter
    anchors.horizontalCenter: parent.horizontalCenter

    property alias m_waiter: waiter
    property alias text: textMessage.text
    property alias boxHeight: messageBox.height
    property alias boxWidth: messageBox.width
    property string m_color: "black"

    signal clicked

    states: [

        State {
            name: "hide"
            PropertyChanges {
                target: container;
                opacity: 0
            }
        },

        State {
            name: "show"
            PropertyChanges {
                target: container;
                opacity: 1;
                m_waiter.running: true
            }
        }
    ]

    transitions: [
        Transition { to: "hide";
            SequentialAnimation {
                NumberAnimation { target: messageBox; property: "opacity"; from: 1; to: 0; duration: 100; }
            }
        },
        Transition { to: "show";
            SequentialAnimation {
                NumberAnimation { target: messageBox; property: "opacity"; from: 0; to: 1; duration: 100; }
            }
        }
    ]

    Timer
    {
        id: waiter; interval: 4000; running: false; repeat: false;

        onTriggered:
        {
            container.state = "hide";
        }

    }

    Rectangle
    {
        id: messageBox
        anchors.margins: 30
        anchors.rightMargin: 10
        border {width: 5; color: "#BBB"}
        color: "#232323"
        radius: 7
        opacity: 0.9

        Text
        {
            text: "MESSAGE"
            font { pointSize: 18; weight: Font.Bold }
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            color: "white"
            anchors.topMargin: 10
        }

        Text
        {
            id: textMessage
            color: m_color
            anchors {leftMargin: 10; left: parent.left; right: parent.right; top: parent.top; topMargin: 50; rightMargin: 10}
            text: ""
            font { pointSize: 16; weight: Font.Bold }
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
        }
    }   
}
