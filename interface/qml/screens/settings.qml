import QtQuick 2.0
import "../components"

MainScreen
{
    property string m_activeScreen
    property alias m_listView: settingsListView
    property alias loader: settingsLoading

    Component.onCompleted:
    {
        screenPhone.loadCodecList();
        m_activeScreen = "Configurations";
    }

    onM_activeScreenChanged:
    {
        if(m_activeScreen === "Audio")
        {
            settingsLoading.source = "config.qml";
        }

        if(m_activeScreen === "Video")
        {
            settingsLoading.source = "video.qml";
        }


        if(m_activeScreen === "Info")
        {
            settingsLoading.source = "info.qml";
        }

        if(m_activeScreen === "Accounts")
        {
            settingsLoading.source = "accounts.qml";
        }

        if(m_activeScreen === "1")
        {
            settingsLoading.source = "accounts_othos.qml";
        }

        if(m_activeScreen === "2")
        {
            settingsLoading.source = "accounts_pbx.qml";
        }

        if(m_activeScreen === "3")
        {
            settingsLoading.source = "accounts_custom.qml";
        }
    }

    ListModel
    {
        id: settingsModel
        ListElement { image: "ic_settings_accounts.png"; name: "Accounts"; }
        ListElement { image: "ic_settings_audio.png"; name: "Audio"; }
        ListElement { image: "ic_settings_video.png"; name: "Video"; }
        ListElement { image: "ic_settings_info.png"; name: "Info"; }
    }

    Row
    {
        height: app_height - contentStart
        anchors
        {
            top: parent.top
            topMargin: contentStart
            left: parent.left
        }

        Item
        {
            width: app_width * 0.45
            height: parent.height

            ListView
            {
                id: settingsListView
                anchors.fill: parent
                clip: true
                focus: true
                model: settingsModel
                interactive: false
                delegate: settingsListRow;
                height: parent.height
            }
        }

        Item
        {
            width: app_width * 0.55
            height: parent.height

            Loader
            {
                id: settingsLoading
                anchors.fill: parent
            }
        }
    }

    Component
    {
        id: settingsListRow

        Item
        {
            id:listViewRowWrapper
            width: settingsListView.width;
            height: 72

            Rectangle
            {
                gradient: (m_activeScreen === name || ((name=="Accounts") && (m_activeScreen==='1'||m_activeScreen==='2'||m_activeScreen==='3'||m_activeScreen==='4'))) ? selectedItem : (mouseArea.containsMouse) ? normalItemOdd : normalItem
                anchors.fill: parent

                MouseArea
                {
                    id: mouseArea
                    cursorShape: "PointingHandCursor"
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked:
                    {
                        m_activeScreen = name;
                        settingsListView.currentIndex = index;
                    }
                }

                Row
                {
                    spacing: 15
                    width: parent.width - 10
                    x: 10
                    height: parent.height

                    Rectangle
                    {
                        height: parent.height
                        Image
                        {
                            anchors.centerIn: parent
                            source: "../../assets/" + image
                        }
                        width: 50
                        color: "transparent"

                    }

                    Text
                    {
                        height: parent.height
                        verticalAlignment: Text.AlignVCenter
                        text: name
                        width: 180
                        color: "white"
                        font.pixelSize: 22
                    }
                }
            }

            Rectangle
            {
                height: 1
                width: parent.width
                color: "#888"
                anchors.bottom: parent.bottom
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
        id: normalItemOdd
        GradientStop {position: 0; color: "#444"}
        GradientStop {position: 1; color: "#222"}
    }
}
