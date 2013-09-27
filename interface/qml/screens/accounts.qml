import QtQuick 2.0
import "../components"
import "../../js/account_active.js" as ControllerJS

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

    ListModel
    {
        id: accsModel
        ListElement { idx: "1"; section: "Accounts"; image: "othos_icon.png"; name: "Othos VoIP Account"; description: "Your personal account";}
        ListElement { idx: "2";section: "Accounts"; image: "othos_icon.png"; name: "OCS"; description: "Your corporate account";}
        ListElement { idx: "3";section: "Accounts"; image: "ic_account_other.png"; name: "Custom SIP Account"; description: "Connect to any SIP provider";}
    }

    Component
    {
        id: sectionHeading
        Rectangle
        {
            height: 38
            width: accountListView.width
            color: "#303030"

            Rectangle
            {
                width: parent.width
                height: 1
                color: "#6E6E6E";
                anchors.top: parent.top
            }

            Rectangle
            {
                width: parent.width
                height: 1
                color: "#6E6E6E";
                anchors.bottom: parent.bottom
            }

            Text
            {
                text: section
                font.bold: true
                color: "white"
                font.pixelSize: 19
                anchors
                {
                    left: parent.left
                    leftMargin: 20
                    verticalCenter: parent.verticalCenter
                }
            }
        }
    }


    ListView
    {
        id: accountListView
        width: parent.width
        height: 217
        clip: true
        interactive: false
        section.property: "section"
        section.criteria: ViewSection.FullString
        section.delegate: sectionHeading
        model: accsModel

        delegate:
            Item
            {
                id:listViewRowWrapper
                width: accountListView.width;
                height: 60

                Rectangle
                {
                    gradient: (mouseArea.containsMouse) ? normalItemOdd : normalItem
                    anchors.fill: parent

                    MouseArea
                    {
                        id: mouseArea
                        cursorShape: "PointingHandCursor"
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked:
                        {
                            screenAccounts.loadAccount(idx);
                            m_activeScreen = idx;
                        }
                    }

                    Row
                    {
                        spacing: 10
                        width: parent.width - 10
                        height: parent.height
                        x: 10

                        Rectangle
                        {
                            visible: image!==""
                            height: parent.height

                            Image
                            {

                                anchors.centerIn: parent
                                source: "../../assets/" + image
                            }
                            width: 55
                            color: "transparent"

                        }

                        Column
                        {
                            height: parent.height
                            width: 180
                            y:9
                            spacing: 1
                            Text
                            {
                                text: name
                                color: "white"
                                font.pixelSize: 18
                            }
                            Text
                            {
                                text: description
                                color: "white"
                                font.pixelSize: 12
                            }
                        }

                        Item
                        {
                            width: 60
                            height: parent.height
                        }

                        Image
                        {
                            id: activate
                            y: 13
                            width: 34
                            height: 34
                            z:3333333
                            source: activeAccount === idx ? "../../assets/ic_radio_on.png" : "../../assets/ic_radio_off.png"

                            MouseArea
                            {
                                id: mouseAreaActivate
                                anchors.fill: parent
                                cursorShape: "ArrowCursor"
                                hoverEnabled: true
                                onClicked:
                                {
                                    ControllerJS.activateAccount(idx);
                                }
                            }

                            Rectangle
                            {
                                id: tooltip
                                anchors.right: parent.left;
                                visible: mouseAreaActivate.containsMouse
                                clip: false
                                color: "#CCC"
                                width: tip.width
                                height: tip.height
                                opacity: 0.8
                                Text
                                {
                                    id: tip
                                    text: "Activate this account"
                                    color: "#000"
                                    anchors.margins: 6
                                    anchors.centerIn: parent
                                }
                            }
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
        GradientStop { position: 1; color: "#222421" }
        GradientStop { position: 0; color: "#222421" }
    }

    Gradient
    {
        id: normalItemOdd
        GradientStop {position: 0; color: "#212121"}
        GradientStop {position: 1; color: "#121212"}
    }
}
