import QtQuick 2.0

Rectangle
{
    id:textBox
    property int m_maxchar: 60
    property bool m_password: false
    property alias m_text: textInput.text
    property alias m_input: textInput
    smooth: true

    FontLoader
    {
        id: fontMain
        source: "../../assets/OpenSans-Regular.ttf"
    }

    MouseArea
    {
        id: mouseArea
        cursorShape: "IBeamCursor"
        hoverEnabled: true
        anchors.fill: parent
    }

    color: ((mouseArea.containsMouse | textInput.activeFocus) ? "#444" : "#333")

    radius: 3
    TextInput {
        anchors.leftMargin: 4
        anchors.topMargin: 4
        anchors.rightMargin: 4
        maximumLength: m_maxchar
        id: textInput
        anchors.fill: parent
        color: "#FFFFFF"
        echoMode: ((m_password) ? TextInput.Password : TextInput.Normal)
        selectByMouse: true
        selectedTextColor: "#000000"
        selectionColor: "#FFFFFF"
        horizontalAlignment: TextInput.AlignLeft
        cursorVisible: false
        font.pixelSize: 13
        font.family: fontMain.name
        onTextChanged:
        {
            m_text = textInput.text
        }
    }
}
