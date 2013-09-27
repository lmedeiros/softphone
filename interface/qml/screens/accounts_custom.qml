import QtQuick 2.0
import "../components"
import "../../js/account_edit.js" as ControllerJS

Rectangle
{
    property string currentAccount: accountEditModel3[0] + " - " + accountEditModel3[1];
    property string currentAccId: accountEditModel3[0];
    property string currentAccDesc: accountEditModel3[1];

    color: "transparent"

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
            ControllerJS.updateAccount();
        }
    }

    Column
    {
        spacing: 10
        width: parent.width
        anchors
        {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
        }

        Rectangle
        {
            height: 38
            width: parent.width
            color: "#121212"

            Rectangle
            {
                width: parent.width
                height: 1
                color: "#6E6E6E";
                anchors.top: parent.top
            }

            Rectangle
            {
                width: parent.width
                height: 1
                color: "#6E6E6E";
                anchors.bottom: parent.bottom
            }

            Text
            {
                text: accountEditModel3[1]
                font.bold: true
                color: "white"
                font.pixelSize: 19
                anchors
                {
                    left: parent.left
                    leftMargin: 20
                    verticalCenter: parent.verticalCenter
                }
            }
        }

        Textbox
        {
            id: txtUsername
            width: parent.width - 20
            x: 10
            height: 35
            radius: 10
            color: "white"
            m_input.anchors.topMargin: 7
            m_input.anchors.leftMargin: 10
            m_input.color: accountEditModel3[2]==="" ? "#BBB" : "#111"
            m_input.font.pixelSize: 17
            m_text: accountEditModel3[2]==="" ? "Username" : accountEditModel3[2]
            Keys.onTabPressed:
            {
                txtSecret.m_input.forceActiveFocus();
            }

            m_input.onFocusChanged:
            {
                if(m_input.focus && m_text === "Username")
                {
                    m_text = "";
                    m_input.color = "#111"
                }
            }
        }

        Textbox
        {
            id: txtSecret
            width: parent.width - 20
            x: 10
            height: 35
            radius: 10
            color: "white"
            m_input.anchors.topMargin: 7
            m_input.anchors.leftMargin: 10
            m_input.color: accountEditModel3[3]==="" ? "#BBB" : "#111"
            m_input.font.pixelSize: 17
            m_text: accountEditModel3[3]==="" ? "Password" : accountEditModel3[3]
            Keys.onTabPressed:
            {
                txtServer.m_input.forceActiveFocus();
            }

            m_input.onFocusChanged:
            {
                if(m_input.focus && m_text === "Password")
                {
                    m_text = "";
                    m_input.color = "#111"
                }
            }
        }

        Textbox
        {
            id: txtServer
            width: parent.width - 20
            x: 10
            height: 35
            radius: 10
            color: "white"
            m_input.anchors.topMargin: 7
            m_input.anchors.leftMargin: 10
            m_input.color: accountEditModel3[4]==="" ? "#BBB" : "#111"
            m_input.font.pixelSize: 17
            m_text: accountEditModel3[4]==="" ? "Server Address" : accountEditModel3[4]
            Keys.onTabPressed:
            {
                txtMailbox.m_input.forceActiveFocus();
            }

            m_input.onFocusChanged:
            {
                if(m_input.focus && m_text === "Server Address")
                {
                    m_text = "";
                    m_input.color = "#111"
                }
            }
        }

        Row
        {
            spacing: 10
            width: parent.width - 20
            x: 12
            height: 35

            Text
            {
                id: lblMailbox
                width: 160
                color: "#ffffff"
                text: qsTr("Voicemail Service")
                font.pixelSize: 15
                font.bold: true
                height: parent.height
                verticalAlignment: Text.AlignVCenter
            }

            Textbox
            {
                id: txtMailbox
                width: parent.width - 180
                x: 10
                height: 35
                radius: 10
                color: "white"
                m_input.anchors.topMargin: 7
                m_input.anchors.leftMargin: 10
                m_input.color: "#111"
                m_input.font.pixelSize: 17
                m_text: accountEditModel3[10]==="" ? "*100" : accountEditModel3[10]
                Keys.onTabPressed:
                {
                    txtUsername.m_input.forceActiveFocus();
                }
            }
        }

        Row
        {
            spacing: 10
            width: parent.width - 20
            x: 12
            height: 35

            Text
            {
                id: lblSRTP
                width: 50
                color: "#ffffff"
                text: qsTr("SRTP")
                font.pixelSize: 17
                font.bold: true
                height: parent.height
                verticalAlignment: Text.AlignVCenter
            }

            Spinbox
            {
                id: cboSRTP
                width: 120
                height: 35
                textSize: 15
                defaultValue: accountEditModel3[8]
                model: ["No", "Optional", "Required"]
            }

            Text
            {
                id: lblTLS
                width: 40
                color: "#ffffff"
                text: qsTr("Sign.")
                font.pixelSize: 16
                font.bold: true
                height: parent.height
                verticalAlignment: Text.AlignVCenter
            }

            Spinbox
            {
                id: cboTLS
                width: 120
                height: 35
                textSize: 16
                defaultValue: accountEditModel3[9]
                model: ["UDP", "TCP", "TLS"]
            }
        }


        Text
        {
            id: txtCodec
            width: 140
            color: "#ffffff"
            text: qsTr("Codec Priority")
            font.pixelSize: 17
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Row
        {
            spacing: 6
            x: 10
            Spinbox
            {
                id: cboFirstCodec
                width: 120
                height: 35
                textSize: 11
                defaultValue: accountEditModel3[5]
                model: screenPhone.codec_list
            }

            Spinbox
            {
                id: cboSecCodec
                width: 120
                height: 35
                textSize: 11
                defaultValue: accountEditModel3[6]
                model: screenPhone.codec_list
            }

            Spinbox
            {
                id: cboThirdCodec
                width: 120
                height: 35
                textSize: 11
                defaultValue: accountEditModel3[7]
                model: screenPhone.codec_list
            }
        }
    }
}
