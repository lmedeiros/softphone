import QtQuick 2.1
import QtQuick.Dialogs 1.0
import "../components"

import "../../js/contact_edit.js" as ControllerJS

Rectangle
{
    color: "#222421"

    Component.onCompleted:
    {
        txtName.m_input.forceActiveFocus();
        screenContacts.getNewNumbers(selectedContact);
    }

    FileDialog
    {
        id: fileDialog
        visible: false
        selectFolder: false
        nameFilters: [ "Image files (*.jpg *.png *.gif)" ]
        title: "Please choose a file"
        onAccepted:
        {
            txtPicPath.source = fileUrl;
        }
    }

    Button
    {
        id: btnConfirmEdit
        width: 40
        height: 25
        anchors.right: parent.right
        anchors.rightMargin: 60
        y: -33
        Text
        {
            anchors.centerIn: parent
            text: "Save"
            font.pixelSize: 18
            font.bold: true
            color: !parent.m_clicker.containsMouse ? "#FF6600" : "#FF9900"
        }

        onButtonClicked:
        {
            ControllerJS.editContact();
        }
    }

    Column
    {
        id: fieldsColumn

        spacing: 15
        anchors
        {
            fill: parent
            margins: 10
        }

        Row
        {
            spacing: 10
            z: 120

            Image
            {
                id: txtPicPath
                width: 105
                height: 105
                source: m_picpath!=="" ? m_picpath : "../../assets/buddy.png"

                MouseArea
                {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: "PointingHandCursor"
                    onClicked:
                    {
                        fileDialog.visible = true
                        parent.source = fileDialog.fileUrl
                    }
                }
            }

            Column
            {
                spacing: 10
                y: 3

                Textbox
                {
                    id: txtName
                    width: 562
                    height: 45
                    radius: 10
                    color: "white"
                    m_input.anchors.topMargin: 8
                    m_input.anchors.leftMargin: 10
                    m_input.color: "#111"
                    m_text: m_name
                    m_input.font.pixelSize: 20
                    Keys.onTabPressed:
                    {
                        txtSurname.m_input.forceActiveFocus();
                    }
                }

                Textbox
                {
                    id: txtSurname
                    width: 562
                    height: 45
                    radius: 10
                    color: "white"
                    m_input.color: "#111"
                    m_input.font.pixelSize: 20
                    m_input.anchors.topMargin: 8
                    m_input.anchors.leftMargin: 10
                    m_text: m_surname
                    Keys.onTabPressed:
                    {
                        txtName.m_input.forceActiveFocus();
                    }
                }
            }
        }
    }

    Rectangle
    {
        id: phoneNumberBar
        height: 28
        width: parent.width
        anchors.top: parent.top
        anchors.topMargin: txtSurname.y + txtSurname.height + 30
        color: "#3A3C3A"

        Rectangle
        {
            width: parent.width
            height: 1
            color: "#4B4C4B";
            anchors.top: parent.top
        }

        Rectangle
        {
            width: parent.width
            height: 1
            color: "#424442";
            anchors.bottom: parent.bottom
        }

        Text
        {
            text: "Phone Numbers"
            height: parent.height
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 13
            color: "#FF6600"
            font.family: mainScreen.m_fontMain.name
            font.bold: true
            x: 10
        }

        Rectangle
        {
            width: 2
            height: parent.height
            color: parent.parent.color
            anchors.right: btnAdd.left
        }

        Rectangle
        {
            id: btnAdd
            height: parent.height
            width: 75
            anchors.right: parent.right
            color: (marea.containsMouse) ? "#434343" : "transparent"
            Text
            {
                text: "Add"
                font.pixelSize: 18
                font.bold: true
                color: "white"
                anchors.centerIn: parent
            }
            MouseArea
            {
                id: marea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: "PointingHandCursor"
                onClicked:
                {
                    ControllerJS.addNumber();
                }
            }
        }
    }

    Row
    {
        id: numberForm
        anchors.top: phoneNumberBar.bottom
        anchors.topMargin: 15
        anchors.leftMargin: 10
        anchors.left: parent.left
        spacing: 10
        height: 45

        Textbox
        {
            id: phoneDescription
            width: 200
            height: 45
            radius: 10
            color: "white"
            m_input.color: "#BBB"
            m_text: "Description"
            m_input.font.pixelSize: 20
            m_input.anchors.topMargin: 8
            m_input.anchors.leftMargin: 10

            Keys.onTabPressed:
            {
                phoneNumber.m_input.forceActiveFocus();
            }
            m_input.onFocusChanged:
            {
                if(m_input.focus && m_text === "Description")
                {
                    m_text = "";
                    m_input.color = "#111"
                }
            }
        }

        Textbox
        {
            id: phoneNumber
            width: 415
            height: 45
            radius: 10
            color: "white"
            m_input.color: "#BBB"
            m_text: "Phone Number"
            m_input.font.pixelSize: 20
            m_input.anchors.topMargin: 8
            m_input.anchors.leftMargin: 10

            Keys.onTabPressed:
            {
                phoneDescription.m_input.forceActiveFocus();
            }
            m_input.onFocusChanged:
            {
                if(m_input.focus && m_text === "Phone Number")
                {
                    m_text = "";
                    m_input.color = "#111"
                }
            }
        }

        Button
        {
            id: btnClearPhone
            width: 44
            height: 40
            y:3
            tooltip_text: "Clear phone fields"
            m_image: "../../assets/background_button_small.png"
            m_image_over: "../../assets/background_button_small_pressed.png"
            Image
            {
                source: "../../assets/ic_contact_detail_delete.png"
                anchors.centerIn: parent
            }

            onButtonClicked:
            {
                ControllerJS.clearPhone();
            }
        }
    }

    ListView
    {
        id: listViewNumbers
        anchors.top: numberForm.bottom
        anchors.margins: 10
        anchors.topMargin: 20
        anchors.left: parent.left
        spacing: 10
        height: 100
        width: parent.width - 20
        clip: true
        model: numbersNewModel
        delegate: rowNewNumbers

        ScrollBar
        {
            target: parent
        }
    }

    Component
    {
        id: rowNewNumbers
        Row
        {
            id: numberForm
            spacing: 10
            height: 45

            Textbox
            {
                id: phoneDescription
                width: 200
                height: 45
                radius: 10
                color: "white"
                m_input.color: "#222"
                m_text: description
                m_input.font.pixelSize: 20
                m_input.anchors.topMargin: 8
                m_input.anchors.leftMargin: 10

                Keys.onTabPressed:
                {
                    phoneNumber.m_input.forceActiveFocus();
                }
            }

            Textbox
            {
                id: phoneNumber
                width: 415
                height: 45
                radius: 10
                color: "white"
                m_input.color: "#222"
                m_text: number
                m_input.font.pixelSize: 20
                m_input.anchors.topMargin: 8
                m_input.anchors.leftMargin: 10

                Keys.onTabPressed:
                {
                    phoneDescription.m_input.forceActiveFocus();
                }
            }

            Button
            {
                id: btnClearPhone
                width: 25
                height: 25
                x: 45
                y: 10
                tooltip_text: "Cancel this number"
                m_image: "../../assets/background_button_small.png"
                m_image_over: "../../assets/background_button_small_pressed.png"
                Image
                {
                    width: 16
                    height: 16
                    source: "../../assets/ic_contact_detail_delete.png"
                    anchors.centerIn: parent
                }

                onButtonClicked:
                {
                    if(screenContacts.deleteNumber(number_id))
                    {
                        screenContacts.getNewNumbers(selectedContact);
                    }
                }
            }
        }
    }

    Timer
    {
        id: waiterAddContact
        running: false
        interval: 1000
        repeat: false
        onTriggered:
        {
            Controller.setQml("screens/contacts.qml", "contacts");
        }
    }
}
