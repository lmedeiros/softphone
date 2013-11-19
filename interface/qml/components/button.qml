import QtQuick 2.0

Rectangle
{
    id: button
    smooth: true

    signal buttonClicked();
    signal buttonPressed();

    property string m_image
    property string m_image_over
    property string m_data
    property alias m_clicker: mouseArea
    property string tooltip_text
    property string tooltip_pos: "left"

    color: "transparent"

    Image
    {
        id: backgroundImage
        fillMode: Image.PreserveAspectFit
        anchors.fill: parent
        source: mouseArea.containsMouse ? m_image_over : m_image
    }

    MouseArea
    {
        id: mouseArea
        cursorShape: "PointingHandCursor"
        anchors.fill: parent
        hoverEnabled: true
        onReleased:
        {
            button.buttonClicked()
            backgroundImage.source = m_image;
        }

        onPressed:
        {
            button.buttonPressed();
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
        opacity: 0.8
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
