import QtQuick 2.0

Rectangle
{
    id: container
    property alias model: lstItems.model
    property string modelIdField
    property string modelDataField

    property string textColor
    property int textSize

    property string defaultValue
    property int defaultIndex

    property string selectedData
    property string selectedId

    radius: 10

    Row
    {
        anchors.fill: parent
        ListView
        {
            id: lstItems
            width: parent.width - 20
            clip: true
            height: parent.height - 6
            interactive: false

            delegate: Item
            {
                property variant currentData: (modelDataField!=="") ? model[modelDataField] : modelData
                property variant currentId: (modelIdField!=="") ? model[modelIdField] : modelData


                width: lstItems.width - 10
                height: lstItems.height
                Text
                {
                    id: txtItem
                    anchors.fill: parent
                    elide: Text.ElideRight
                    font.pixelSize: textSize
                    color: textColor
                    text: (modelDataField!=="") ? model[modelDataField] : modelData
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    anchors.leftMargin: 10

                    Component.onCompleted:
                    {
                        console.debug("looking for default index: " + index +" if "+ currentData +" = "+ defaultValue)

                        if(modelIdField!=="")
                        {
                            if((defaultValue.toString() == currentData) || (defaultValue.toString() == currentId))
                            {
                                console.debug("found default index " + index )
                                defaultIndex = index
                                lstItems.currentIndex = defaultIndex
                            }
                        }
                        else
                        {
                            if(defaultValue.toString() == modelData)
                            {
                                console.debug("found default index " + index )
                                defaultIndex = index
                                lstItems.currentIndex = defaultIndex
                            }
                        }
                    }
                }
            }

            onCurrentIndexChanged:
            {

                selectedData = currentItem.currentData;
                selectedId = currentItem.currentId;
                console.debug("currentData " + selectedId);
            }

            Component.onCompleted:
            {
                lstItems.currentIndex = defaultIndex
            }
        }
        Column
        {
            width: 15
            height: parent.height
            spacing: container.height / 6
            y: 5

            Image
            {
                id: previous
                width: 15
                source: "../../assets/spinbox_up.png"
                height: parent.height /3.5
                MouseArea
                {
                    id: previousMA
                    anchors.fill: parent
                    cursorShape: "PointingHandCursor"
                    hoverEnabled: true
                    onClicked:
                    {
                        lstItems.decrementCurrentIndex();
                    }
                }
            }

            Image
            {
                id: next
                width: 15
                source: "../../assets/spinbox_down.png"
                height: parent.height / 3.5
                MouseArea
                {
                    id: nextMA
                    anchors.fill: parent
                    cursorShape: "PointingHandCursor"
                    hoverEnabled: true
                    onClicked:
                    {
                        lstItems.incrementCurrentIndex();
                    }
                }
            }
        }
    }
}
