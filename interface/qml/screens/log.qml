import QtQuick 2.0
import "../components"

MainScreen
{
    id: mainScreen

    property string m_duration
    property string m_moment
    property string m_type
    property string m_log_id
    property string m_to_num
    property string m_description
    property string m_pic_path
    property string m_rx
    property string m_tx
    property string m_cost
    property string m_codec
    property string m_local_loss
    property string m_remote_loss
    property bool is_contact: false

    property int selectedLog

    Component.onCompleted:
    {
        screenLog.getLogs(activeAccount);
        screenPhone.setMiss_notify(false);
    }

    Text
    {
        text: "Call history is empty"
        color: "white"
        visible: logListView.count <= 0
        anchors.centerIn: parent
        font.pixelSize: 30
    }

    Row
    {
        height: app_height - contentStart
        anchors
        {
            top: parent.top
            topMargin: contentStart
            left: parent.left
            leftMargin: 1
        }

        Item
        {
            width: 320
            height: parent.height

            ListView
            {
                id: logListView
                anchors.fill: parent
                clip: true
                focus: true
                model: logModel
                interactive: true
                delegate: listViewRow;
                visible: logListView.count > 0

                ScrollBar
                {
                    target: parent
                }
            }
        }

        Item
        {
            width: app_width - 320
            height: parent.height
            visible: m_type !== ""

            Column
            {
                width: parent.width
                height: parent.height

                Rectangle
                {
                    width: parent.width
                    height: 155
                    color: "transparent"

                    Image
                    {
                        fillMode: Image.PreserveAspectCrop
                        smooth: true
                        source: (m_pic_path==="") ? "../../assets/buddy.png" : m_pic_path
                        anchors.fill: parent

                        Rectangle
                        {
                            anchors.top: parent.top
                            width: parent.width
                            height: 37
                            color: "black"
                            opacity: 0.5

                        }

                        Text
                        {
                            id: txtActiveNum
                            height: 37
                            verticalAlignment: Text.AlignVCenter
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            font.family: mainScreen.m_fontMain.name
                            font.bold: true
                            color: "white"
                            font.pointSize: 13
                            elide: Text.ElideRight
                        }

                        Button
                        {
                            m_image: "../../assets/add_contact.png"
                            m_image_over: "../../assets/add_contact.png"
                            width: 22
                            height: 22
                            tooltip_text: "Add this number to contacts"
                            x: 322
                            y: 7
                            visible: !is_contact
                            onButtonClicked:
                            {
                                screenContacts.setNewNumber("Number", m_to_num, txtActiveNum.text);
                                Controller.setQml("screens/contacts.qml", "contacts");
                            }
                        }

                        Button
                        {
                            m_image: "../../assets/tab_phone.png"
                            m_image_over: "../../assets/tab_phone.png"
                            width: 22
                            height: 22
                            tooltip_text: "Call to this number"
                            x: 353
                            y: 7
                            onButtonClicked:
                            {
                                screenPhone.setDialText(m_to_num);
                                Controller.setQml("screens/phone.qml", "phone");
                            }
                        }
                    }
                }

                Rectangle
                {
                    width: parent.width
                    color: "transparent"
                    height: 135

                    Image
                    {
                        x: 320
                        y: 75
                        source: if(m_type=="IN") { "../../assets/call_incoming.png" } else if(m_type=="OUT") { "../../assets/call_dialed.png" } else if(m_type=="MISS") { "../../assets/call_missed.png" } else { "../../assets/call_failed.png" }
                    }

                    Column
                    {
                        width: parent.width
                        height: 135
                        spacing: 5
                        anchors
                        {
                            left: parent.left
                            top: parent.top
                        }

                        Rectangle
                        {
                            height: 28
                            width: parent.width
                            color: "#3A3C3A"
                            Text
                            {
                                visible: m_duration!==""
                                height: parent.height
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 13
                                text: "Call Details"
                                color: "#FF6600"
                                font.family: mainScreen.m_fontMain.name
                                font.bold: true
                                x: 10
                            }
                            Rectangle
                            {
                                width: parent.width
                                height: 1
                                color: "#4B4C4B";
                                anchors.top: parent.top
                            }

                            Rectangle
                            {
                                width: parent.width
                                height: 1
                                color: "#424442";
                                anchors.bottom: parent.bottom
                            }
                        }

                        Text
                        {
                            x:10
                            text: formatDate(m_moment);
                            font.pixelSize: 15
                            font.bold: true
                            color: "white"
                            height: 25
                            verticalAlignment: Text.AlignBottom
                        }

                        Text
                        {
                            x: 30
                            font.pixelSize: 14
                            text: (m_type === "OUT" ? "Outgoing call" : m_type === "IN" ? "Incoming call" : m_type === "MISS" ? "Missed Incoming call" : m_type === "FAIL" ? "Outgoing not answered" : "Unknown") +  " " + secToTime(m_duration)
                            color: "#CCC" //if(m_type=="IN") {"cyan"} else if(m_type=="OUT") { "lightgreen" } else if(m_type=="MISS") { "red" } else { "orange" }
                        }

                        Text
                        {
                            x: 30
                            font.pixelSize: 11
                            text: "Voice Downloaded: " + (parseInt(m_rx)/1024).toFixed(2) + " KBytes"
                            color: "#CCC"
                        }

                        Text
                        {
                            x: 30
                            font.pixelSize: 11
                            text: "Voice Uploaded : " + (parseInt(m_tx)/1024).toFixed(2) + " KBytes"
                            color: "#CCC"
                        }

                        Text
                        {
                            x: 30
                            font.pixelSize: 11
                            text: "Local Voice Loss: " + (parseInt(m_local_loss)).toFixed(1)
                            color: "#CCC"
                        }

                        Text
                        {
                            x: 30
                            font.pixelSize: 11
                            text: "Remote Voice loss : " + (parseInt(m_remote_loss)).toFixed(1)
                            color: "#CCC"
                        }

                        Text
                        {
                            x: 30
                            font.pixelSize: 11
                            text: "Codec: " + m_codec
                            color: "#CCC"
                        }

                        Text
                        {
                            x: 30
                            font.pixelSize: 11
                            text: "Othos Call Cost: " + m_cost
                            color: "#CCC"
                        }

                        Text
                        {
                            x: 30
                            font.pixelSize: 12
                            height: 10
                            text: " "
                            color: "#CCC"
                        }
                    }
                }
            }
        }
    }

    Component
    {
        id: listViewRow

        Item
        {
            id:listViewRowWrapper
            width: logListView.width;
            height: 70

            Rectangle
            {
                gradient: (log_call_id===selectedLog) ? selectedItem : (mouseArea.containsMouse) ? overItem : normalItem
                anchors.fill: parent

                Rectangle
                {
                    width: parent.width
                    height: 1
                    color: "#3c3c3c"
                    anchors.bottom: parent.bottom
                }

                MouseArea
                {
                    id: mouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: "PointingHandCursor"
                    onClicked:
                    {
                        selectedLog = log_call_id
                        txtActiveNum.text = name;
                        m_to_num = to_num
                        m_description = description
                        m_pic_path = pic_path
                        logListView.currentIndex = index;
                        m_duration = duration;
                        m_moment = moment;
                        m_type = log_type;
                        m_log_id = log_call_id
                        m_rx = rx
                        m_tx = tx
                        m_cost = cost
                        m_codec = codec
                        m_local_loss = local_loss
                        m_remote_loss = remote_loss
                        is_contact = (description !== 'Unknown')
                    }
                }

                Row
                {
                    anchors
                    {
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                        leftMargin: 17
                    }

                    spacing: 10

                    Image
                    {
                        source: (pic_path==="") ? "../../assets/buddy.png" : pic_path
                        fillMode: Image.PreserveAspectFit
                        smooth: true
                        width: 60
                        height: 60
                    }

                    Column
                    {
                        spacing: 8
                        y:5
                        Text
                        {
                            text: name
                            color: "white"
                            font.pixelSize: 18
                            font.bold: true
                            width: 175
                            elide: Text.ElideRight
                        }

                        Text
                        {
                            text: description + " " + to_num
                            color: "white"
                            font.pixelSize: 16
                            width: 175
                            elide: Text.ElideRight
                        }
                    }

                    Image
                    {
                        anchors.verticalCenter: parent.verticalCenter
                        source: if(log_type=="IN") { "../../assets/call_incoming.png" } else if(log_type=="OUT") { "../../assets/call_dialed.png" } else if(log_type=="MISS") { "../../assets/call_missed.png" } else { "../../assets/call_failed.png" }
                    }
                }
            }
        }
    }

    Gradient
    {
        id: selectedItem
        GradientStop {position: 0; color: "#FF7800"}
        GradientStop {position: 1; color: "#FF5800"}
    }

    Gradient
    {
        id: normalItem
        GradientStop { position: 1; color: "#303030" }
        GradientStop { position: 0; color: "#303030" }
    }

    Gradient
    {
        id: overItem
        GradientStop {position: 0; color: "#444"}
        GradientStop {position: 1; color: "#222"}
    }

    function secToTime(secs)
    {
        if(secs.indexOf(":")<=0)
        {
            if(secs<0) secs = 0;
            var sec_numb = parseInt(secs);
            var hours   = Math.floor(sec_numb / 3600);
            var minutes = Math.floor((sec_numb - (hours * 3600)) / 60);
            var seconds = sec_numb - (hours * 3600) - (minutes * 60);

            if (hours   < 10) {hours   = "0"+hours;}
            if (minutes < 10) {minutes = "0"+minutes;}
            if (seconds < 10) {seconds = "0"+seconds;}
            var tim    = hours+':'+minutes+':'+seconds;
            return tim;
        }
        else
        {
            var hour = secs.substr(0,2);
            var min = secs.substr(3,2);
            var sec = secs.substr(6,2);
            var time = hour +' h '+ min +' min '+ sec + " sec";
            return time;
        }
    }

    function formatDate(dt)
    {
        var d1 = new Date(dt);
        return d1.toLocaleTimeString() + ", " + d1.toLocaleDateString();
    }
}
