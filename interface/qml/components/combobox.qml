import QtQuick 2.0

Item
{
    id:combob
    property alias model: listView.model
    property string m_selectedvalue: chosenItemText.text
    property int m_list_height: 180
    height: 25

    FontLoader
    {
        id: fontMain
        source: "../../assets/OpenSans-Regular.ttf"
    }

    Rectangle
    {
        id:comboBox

        property variant items: combob.m_items
        property alias selectedItem: chosenItemText.text;
        property alias selectedIndex: listView.currentIndex;
        signal comboClicked;

        width: parent.width;
        height: parent.height;
        smooth: true;
        color: "transparent"

        Rectangle
        {
            id:chosenItem
            radius:4;
            width:parent.width;
            height:comboBox.height;
            color: ((mouseAreaSelected.containsMouse) ? "#EEE" : "#DDD")
            smooth:true;

            Text
            {
                id:chosenItemText
                anchors.top: parent.top;
                anchors.left: parent.left;
                anchors.margins: 4;
                elide: Text.ElideRight
                font.family: fontMain.name
                text: m_selectedvalue
                font.pointSize: 15;
                color: "#000"
                smooth:true
                clip: true
            }

            MouseArea
            {
                id:mouseAreaSelected
                anchors.fill: parent;
                cursorShape: "PointingHandCursor"
                onClicked:
                {
                    comboBox.state = comboBox.state==="dropDown"?"":"dropDown";
                }
            }
        }

        Rectangle
        {
            id:dropDown
            width:comboBox.width;
            height:0;
            clip:true;
            radius: 10;
            anchors.top: chosenItem.bottom;
            anchors.margins: 4;
            color: "#EEE"

            ListView
            {
                id:listView
                interactive: true
                contentHeight: m_list_height
                height: m_list_height;
                width: parent.width
                model: m_items
                currentIndex: 0
                delegate: Rectangle
                {
                    id: selIen
                    width:comboBox.width;
                    height: comboBox.height;
                    color: "transparent"
                    Text
                    {
                        text: modelData
                        anchors.top: parent.top;
                        anchors.left: parent.left;
                        anchors.margins: 2;
                        color: "#000"
                        font.pointSize: 14;
                        elide: Text.ElideRight
                        font.family: fontMain.name
                        clip: true

                    }
                    MouseArea
                    {
                        id: itemSelected
                        anchors.fill: parent;
                        cursorShape: "PointingHandCursor"
                        onClicked: {
                            m_selectedvalue = modelData;
                            comboBox.state = ""
                            var prevSelection = chosenItemText.text
                            chosenItemText.text = modelData
                            if(chosenItemText.text != prevSelection){
                                comboBox.comboClicked();
                            }
                            listView.currentIndex = index;
                        }
                    }
                }
            }

            ScrollBar
            {
                target: listView
            }
        }

        Component
        {
            id: highlight
            Rectangle
            {
                width:comboBox.width;
                height:comboBox.height;
                color: "red";
                radius: 4
            }
        }

        states: State {
            name: "dropDown";
            PropertyChanges { target: dropDown; height: m_list_height }
        }

        transitions: Transition {
            NumberAnimation { target: dropDown; properties: "height"; easing.type: Easing.OutExpo; duration: 200 }
        }
    }
}
