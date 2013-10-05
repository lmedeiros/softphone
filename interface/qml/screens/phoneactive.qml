import QtQuick 2.0
import "../components"
import "../parts"
import "../../js/phone.js" as ControllerJS

Rectangle
{
    id: mainScreen


    property bool is_c1recording: false
    property bool is_c2recording: false
    property bool transferOn: false

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

    Keys.onPressed:
    {
        if(event.key === 35)
        {
            screenPhone.keypadEvent("#");
        }
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
            screenPhone.setNetworkRates();
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
            screenPhone.setNetworkRates();
        }
    }

    Row
    {
        height: app_height
        anchors
        {
            top: parent.top
            left: mainScreen.left
        }

        Item
        {
            width: 320
            height: parent.height

            Component.onCompleted:
            {
                forceActiveFocus();
            }

            focus: true;

            Image
            {
                id: advertising
                opacity: 0
                anchors.top: parent.top
                anchors.topMargin: 62
                source: advertising.source = "../../assets/advert_1.jpg"
                Component.onCompleted: fadeIn.start();
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
                interval: 5000;
                repeat: true;
                property int adv_num: 3

                onTriggered:
                {
                    fadeOut.start();
                    advertising.source = "../../assets/advert_" + adv_num + ".jpg"
                    fadeIn.start();
                    adv_num = (adv_num===3) ? 1 : adv_num+1
                }
            }

            Column
            {
                height: 98
                width: parent.width
                Rectangle
                {
                    color: "#000"
                    height: 62
                    width: parent.width

                    Row
                    {
                        anchors.fill: parent
                        anchors.margins: 7
                        spacing: 10
                        Image
                        {
                            source: (screenPhone.active_channel===1) ? (activeCall1Contact[2]==="" ? "../../assets/buddy.png" : activeCall1Contact[2]) : (activeCall2Contact[2]==="" ? "../../assets/buddy.png" : activeCall2Contact[2])
                            width: 48
                            fillMode: Image.PreserveAspectFit
                            height: 48
                        }

                        Column
                        {
                            y:3
                            spacing: 3
                            Text
                            {
                                id: txtContactNameLeft
                                color: "#FF6600"
                                text: (screenPhone.active_channel===1) ? (activeCall1Contact[0] + " " + activeCall1Contact[1]) : (activeCall2Contact[0] + " " + activeCall2Contact[1])
                                font.pixelSize: 18
                                font.bold: true
                            }

                            Text
                            {
                                id: txtContactNumberLeft
                                text:  (screenPhone.active_channel===1) ? (activeCall1Contact[4] + " - " + activeCall1Contact[3]) : (activeCall2Contact[4] + " - " + activeCall2Contact[3])
                                color: "#CCC"
                                font.pixelSize: 16
                                font.bold: true
                            }
                        }
                    }
                }

                Rectangle
                {
                    color: "transparent"
                    height: 35
                    width: parent.width

                    Rectangle
                    {
                        color: "#000"
                        height: 35
                        opacity: 0.6
                        width: parent.width
                    }

                    Text
                    {
                        id: txtTimer
                        anchors
                        {
                            centerIn: parent
                        }
                        color: "#ffffff"
                        visible: text!=="00:00:00"
                        text:(screenPhone.active_channel===1) ? ControllerJS.secToTime(screenPhone.c1_timer) : ControllerJS.secToTime(screenPhone.c2_timer);
                        font.bold: false
                        font.pointSize: 11
                    }
                }
            }

            Rectangle
            {
                anchors.bottom: parent.bottom
                width: parent.width
                height: 30
                color: "#222"
                opacity: 0.9
            }
        }

        Item
        {
            width: app_width - 320
            height: parent.height

            Rectangle
            {
                anchors.fill: parent
                color: "#313131"

                Text
                {
                    id: lblOutgoing
                    height: 25
                    text: "Outgoing Calls"
                    color: "#FFF";
                    font.pixelSize: 20
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.margins: 20
                }

                Column
                {
                    anchors.top: lblOutgoing.bottom
                    anchors.left: parent.left
                    anchors.margins: 20
                    width: parent.width
                    height: 130
                    spacing: 8

                    Row
                    {
                        id: c1_row
                        width: parent.width
                        height: 50
                        visible: screenPhone.c1_sip_actiontext !== ""
                        spacing: 10

                        MouseArea
                        {
                            anchors.fill: parent
                            cursorShape: "PointingHandCursor"
                            hoverEnabled: true
                            visible: screenPhone.active_channel === 2
                            onClicked:
                            {
                                ControllerJS.channelPress("c1");
                            }
                        }

                        Image
                        {
                            source: (activeCall1Contact[2] === "") ? "../../assets/buddy.png" : activeCall1Contact[2]
                            width: 35
                            fillMode: Image.PreserveAspectFit
                            height: 35
                        }

                        Text
                        {
                            text: screenPhone.c1_sip_actiontext;
                            color: "#FF5400"
                            font.pixelSize: 14
                            font.bold: true
                            wrapMode: Text.WordWrap
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            width: 195
                            height:35
                        }

                        PhoneActiveButton
                        {
                            id: btnAnswer
                            visible: screenPhone.c1_state==="incoming"
                            tooltip_text: "Answer the call C1"
                            m_image_normal: "../../assets/Button_top_normal.png"
                            m_image_over: "../../assets/Button_top_over.png"
                            m_image_down: "../../assets/Button_top_down.png"
                            m_icon: "../../assets/ic_call_small.png"
                            orientation_column: false
                            width: 44
                            height: 40
                            onButtonClicked:
                            {
                                if(screenPhone.c2_call_id >= 0) telapi.hold(screenPhone.c2_call_id);
                                telapi.answerCall(screenPhone.c1_call_id);
                            }
                        }

                        PhoneActiveButton
                        {
                            id: btnReject
                            visible: screenPhone.c1_call_id >= 0
                            tooltip_text: "Reject the call C1"
                            m_image_normal: "../../assets/Button_top_normal.png"
                            m_image_over: "../../assets/Button_top_over.png"
                            m_image_down: "../../assets/Button_top_down.png"
                            m_icon: "../../assets/ic_call_end_small.png"
                            orientation_column: false
                            width: 44
                            height: 40
                            onButtonClicked:
                            {
                                ControllerJS.hangup(screenPhone.c1_call_id);
                                if(screenPhone.c2_call_id >= 0  && screenPhone.c2_state === "hold") telapi.unhold(screenPhone.c2_call_id);
                            }
                        }
                    }

                    Row
                    {
                        id: c2_row
                        width: parent.width
                        height: 50
                        visible: screenPhone.c2_sip_actiontext !== ""
                        spacing: 10

                        MouseArea
                        {
                            anchors.fill: parent
                            cursorShape: "PointingHandCursor"
                            hoverEnabled: true
                            visible: screenPhone.active_channel === 1
                            onClicked:
                            {
                                ControllerJS.channelPress("c2");
                            }
                        }

                        Image
                        {
                            source: (activeCall2Contact[2] === "") ? "../../assets/buddy.png" : activeCall2Contact[2]
                            width: 35
                            fillMode: Image.PreserveAspectFit
                            height: 35
                        }

                        Text
                        {
                            text: screenPhone.c2_sip_actiontext;
                            color: "#FF5400"
                            font.pixelSize: 14
                            font.bold: true
                            wrapMode: Text.WordWrap
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            width: 195
                            height:35
                        }

                        PhoneActiveButton
                        {
                            id: btnAnswerC2
                            visible: screenPhone.c2_state==="incoming"
                            tooltip_text: "Answer the call C2"
                            m_image_normal: "../../assets/Button_top_normal.png"
                            m_image_over: "../../assets/Button_top_over.png"
                            m_image_down: "../../assets/Button_top_down.png"
                            m_icon: "../../assets/ic_call_small.png"
                            orientation_column: false
                            width: 44
                            height: 40
                            onButtonClicked:
                            {
                                if(screenPhone.c1_call_id >= 0) telapi.hold(screenPhone.c1_call_id);
                                telapi.answerCall(screenPhone.c2_call_id);
                            }
                        }

                        PhoneActiveButton
                        {
                            id: btnRejectC2
                            visible: screenPhone.c2_call_id >= 0
                            tooltip_text: "Reject the call C2"
                            m_image_normal: "../../assets/Button_top_normal.png"
                            m_image_over: "../../assets/Button_top_over.png"
                            m_image_down: "../../assets/Button_top_down.png"
                            m_icon: "../../assets/ic_call_end_small.png"
                            orientation_column: false
                            width: 44
                            height: 40
                            onButtonClicked:
                            {
                                ControllerJS.hangup(screenPhone.c2_call_id);
                                if(screenPhone.c1_call_id >= 0 && screenPhone.c1_state === "hold") telapi.unhold(screenPhone.c1_call_id);
                            }
                        }
                    }

                    Text
                    {
                        text: "Networking Status"
                        font.pixelSize: 13
                        visible: (screenPhone.active_channel === 1) ? (screenPhone.c1_uprate !== "") : (screenPhone.c2_uprate !== "")
                        color: "white"
                        font.bold: true
                    }

                    Text
                    {
                        id: netstatus
                        text: (screenPhone.active_channel === 1) ? "Call Info: " + screenPhone.c1_stream_info + " \nDownload Rate: " + screenPhone.c1_downrate + " \nUpload Rate: " + screenPhone.c1_uprate + " \nLoss: " + screenPhone.c1_loss : "Codec: " + screenPhone.c2_stream_info + " \nDownload Rate: " + screenPhone.c2_downrate + " \nUpload Rate: " + screenPhone.c2_uprate + " \nLoss: " + screenPhone.c2_loss
                        font.pixelSize: 13
                        visible: (screenPhone.active_channel === 1) ? (screenPhone.c1_uprate !== "") : (screenPhone.c2_uprate !== "")
                        color: "white"
                    }
                }

                Column
                {
                    height: 130
                    width: parent.width
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom

                    Row
                    {
                        height: 65
                        spacing: 4
                        anchors.horizontalCenter: parent.horizontalCenter

//                        PhoneActiveButton
//                        {
//                            id: btnVideo
//                            tooltip_text: "Enable video call"
//                            m_image_normal: "../../assets/Button_top_normal.png"
//                            m_image_over: "../../assets/Button_top_over.png"
//                            m_image_down: "../../assets/Button_top_down.png"
//                            m_icon: "../../assets/ic_video_disabled.png"
//                            m_text: "Video"
//                            width: 65
//                            height: 54
//                        }

                        PhoneActiveButton
                        {
                            id: btnHold
                            tooltip_text: "Hold/Unhold active call"
                            m_image_normal: "../../assets/Button_top_normal.png"
                            m_image_over: "../../assets/Button_top_over.png"
                            m_image_down: "../../assets/Button_top_down.png"
                            m_icon: ControllerJS.holdIcon();
                            m_text: "Hold"
                            width: 65
                            height: 54
                            onButtonClicked:
                            {
                                ControllerJS.hold();
                            }
                        }

                        PhoneActiveButton
                        {
                            id: btnTransfer
                            tooltip_text: "Transfer active call"
                            m_image_normal: "../../assets/Button_top_normal.png"
                            m_image_over: "../../assets/Button_top_over.png"
                            m_image_down: "../../assets/Button_top_down.png"
                            enabled: (screenPhone.c1_state === "busy" && screenPhone.active_channel === 1) || (screenPhone.c2_state === "busy" && screenPhone.active_channel === 2)
                            m_icon: ControllerJS.transferIcon();
                            m_text: "Transfer"
                            width: 65
                            height: 54

                            onButtonClicked:
                            {
                                if(transferOn)
                                {
                                    txtTransfer.forceActiveFocus();
                                }

                                transferOn = !transferOn;
                            }
                        }

//                        PhoneActiveButton
//                        {
//                            id: btnAdd
//                            tooltip_text: "Add this number to contacts"
//                            m_image_normal: "../../assets/Button_top_normal.png"
//                            m_image_over: "../../assets/Button_top_over.png"
//                            m_image_down: "../../assets/Button_top_down.png"
//                            m_icon: "../../assets/ic_add_disabled.png"
//                            m_text: "Add"
//                            width: 65
//                            height: 54
//                        }

                        PhoneActiveButton
                        {
                            id: btnRecord
                            tooltip_text: "Start record active call"
                            m_image_normal: "../../assets/Button_top_normal.png"
                            m_image_over: "../../assets/Button_top_over.png"
                            m_image_down: "../../assets/Button_top_down.png"
                            enabled: (screenPhone.c1_state === "busy" && screenPhone.active_channel === 1) || (screenPhone.c2_state === "busy" && screenPhone.active_channel === 2)
                            m_icon:  (is_c1recording || is_c2recording) ? "../../assets/ic_record_on.png" : "../../assets/ic_record_off.png"
                            m_text: "Record"
                            width: 65
                            height: 54
                            onButtonClicked:
                            {
                                if (screenPhone.active_channel===1)
                                {
                                    if(!is_c1recording)
                                    {
                                        telapi.recordCall(screenPhone.c1_call_id)
                                        is_c1recording=true;
                                    }
                                    else
                                    {
                                        telapi.stopRecordCall(screenPhone.c1_call_id);
                                        is_c1recording=false;
                                    }
                                }
                                else
                                {
                                    if(!is_c2recording)
                                    {
                                        telapi.recordCall(screenPhone.c2_call_id)
                                        is_c2recording=true;
                                    }
                                    else
                                    {
                                        telapi.stopRecordCall(screenPhone.c2_call_id);
                                        is_c2recording=false;
                                    }
                                }
                            }
                        }
                    }

                    Row
                    {
                        spacing: 4
                        anchors.horizontalCenter: parent.horizontalCenter
                        height: 65

                        PhoneActiveButton
                        {
                            id: btnMute
                            tooltip_text: "Mute/Unmute the microphone"
                            m_image_normal: "../../assets/button_normal.png"
                            m_image_over: "../../assets/button_over.png"
                            m_image_down: "../../assets/button_down.png"
                            m_icon: (micVolume>0) ? "../../assets/ic_mute_off.png" :"../../assets/ic_mute_on.png"
                            m_text: "Mute"
                            width: 84
                            height: 54

                            onButtonClicked:
                            {
                                (micVolume>0) ? screenPhone.setVolume(0.0,"mic") : screenPhone.setVolume(0.9,"mic");
                            }
                            Component.onCompleted:
                            {
                                console.debug("mic volumeddd: " + micVolume)
                            }
                        }

                        PhoneActiveButton
                        {
                            id: btnSpeaker
                            tooltip_text: "Mute/Unmute the speaker"
                            m_image_normal: "../../assets/button_normal.png"
                            m_image_over: "../../assets/button_over.png"
                            m_image_down: "../../assets/button_down.png"
                            m_icon: (spkVolume>0) ? "../../assets/ic_speaker_on.png" : "../../assets/ic_speaker_off.png"
                            m_text: "Speaker"
                            width: 84
                            height: 54

                            onButtonClicked:
                            {
                                (spkVolume>0) ? screenPhone.setVolume(0.0,"spk") : screenPhone.setVolume(0.9,"spk");

                            }
                            Component.onCompleted:
                            {
                                console.debug("spk volumeddd: " + spkVolume)
                            }
                        }

                        PhoneActiveButton
                        {
                            id: btnHangup
                            enabled: ((screenPhone.c1_call_id >= 0) || (screenPhone.c2_call_id >= 0))
                            tooltip_text: "End current call"
                            m_image_normal: "../../assets/button_call_normal.png"
                            m_image_over: "../../assets/button_call_over.png"
                            m_image_down: "../../assets/button_call_down.png"
                            m_icon: "../../assets/ic_call_end.png"
                            m_text: "End"
                            width: 165
                            height: 54
                            orientation_column: false

                            onButtonClicked:
                            {
                                if(screenPhone.c1_call_id >= 0)
                                {
                                    ControllerJS.hangup(screenPhone.c1_call_id);
                                }

                                if(screenPhone.c2_call_id >= 0)
                                {
                                    ControllerJS.hangup(screenPhone.c2_call_id);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    FontLoader
    {
        id: fontMain
        source: "../../assets/OpenSans-Regular.ttf"
    }

    Messagebox
    {
        id: mbox
        anchors.centerIn: parent
        state: "hide"
        boxHeight: 120
        boxWidth: 300
    }

    Rectangle
    {
        color: "#111"
        opacity: 0.8
        anchors.fill: parent
        visible: transferOn
        z:99
        MouseArea
        {
            anchors.fill: parent
            onClicked:
            {
                transferOn = false;
            }
        }
    }

    Rectangle
    {
        id: transferBox
        width: 300
        height: 130
        anchors.centerIn: parent
        radius: 10
        color: "#323232"
        z: 100
        border.width: 3
        border.color: "#FFF"
        smooth: true
        visible: transferOn

        Text
        {
            text: "Transfer to:"
            color: "#FFF"
            font.pixelSize: 15
            focus: true
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 15
            font.bold: true
        }

        Row
        {
            anchors.centerIn: parent
            width: parent.width - 40
            height: 40
            spacing: 20

            Textbox
            {
                id: txtTransfer
                focus: true
                height: 35
                color: "white"
                m_input.anchors.topMargin: 7
                m_input.anchors.leftMargin: 10
                m_input.color: "#111"
                m_input.font.pixelSize: 17
                width: 130
            }

            PhoneActiveButton
            {
                id: btnTransferTo
                tooltip_text: "Confirm call transfer"
                m_image_normal: "../../assets/button_normal.png"
                m_image_over: "../../assets/button_over.png"
                m_image_down: "../../assets/button_down.png"
                m_text: "Transfer"
                m_icon: "../../assets/ic_transfer_enabled.png"
                width: 64
                height: 64

                onButtonClicked:
                {
                    if(txtTransfer.m_text!=="")
                    {
                        transferOn = false;

                        if(screenPhone.active_channel===1)
                        {
                            telapi.transferCall(screenPhone.c1_call_id, txtTransfer.m_text.toString());

                        }
                        else
                        {
                            telapi.transferCall(screenPhone.c2_call_id, txtTransfer.m_text.toString());
                        }
                    }
                    txtTransfer.m_text="";
                }
            }
        }
    }
}
