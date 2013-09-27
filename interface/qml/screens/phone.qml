import QtQuick 2.0
import "../components"
import "../../js/phone.js" as ControllerJS

MainScreen
{
    id: mainScreen

    Component.onCompleted:
    {
        keypadDisplayText.forceActiveFocus();
    }

    Timer
    {
        id: timerC1
        interval: 1000
        running: (screenPhone.c1_timer>=0) ? true : false
        repeat: true
        onTriggered:
        {
            screenPhone.c1_timer++;
        }
    }

    Timer
    {
        id: timerC2
        interval: 1000
        running: (screenPhone.c2_timer>=0) ? true : false
        repeat: true
        onTriggered:
        {
            screenPhone.c2_timer++;
        }
    }

    Row
    {
        height: app_height - contentStart
        anchors
        {
            top: parent.top
            topMargin: contentStart
            left: mainScreen.left
        }

        Item
        {
            width: 320
            height: parent.height

            Rectangle
            {
                id: phoneDisplay
                width: keypad.width
                smooth: true
                height: 38
                color: "#FFF"
                radius: 8
                anchors
                {
                    top: parent.top
                    topMargin: 18
                    horizontalCenter: parent.horizontalCenter
                }

                Button
                {
                    id: btnBackspace
                    z: 100
                    anchors
                    {
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                        rightMargin: 5
                    }

                    width: 36
                    height: 21
                    m_data: "backspace"
                    m_image_over: "../../assets/btn_backspace_over.png"
                    m_image: "../../assets/btn_backspace.png"
                    visible: keypadText!==""

                    m_clicker.onPressAndHold:
                    {
                        screenPhone.keypadEvent("clear");
                    }

                    onButtonClicked:
                    {
                        screenPhone.keypadEvent(m_data);
                    }
                }

                Text
                {
                    z: 90
                    id: keypadDisplayText
                    anchors
                    {
                        top: parent.top
                        left: parent.left
                        leftMargin: 5
                    }
                    height: parent.height + 3
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width - btnBackspace.width - 10
                    color: "black";
                    text: keypadText
                    font.pointSize: 16
                    font.bold: true
                    elide: Text.ElideRight;
                    font.family: m_fontMain.name

                    onTextChanged:
                    {
                        if(!text)
                        {
                            screenLog.contactInfo("000000000000000000000000000000000000");
                        }
                        else
                        {
                            screenLog.contactInfo(text);
                        }
                    }

                    Keys.onDigit0Pressed:
                    {
                        screenPhone.keypadEvent("0");
                    }
                    Keys.onDigit1Pressed:
                    {
                        screenPhone.keypadEvent("1");
                    }
                    Keys.onDigit2Pressed:
                    {
                        screenPhone.keypadEvent("2");
                    }
                    Keys.onDigit3Pressed:
                    {
                        screenPhone.keypadEvent("3");
                    }
                    Keys.onDigit4Pressed:
                    {
                        screenPhone.keypadEvent("4");
                    }
                    Keys.onDigit5Pressed:
                    {
                        screenPhone.keypadEvent("5");
                    }
                    Keys.onDigit6Pressed:
                    {
                        screenPhone.keypadEvent("6");
                    }
                    Keys.onDigit7Pressed:
                    {
                        screenPhone.keypadEvent("7");
                    }
                    Keys.onDigit8Pressed:
                    {
                        screenPhone.keypadEvent("8");
                    }
                    Keys.onDigit9Pressed:
                    {
                        screenPhone.keypadEvent("9");
                    }
                    Keys.onAsteriskPressed:
                    {
                        screenPhone.keypadEvent("*");
                    }
                    Keys.onEscapePressed:
                    {
                        ControllerJS.hangup("hangup");
                    }
                    Keys.onEnterPressed:
                    {
                        ControllerJS.callAnswer();
                    }
                    Keys.onReturnPressed:
                    {
                        ControllerJS.callAnswer();
                    }
                    Keys.onDeletePressed:
                    {
                        screenPhone.keypadEvent("backspace");
                    }
                    Keys.onPressed:
                    {
                        if(event.key === 35)
                        {
                            screenPhone.keypadEvent("#");
                        }

                        if(event.key === 16777219)
                        {
                            screenPhone.keypadEvent("backspace");
                        }
                    }
                }
            }

            Keypad
            {
                id: keypad
                anchors
                {
                    top: phoneDisplay.bottom
                    topMargin: 14
                    horizontalCenter: parent.horizontalCenter
                }
            }
        }

        Item
        {
            width: app_width - 320
            height: parent.height

            Rectangle
            {
                id: suggestions
                color: "#FFF"
                width: 365
                height: (50+listViewSuggestions.count*68 >= 310) ? 310 : 50+listViewSuggestions.count*68
                y: 18
                radius: 8
                smooth: true
                visible: listViewSuggestions.count > 0 ;
                z: 10000000

                Image
                {
                    source: "../../assets/suggestion.png"
                    width: 15
                    height: 15
                    x: -15
                    y: 12
                }

                Column
                {
                    x: 10
                    y: 10
                    spacing: 5
                    Text
                    {
                        text: "Suggestions"
                        color: "#343434"
                        font.pixelSize: 15
                    }

                    Rectangle
                    {
                        color: "#DDD"
                        height: 1
                        width: suggestions.width - 10
                    }

                    ListView
                    {
                        id: listViewSuggestions
                        model: suggestionModel
                        clip: true
                        height: suggestions.height - 50
                        width: suggestions.width - 20
                        delegate: suggestionRow
                        spacing: 4
                    }
                }
            }

            Image
            {
                id: advertising
                opacity: 0
                anchors.fill: parent
                source: advertising.source = "../../assets/advert3.png"
                Component.onCompleted: fadeIn.start();

                Rectangle
                {
                    color: "#000";
                    opacity: 0.7;
                    visible: suggestions.visible
                    anchors.fill: parent
                }
            }

            PropertyAnimation
            {
                id: fadeIn
                target: advertising
                properties: "opacity"
                from: 0
                to: 1
                duration: 800
            }
            PropertyAnimation
            {
                id: fadeOut
                target: advertising
                properties: "opacity"
                from: 1
                to: 0
                duration: 800
            }

            Timer
            {
                running: true;
                interval: 10000;
                repeat: true;
                property int adv_num: 1

                onTriggered:
                {
                    fadeOut.start();
                    advertising.source = "../../assets/advert" + adv_num + ".png"
                    fadeIn.start();
                    adv_num = (adv_num===3) ? 1 : adv_num+1
                }
            }
        }
    }

    Component
    {
        id: suggestionRow
        Column
        {
            spacing: 4
            Row
            {
                id: rowSugs
                width: suggestions.width
                height: 60
                spacing: 10

                Image
                {
                   id: contact_image
                   fillMode: Image.PreserveAspectFit
                   source: (pic_path==="") ? "../../assets/buddy.png" : pic_path
                   width: 60
                   height: 60
                }
                Column
                {
                    spacing: 12
                    y:5
                    Text
                    {
                       width: 200
                       font.pixelSize: 14
                       text: number + " (" + description + ")"
                       color: "#323232"
                    }

                    Text
                    {
                       width: 200
                       font.pixelSize: 15
                       color: "#323232"
                       text: name + " " + surname
                       elide: Text.ElideRight
                    }
                }
            }

            Rectangle
            {
                color: "#DDD"
                height: 1
                width: suggestions.width - 10
            }
        }
    }
}
