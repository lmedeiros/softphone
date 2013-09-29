import QtQuick 2.0
import "../components"

import "../../js/config.js" as ControllerJS

Rectangle
{
    anchors.fill: parent

    color: "transparent"

    Component.onCompleted:
    {
        screenSettings.getSettings()
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
            //ControllerJS.updateSettings();
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
                id: cboVideoDevice
                width: 190
                height: 35
                textSize: 15
                model: videoDeviceModel
                defaultValue: videoDeviceName
                anchors.verticalCenter: parent.verticalCenter
            }

            PhoneActiveButton
            {
                id: btnVideo
                tooltip_text: "Enable video call"
                m_image_normal: "../../assets/Button_top_normal.png"
                m_image_over: "../../assets/Button_top_over.png"
                m_image_down: "../../assets/Button_top_down.png"
                m_icon: "../../assets/ic_video_disabled.png"
                m_text: "Preview"
                width: 45
                height: 34
                onButtonClicked:
                {
                    screenSettings.showPreviewWindow(parseInt(cboVideoDevice.selectedId));
                }
            }
        }
    }
}
