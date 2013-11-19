import QtQuick 2.0

Rectangle
{
    id: button
    smooth: true

    property bool held: false

    width: 97
    height: 54

    property string m_data
    property alias m_clicker: mouseArea

    color: "transparent"

    Image
    {
        id: backgroundImage
        source: mouseArea.containsMouse ? "../../assets/button_over.png" : "../../assets/button_normal.png"

        Image
        {
            id: number
            source: "../../assets/dialpad_" + ((m_data==='*') ? "s" : (m_data==='#') ? "p" : m_data) + ".png"
            anchors.centerIn: parent
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
            if(!held)
            {
                screenPhone.keypadEvent(m_data ==="1vm" ? "1" : m_data);
            }
            else
            {
                held = false;
            }

            backgroundImage.source = "../../assets/button_normal.png"
        }

        onPressed:
        {
            backgroundImage.source = "../../assets/button_down.png"
        }
    }
}

