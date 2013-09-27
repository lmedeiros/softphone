import QtQuick 2.0

Rectangle
{
    id: button
    smooth: true

    width: 165
    height: 54

    property string m_data
    property alias m_clicker: mouseArea
    property string m_text
    property string m_image_normal
    property string m_image_over
    property string m_icon
    property string m_image_down

    property string tooltip_text
    property string tooltip_pos: "left"

    property bool orientation_column: true

    signal buttonClicked();

    color: "transparent"

    Image
    {
        id: backgroundImage
        width: parent.width
        height: parent.height
        source: mouseArea.containsMouse ? m_image_over : m_image_normal

        Row
        {
            visible: !orientation_column
            spacing: 7
            anchors.centerIn: parent

            Image
            {
                id: icon
                source: m_icon
            }

            Text
            {
                text: m_text
                color: "#9f9f9f"
                font.family: fontMain.name
                font.pixelSize: 26
                font.bold: true
            }
        }

        Image
        {
            visible: orientation_column
            source: m_icon
            anchors.horizontalCenter: parent.horizontalCenter
            y: 7
        }

        Text
        {
            visible: orientation_column
            text: m_text
            color: "#9f9f9f"
            font.pixelSize: 14
            y: 33
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    MouseArea
    {
        id: mouseArea
        cursorShape: "PointingHandCursor"
        anchors.fill: parent
        hoverEnabled: true

        onReleased:
        {
            backgroundImage.source = m_image_normal
            buttonClicked();
        }

        onPressed:
        {
            backgroundImage.source = m_image_down
        }
    }

    Rectangle
    {
        id: tooltip
        Component.onCompleted:
        {
            if(tooltip_pos==="left")
            {
                anchors.right = parent.left;
            }
            else
            if(tooltip_pos==="right")
            {
                anchors.left = parent.right;
            }

        }

        visible: tooltip_text !== "" && mouseArea.containsMouse
        clip: false
        color: "#CCC"
        width: tip.width
        height: tip.height
        opacity: 0.9
        Text
        {
            id: tip
            text: tooltip_text
            color: "#000"
            anchors.margins: 6
            anchors.centerIn: parent
        }
    }
}
