import QtQuick 2.1
import QtQuick.Dialogs 1.0
import "../components"

import "../../js/contact_add.js" as ControllerJS

Rectangle
{
    color: "#222421"

    property int new_contact_id

    Component.onCompleted:
    {
        new_contact_id = screenContacts.generateContactId();
        txtName.m_input.forceActiveFocus();
        screenContacts.getNewNumbers(new_contact_id);

        if(newNumberContactDesc!=="" || newNumberContactNumber!=="" || newNumberContactName!=="")
        {
            phoneDescription.m_text = newNumberContactDesc;
            phoneNumber.m_text = newNumberContactNumber;
            txtName.m_text = newNumberContactName;
            txtSurname.m_text = '';
        }

        screenContacts.setNewNumber('','','');
    }

    FileDialog
    {
        id: fileDialog
        visible: false
        selectFolder: false
        nameFilters: [ "Image files (*.jpg *.png *.gif)" ]
        title: "Please choose a picture"
        onAccepted:
        {
            txtPicPath.source = fileUrl;
        }
    }

    Button
    {
        id: btnConfirmAdd
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
            ControllerJS.addContact();
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
                source: "../../assets/buddy.png"

                MouseArea
                {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: "PointingHandCursor"
                    onClicked:
                    {
                        fileDialog.visible = true
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
                    m_input.color: "#BBB"
                    m_text: "Name"
                    m_input.font.pixelSize: 20
                    Keys.onTabPressed:
                    {
                        txtSurname.m_input.forceActiveFocus();
                    }

                    m_input.onFocusChanged:
                    {
                        if(m_input.focus && m_text === "Name")
                        {
                            m_text = "";
                            m_input.color = "#111"
                        }
                    }
                }

                Textbox
                {
                    id: txtSurname
                    width: 562
                    height: 45
                    radius: 10
                    color: "white"
                    m_input.color: "#BBB"
                    m_input.font.pixelSize: 20
                    m_input.anchors.topMargin: 8
                    m_input.anchors.leftMargin: 10
                    m_text: "Family Name"
                    Keys.onTabPressed:
                    {
                        txtName.m_input.forceActiveFocus();
                    }

                    m_input.onFocusChanged:
                    {
                        if(m_input.focus && m_text === "Family Name")
                        {
                            m_text = "";
                            m_input.color = "#111"
                        }
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
        anchors.topMargin: 20
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
        anchors.top: numberForm.bottom
        anchors.margins: 10
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
            height: 20
            Text
            {
                text: description
                width: 220
                color: "white"
                font.pixelSize: 18
            }
            Text
            {
                text: number
                width: 430
                color: "white"
                font.pixelSize: 18
            }

            Button
            {
                id: btnClearPhone
                width: 20
                height: 20
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
                        screenContacts.getNewNumbers(new_contact_id);
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
