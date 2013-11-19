import QtQuick 2.0
import "../components"

import "../../js/config.js" as ControllerJS

Rectangle
{
    anchors.fill: parent

    color: "transparent"

    Component.onCompleted:
    {
        screenSettings.getSettings();
    }

    Button
    {
        id: btnConfirmAdd
        width: 40
        height: 25
        anchors.right: parent.right
        anchors.rightMargin: 60
        y: -33
        Text
        {
            anchors.centerIn: parent
            text: "Save"
            font.pixelSize: 18
            font.bold: true
            color: !parent.m_clicker.containsMouse ? "#FF6600" : "#FF9900"
        }

        onButtonClicked:
        {
            ControllerJS.saveAudioSettings();
        }
    }

    Column
    {
        spacing: 15
        anchors
        {
            top: parent.top
            topMargin: 30
            horizontalCenter: parent.horizontalCenter
        }

        Row
        {
            spacing: 6
            Text
            {
                id: lblAudioInput
                width: 80
                color: "#ffffff"
                text: "Input Device"
                font.pixelSize: 11
                font.bold: true
                horizontalAlignment: Text.AlignRight
                anchors.verticalCenter: parent.verticalCenter
            }

            Spinbox
            {
                id: cboInputDevice
                width: 270
                height: 35
                textSize: 15
                model: audioDeviceInputModel
                defaultValue: audioInputDeviceName
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Row
        {
            spacing: 6
            Text
            {
                id: lblAudioOutput
                width: 80
                color: "#ffffff"
                text: "Output Device"
                font.pixelSize: 11
                font.bold: true
                horizontalAlignment: Text.AlignRight
                anchors.verticalCenter: parent.verticalCenter
            }

            Spinbox
            {
                id: cboOutputDevice
                width: 270
                height: 35
                textSize: 15
                model: audioDeviceOutputModel
                defaultValue: audioOutputDeviceName
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Row
        {
            visible: false
            spacing: 6
            Text
            {
                id: lblAudioRinging
                width: 80
                color: "#ffffff"
                text: "Ringing Device"
                font.pixelSize: 11
                font.bold: true
                horizontalAlignment: Text.AlignRight
                anchors.verticalCenter: parent.verticalCenter
            }

            Spinbox
            {
                id: cboRingingDevice
                width: 270
                height: 35
                textSize: 15
                model: audioDeviceRingingModel
                defaultValue: audioRingingDeviceName
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Row
        {
            visible: false
            spacing: 6
            Text
            {
                id:lblSipPort
                width: 80
                color: "#ffffff"
                text: qsTr("Sip Port")
                font.bold: true
                font.pixelSize: 11
                horizontalAlignment: Text.AlignRight
                anchors.verticalCenter: parent.verticalCenter
            }

            Textbox
            {
                id: txtSipPort
                width: 100
                height: 25
                m_text: sipPort
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }
}
