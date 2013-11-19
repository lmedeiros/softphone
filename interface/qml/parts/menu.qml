import QtQuick 2.0

import "../components"

Item
{
    width: app_width
    height: m_button_h
    property int m_button_w: 105
    property int m_button_h: 34
    property int m_button_text_size: 13

    Rectangle
    {
        anchors.fill: parent
        color: "#000"
    }

    Gradient
    {
        id: gradientOver
        GradientStop {position: 0; color: "#FF9900"}
        GradientStop {position: 1; color: "#ff5800"}
    }

    Gradient
    {
        id: gradientActive
        GradientStop {position: 0; color: "#333"}
        GradientStop {position: 1; color: "#111"}
    }

    Gradient
    {
        id: gradientNormal
        GradientStop {position: 0; color: "#000"}
        GradientStop {position: 1; color: "#000"}
    }

    Column
    {
        Row
        {
            MenuButton
            {
                id: btnPhone
                m_data: "phone"
                m_screenqml: "screens/phone.qml"
                m_text: "Phone"
                m_image: "../../assets/tab_phone.png"
                width: 90
                gradient: (activeScreen===m_data) ? gradientOver : (m_clicker.containsMouse) ? gradientActive : (activeScreen===m_data) ? gradientOver: gradientNormal
            }

            MenuButton
            {
                id: btnContacts
                m_data: "contacts"
                m_screenqml: "screens/contacts.qml"
                m_text: "Contacts"
                m_image: "../../assets/tab_contacts.png"
                gradient: (activeScreen===m_data) ? gradientOver : (m_clicker.containsMouse) ? gradientActive : (activeScreen===m_data) ? gradientOver: gradientNormal
            }

            MenuButton
            {
                id: btnLog
                width: 95
                m_data: "log"
                m_screenqml: "screens/log.qml"
                m_text: "History"
                m_image: "../../assets/tab_history.png"
                m_text_color: screenPhone.miss_notify ? "#FF3333" : "white"
                gradient: (activeScreen===m_data) ? gradientOver : (m_clicker.containsMouse) ? gradientActive : (activeScreen===m_data) ? gradientOver: gradientNormal
            }

            MenuButton
            {
                id: btnFavourites
                m_data: "favourites"
                m_screenqml: "screens/favourites.qml"
                m_text: "Favourites"
                m_image: "../../assets/tab_favourites.png"
                gradient: (activeScreen===m_data) ? gradientOver : (m_clicker.containsMouse) ? gradientActive : (activeScreen===m_data) ? gradientOver: gradientNormal
            }

            MenuButton
            {
                id: btnRecordings
                m_data: "recordings"
                m_screenqml: "screens/recordings.qml"
                m_text: "Recordings"
                m_image: "../../assets/tab_records.png"
                gradient: (activeScreen===m_data) ? gradientOver : (m_clicker.containsMouse) ? gradientActive : (activeScreen===m_data) ? gradientOver: gradientNormal
            }
        }
    }

    Row
    {
        spacing: 2
        anchors
        {
            right: btnSettings.left
            rightMargin: 5
            top: parent.top
            topMargin: 5
        }

        Button
        {
            id: btnAddContact
            width: 25
            height: 25
            visible: activeScreen==="contacts"  && mainScreen.m_loader_screen===""
            m_image: "../../assets/add_contact.png"
            m_image_over: "../../assets/add_contact.png"
            tooltip_text: "Add new contact"
            tooltip_pos: "left"
            m_data: "menu"
            onButtonClicked:
            {
                contactsLoader.source = "../screens/contacts_add.qml"
                m_loader_screen = "add"
            }
        }

        Button
        {
            id: btnEditContact
            width: 25
            height: 25
            visible: activeScreen==="contacts" && mainScreen.m_name !==""
            m_image: "../../assets/edit.png"
            m_image_over: "../../assets/edit.png"
            tooltip_text: "Edit selected contact"
            tooltip_pos: "left"
            m_data: "menu"
            onButtonClicked:
            {
                contactsLoader.source = "../screens/contacts_edit.qml"
                m_loader_screen = "edit"
            }
        }

        Button
        {
            id: btnDelContact
            width: 25
            height: 25
            visible: activeScreen==="contacts" && mainScreen.m_name !==""
            m_image: "../../assets/delete.png"
            m_image_over: "../../assets/delete.png"
            tooltip_text: "Delete selected contact"
            tooltip_pos: "left"
            m_data: "menu"
            onButtonClicked:
            {
                if(screenContacts.delContact(mainScreen.selectedContact))
                {
                    messageBox.m_color = "white"
                    messageBox.text = "Contact deleted";
                    messageBox.state = "show";
                    waiterDelContact.running = true;
                }
                else
                {
                    messageBox.m_color = "red"
                    messageBox.text = "Error when delete contact";
                    messageBox.state = "show";
                }
            }
        }
    }

    Row
    {
        anchors
        {
            right: btnSettings.left
            rightMargin: 5
            top: parent.top
            topMargin: 5
        }
        spacing: 2
        Button
        {
            id: btnClear
            width: 25
            height: 25
            m_image: "../../assets/clear_log.png"
            m_image_over: "../../assets/clear_log.png"
            tooltip_text: "Clear the call logs"
            tooltip_pos: "left"
            visible: activeScreen==="log" && logListView.count > 0
            onButtonClicked:
            {
                if(screenLog.clearLogs(activeAccount))
                {
                    messageBox.text = "Call logs were cleared";
                    messageBox.state = "show"
                    messageBox.m_color = "white"
                    Controller.setQml("screens/log.qml", "log");
                }
                else
                {
                    messageBox.text = "Error when clear logs";
                    messageBox.state = "show"
                    messageBox.m_color = "red"
                }
            }
        }

        Button
        {
            id: btnDelLog
            width: 25
            height: 25
            visible: activeScreen==="log" && txtActiveNum.text !==""
            m_image: "../../assets/delete.png"
            m_image_over: "../../assets/delete.png"
            tooltip_text: "Delete selected call log"
            tooltip_pos: "left"
            m_data: "menu"
            onButtonClicked:
            {
                if(screenLog.delLog(m_log_id))
                {
                    messageBox.m_color = "white"
                    messageBox.text = "Log deleted";
                    messageBox.state = "show";
                    waiterDelLog.running = true;

                }
                else
                {
                    messageBox.m_color = "red"
                    messageBox.text = "Error when delete log";
                    messageBox.state = "show";
                }
            }
        }

    }

    MenuButton
    {
        id: btnSettings
        anchors
        {
            right: parent.right
            top: parent.top
            topMargin: 0
        }
        tooltip_text: "O2C Settings"
        tooltip_pos: "right"
        m_data: "settings"
        m_screenqml: "screens/settings.qml"
        m_text: ""
        m_image: "../../assets/tab_settings.png"
        gradient: (activeScreen===m_data) ? gradientOver : (m_clicker.containsMouse) ? gradientActive : (activeScreen===m_data) ? gradientOver: gradientNormal
    }


    Timer
    {
        id: waiterDelContact
        running: false
        interval: 1000
        repeat: false
        onTriggered:
        {
            Controller.setQml("screens/contacts.qml", "contacts");
        }
    }

    Timer
    {
        id: waiterDelLog
        running: false
        interval: 1000
        repeat: false
        onTriggered:
        {
            Controller.setQml("screens/log.qml", "log");
        }
    }

    Text
    {
        id: txtAccount
        y: 8
        height: 12
        visible: activeScreen==="phone"
        horizontalAlignment: Text.AlignRight
        anchors
        {

            right: parent.right
            rightMargin: 40
        }
        color: "#FF6600"

        font.bold: true
        font.pointSize: 14
        text: setAccountText()

        Rectangle
        {
            id: tooltip1
            anchors.right: txtAccount.left;
            visible: screenPhone.register_reason !== "" && ma1.containsMouse
            clip: false
            color: "#CDCDCD"
            width: tip1.width
            height: tip1.height
            opacity: 0.8
            Text
            {
                id: tip1
                text: " " + screenPhone.register_reason + " "
                color: "#000"
                anchors.margins: 6
                anchors.centerIn: parent
            }
        }
        MouseArea
        {
            anchors.fill: parent
            id: ma1
            hoverEnabled: true
        }
    }

    function setAccountText()
    {
        if (screenSettings.getActiveAccount() === '0')
        {
            return "No account set";
        }
        else
        {
           if (screenPhone.is_registered === 1)
           {
                return activeAccountUser;
           }

           if (screenPhone.is_registered === 2)
           {
                return "Registering...";
           }

           if (screenPhone.is_registered === 0)
           {
                return "Not Registered";
           }
        }
    }
}
