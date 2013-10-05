import QtQuick 2.0
import "../../js/keypad.js" as ControllerJS

Item
{
    width: (button_w * 3) + (2 * button_spacing)
    height: (button_h * 5) + (4 * button_spacing)

    property int button_w: 97
    property int button_h: 54
    property int button_spacing: 0

    FontLoader
    {
        id: fontMain
        source: "../../assets/OpenSans-Regular.ttf"
    }

    Column
    {
        spacing: button_spacing + 5

        Row
        {
            spacing: button_spacing
            Keypadbutton
            {
                id: btnK01
                m_data: screenPhone.mwi_info > 0 ? "1vm" : "1"
                m_clicker.onPressAndHold:
                {
                    held = true;
                    if(activeAccountMailbox!== "")
                    {
                        screenPhone.keypadEvent(activeAccountMailbox);
                    }
                }
            }

            Keypadbutton
            {
                id: btnK02
                m_data: "2"
            }

            Keypadbutton
            {
                id: btnK03
                m_data: "3"
            }
        }

        Row
        {
            spacing: button_spacing
            Keypadbutton
            {
                id: btnK04
                m_data: "4"
            }

            Keypadbutton
            {
                id: btnK05
                m_data: "5"
            }

            Keypadbutton
            {
                id: btnK06
                m_data: "6"
            }
        }

        Row
        {
            spacing: button_spacing
            Keypadbutton
            {
                id: btnK07
                m_data: "7"
            }

            Keypadbutton
            {
                id: btnK08
                m_data: "8"
            }

            Keypadbutton
            {
                id: btnK09
                m_data: "9"
            }
        }

        Row
        {
            spacing: button_spacing
            Keypadbutton
            {
                id: btnKs
                m_data: "*"
            }

            Keypadbutton
            {
                id: btnK0
                m_data: "0"
                m_clicker.onPressAndHold:
                {
                    held = true;
                    screenPhone.keypadEvent("+");
                }
            }

            Keypadbutton
            {
                id: btnKp
                m_data: "#"

            }
        }

        Row
        {
            spacing: 5
            width: 290
            height: 54

            Rectangle
            {
                id: btnDial
                smooth: true

                width: 142
                height: 54

                property string m_data
                property alias m_clicker: mouseArea

                enabled: (screenPhone.is_registered===1)

                color: "transparent"

                Image
                {
                    id: backgroundImage
                    width: 142
                    source: mouseArea.containsMouse ? "../../assets/button_call_over.png" : "../../assets/button_call_normal.png"

                    Row
                    {
                        spacing: 7
                        anchors.centerIn: parent

                        Image
                        {
                            id: number
                            source: "../../assets/dialpad_call.png"
                        }

                        Text
                        {
                            text: "Call"
                            color: "#9f9f9f"
                            font.family: fontMain.name
                            font.pixelSize: 26
                            font.bold: true
                        }
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
                        ControllerJS.callAnswer(0);
                        backgroundImage.source = "../../assets/button_call_normal.png"
                    }

                    onPressed:
                    {
                        backgroundImage.source = "../../assets/button_call_down.png"
                    }
                }
            }

            Rectangle
            {
                id: btnDialVideo
                smooth: true

                width: 142
                height: 54

                property string m_data
                property alias m_clicker: mouseAreaVideo

                enabled: (screenPhone.is_registered===1)

                color: "transparent"

                Image
                {
                    id: backgroundImageVideo
                    width: 142
                    source: mouseAreaVideo.containsMouse ? "../../assets/button_call_over.png" : "../../assets/button_call_normal.png"

                    Row
                    {
                        spacing: 7
                        anchors.centerIn: parent

                        Image
                        {
                            id: numberVideo
                            source: "../../assets/ic_video_on.png"
                            y: 8
                        }

                        Text
                        {
                            text: "Video"
                            color: "#9f9f9f"
                            font.family: fontMain.name
                            font.pixelSize: 26
                            font.bold: true
                        }
                    }

                }

                MouseArea
                {
                    id: mouseAreaVideo
                    cursorShape: "PointingHandCursor"
                    anchors.fill: parent
                    hoverEnabled: true
//                    enabled: false

                    onReleased:
                    {
                        ControllerJS.callAnswer(1);
                        backgroundImageVideo.source = "../../assets/button_call_normal.png"
                    }

                    onPressed:
                    {
                        backgroundImageVideo.source = "../../assets/button_call_down.png"
                    }
                }
            }
        }
    }
}
