import QtQuick 2.0

Rectangle
{
    id: menubutton
    smooth: true
    width: (m_text!=="") ? 105 : 34
    height: 34

    property string m_screenqml
    property string m_image
    property string m_text_color: "white"
    property string m_text
    property string m_data
    property alias m_clicker: mouseArea
    property string tooltip_text
    property string tooltip_pos: "left"

    signal buttonClicked();

    gradient: (activeScreen===m_data) ? gradientOver : (m_clicker.containsMouse) ? gradientActive : (activeScreen===m_data) ? gradientOver: gradientNormal

    onButtonClicked:
    {
        Controller.setQml(m_screenqml, m_data);
    }

    MouseArea
    {
        id: mouseArea
        cursorShape: "PointingHandCursor"
        anchors.fill: parent
        hoverEnabled: true
        onClicked:
        {
            buttonClicked();
        }
    }

    Row
    {
        spacing: 7
        anchors
        {
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: 5
        }

        Image
        {
            source: m_image
        }

        Text
        {
            height: parent.height
            text: m_text
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 13
            font.bold: true
            font.family: mainScreen.m_fontMain.name
            color: m_text_color
        }
    }

    Rectangle
    {
        id: tooltip
        opacity: 0.8
        Component.onCompleted:
        {
            if(tooltip_pos==="left")
            {
                anchors.left = parent.left;
            }
            else
            if(tooltip_pos==="right")
            {
                anchors.right = parent.right;
            }
        }

        visible: tooltip_text !== "" && mouseArea.containsMouse
        clip: false
        color: "#CCC"
        width: tip.width
        height: tip.height
        Text
        {
            id: tip
            text: tooltip_text
            color: "#555"
            verticalAlignment: Text.AlignVCenter
        }
    }
}


