import QtQuick 2.0
import "../components"

MainScreen
{
    id: mainScreen

    property string m_name
    property string m_surname
    property string m_picpath

    property int selectedContact

    property alias txtActiveName: txtAName
    property string m_loader_screen

    Component.onCompleted:
    {
        screenContacts.getContacts('name', true ,"");
    }

    Text
    {
        text: "No contacts"
        color: "white"
        visible: contactListView.count <= 0
        font.family: mainScreen.m_fontMain.name
        anchors.centerIn: parent
        font.pixelSize: 30

        onVisibleChanged:
        {
            if(visible)
            {
                m_name = ""
                m_surname = ""
                m_picpath = ""
                selectedContact = 0
            }
        }
    }

    Item
    {
        width: app_width
        anchors.top: parent.top
        anchors.topMargin: contentStart
        height: parent.height - contentStart
        visible: m_loader_screen!==""

        onVisibleChanged:
        {
            contactListView = !visible;
            if(visible)
            {
                m_name = ""
                m_surname = ""
                txtActiveName.text = ""
            }
        }

        Rectangle
        {
            color: "#000"
            anchors.fill: parent
            opacity: 0.8
        }

        Loader
        {
            z: 1000
            id: contactsLoader
            anchors.fill: parent
        }
    }

    Component
    {
        id: sectionHeading
        Rectangle
        {
            height: 30
            width: contactListView.width
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
                text: section.slice(0,1).toUpperCase();
                font.bold: true
                color: "white"
                font.pixelSize: 18
                font.family: mainScreen.m_fontMain.name
                anchors
                {
                    left: parent.left
                    leftMargin: 20
                    verticalCenter: parent.verticalCenter
                }
            }
        }
    }

    Row
    {
        height: app_height - contentStart

        visible: m_loader_screen===""

        anchors
        {
            top: parent.top
            topMargin: contentStart
            left: parent.left
        }

        Item
        {
            width: 320
            height: parent.height

            Rectangle
            {
                id: searchBox
                color: "#111"
                width: parent.width
                height: 60
                visible: (contactListView.count > 0) || (txtSearchContacts.m_text !== "")

                Textbox
                {
                    id: txtSearchContacts
                    width: 250
                    height: 36
                    radius: 10
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 15
                    color: "white"
                    m_input.color: "#BBB"
                    m_text: "Search..."
                    m_input.font.pixelSize: 16
                    m_input.font.italic: true
                    m_input.anchors.topMargin: 8
                    m_input.anchors.leftMargin: 10

                    Keys.onTabPressed:
                    {
                        phoneNumber.m_input.forceActiveFocus();
                    }

                    m_input.onFocusChanged:
                    {
                        if(m_input.focus && m_text === "Search...")
                        {
                            m_name = ""
                            txtAName.text = ""
                            m_text = "";
                            m_input.color = "#111"
                        }
                    }

                    m_input.onTextChanged:
                    {
                        screenContacts.getContacts('name', true , m_input.text);
                    }
                }

                Image
                {
                    source: "../../assets/ic_search.png"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: txtSearchContacts.right
                    anchors.leftMargin: 10
                }

            }

            ListView
            {
                id: contactListView
                width: parent.width
                height: parent.height - searchBox.height
                anchors.top: searchBox.bottom
                clip: true
                focus: true
                model: contactModel
                interactive: true
                delegate: listViewRow;
                section.property: "name"
                section.criteria: ViewSection.FirstCharacter
                section.delegate: sectionHeading
                visible: contactListView.count > 0

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
            visible: m_name !== ""

            Column
            {
                width: parent.width
                height: parent.height

                Rectangle
                {
                    width: parent.width
                    height: 155

                    Image
                    {
                        fillMode: Image.PreserveAspectCrop
                        smooth: true
                        source: m_picpath==="" ? "../../assets/buddy.png" : m_picpath
                        anchors.fill: parent

                        Rectangle
                        {
                            anchors.top: parent.top
                            width: parent.width
                            height: 37
                            color: "black"
                            opacity: 0.5
                        }

                        Text
                        {
                            id: txtAName
                            height: 37
                            width:parent.width - 30
                            verticalAlignment: Text.AlignVCenter
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            font.family: mainScreen.m_fontMain.name
                            font.bold: true
                            color: "white"
                            font.pointSize: 13
                            elide: Text.ElideRight
                        }
                    }
                }

                Rectangle
                {
                    id: phoneNumbersLabel
                    width: parent.width
                    color: "transparent"
                    height: 30
                    visible: m_name!==""

                    Column
                    {
                        height: parent.height
                        width: parent.width
                        anchors
                        {
                            left: parent.left
                            top: parent.top
                        }

                        Rectangle
                        {
                            width: parent.width
                            height: 1
                            color: "#4B4C4B";
                        }

                        Rectangle
                        {
                            height: 28
                            width: parent.width
                            color: "#3C3C3C"
                            Text
                            {
                                text: "Phone Numbers"
                                width: parent.width
                                height: parent.height
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 14
                                color: "#FF6600"
                                font.family: mainScreen.m_fontMain.name
                                font.bold: true
                                x: 10
                            }
                        }

                        Rectangle
                        {
                            width: parent.width
                            height: 1
                            color: "#424442";
                        }
                    }

                    ListView
                    {
                        id: listViewNum
                        anchors.top: phoneNumbersLabel.bottom
                        anchors.topMargin: 1
                        height: 167
                        width: parent.width
                        clip: true
                        model: numbersModel
                        delegate: rowNumbers

                        ScrollBar
                        {
                            target: parent
                        }
                    }
                }
            }
        }
    }

    Component
    {
        id: listViewRow

        Item
        {
            id:listViewRowWrapper
            width: contactListView.width;
            height: 70

            Rectangle
            {
                gradient: (contact_id===selectedContact) ? selectedItem : (mare3.containsMouse) ? overItem : normalItem
                anchors.fill: parent

                Rectangle
                {
                    width: parent.width
                    height: 1
                    color: "#3c3c3c"
                }

                MouseArea
                {
                    id: mare3
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: "PointingHandCursor"
                    onClicked:
                    {
                        selectedContact = contact_id
                        txtActiveName.text =  name + " "+ surname;
                        contactListView.currentIndex = index;
                        m_name = name
                        m_surname = surname
                        m_picpath = pic_path
                        screenContacts.getNumbers(contact_id);
                    }
                }

                Row
                {
                    anchors
                    {
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                        leftMargin: 17
                    }

                    spacing: 10

                    Image
                    {
                        source: pic_path==="" ? "../../assets/buddy.png" : pic_path
                        fillMode: Image.PreserveAspectFit
                        smooth: true
                        width: 60
                        height: 60
                    }

                    Text
                    {
                        height: parent.height
                        verticalAlignment: Text.AlignVCenter
                        text: name + " " + surname
                        color: "white"
                        font.pixelSize: 20
                        font.family: mainScreen.m_fontMain.name
                        font.bold: true
                        width: 200
                        elide: Text.ElideRight
                    }
                }
            }
        }
    }

    Component
    {
        id: rowNumbers
        Column
        {
            Row
            {
                height: 60
                Column
                {
                    spacing: 3
                    width: 317
                    Text
                    {
                        text: description
                        color: "white"
                        font.pixelSize: 17
                        font.family: mainScreen.m_fontMain.name
                        font.bold: true
                        height: 28
                        x: 10
                        verticalAlignment: Text.AlignVCenter
                    }

                    Text
                    {
                        text: number
                        font.pixelSize: 15
                        font.family: mainScreen.m_fontMain.name
                        color: "#DDD"
                        height: 28
                        x: 10
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                Button
                {
                    m_image: favourite===1  ? "../../assets/favourites_on.png" : "../../assets/tab_favourites.png"
                    m_image_over: favourite===1 ? "../../assets/favourites_on.png" : "../../assets/tab_favourites.png"
                    tooltip_text: "Add to favourites"
                    width: 22
                    height: 22
                    y: 20
                    onButtonClicked:
                    {
                        if(favourite===1)
                        {
                            screenContacts.setFavourite(number_id, "0");
                        }
                        else
                        {
                            screenContacts.setFavourite(number_id, "1");
                        }
                        screenContacts.getNumbers(contact_id);
                    }
                }

                Button
                {
                    m_image: "../../assets/tab_phone.png"
                    m_image_over: "../../assets/tab_phone.png"
                    tooltip_text: "Call to this number"
                    width: 22
                    height: 22
                    y: 20
                    onButtonClicked:
                    {
                        screenPhone.setDialText(number);
                        Controller.setQml("screens/phone.qml", "phone");
                    }
                }
            }
            Rectangle
            {
                color: "#444"
                height: 1
                width: app_width - 320
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
