import QtQuick 2.0

Rectangle {
    id: container

    //signal m_valueChanged;

    property string m_slider_color1: "#777";
    property string m_slider_color2: "#444";
    property string m_data;
    property alias m_slider: slider;
    property real m_value

    color: "transparent";

    Rectangle {
        id: line
        x: 7
        y: 10;
        width: 1
        height: container.height - y*2;
        z: -1
        border.width: 3
        border.color: m_slider_color2
    }


    MouseArea
    {
        cursorShape: "PointingHandCursor"
        anchors.fill: parent
        onClicked: container.moveSlider(mouse.y)
    }

    function moveSlider(y)
    {
        var sheight = slider.height/2

        if(y <= sheight)
        {
            slider.y = 0;
        }
        else if(y >= container.height - sheight)
        {
            slider.y = container.height - slider.height;
        }
        else
        {
            slider.y = y - sheight;
        }
    }

    radius: 8; smooth: true

    Rectangle
    {
        id: slider

        anchors.horizontalCenter: parent.horizontalCenter

        Behavior on y
        {
            SequentialAnimation {
                NumberAnimation {
                    easing.type: "OutBack"
                    duration: 300
                }
            }
        }

        y: m_value * (container.height - slider.height)
        x: 2
        width: 12;
        height: 22;
        radius: 6;
        smooth: true
        border.color: m_slider_color1
        border.width: 2;

        gradient: Gradient {
            GradientStop { position: 0.0; color: (sliderMouse.containsMouse) ? m_slider_color1 : m_slider_color2 }
            GradientStop { position: 1.0; color: (sliderMouse.containsMouse) ? m_slider_color2 : m_slider_color1 }
        }

        MouseArea
        {
            id: sliderMouse
            anchors.fill: parent
            drag.target: parent;
            cursorShape: "PointingHandCursor"
            drag.axis: "YAxis";
            drag.minimumY: 2;
            drag.maximumY: container.height - slider.height - 2;
            hoverEnabled: true;
        }

        onYChanged:
        {
            if (slider.y < 0)
            {
                m_value = 1;
            }
            else
            {
                m_value = slider.y / (container.height - slider.height);
                container.m_valueChanged((m_value).toFixed(2));
            }
            if(m_value < 0) m_value=0
            screenPhone.setVolume((m_value).toFixed(2), m_data);
        }
    }
}
