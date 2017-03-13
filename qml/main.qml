import QtQuick 2.0
import QtQuick.Controls 2.0
import Item 1.0
AnimatedImage {
    id:root
    width:1024
    height:768
    source:"qrc:/bg.gif"
    Item{
        id:item
        width:512
        height:384
        anchors {
            verticalCenter: parent.verticalCenter
            left:parent.left
        }

        Text {
            anchors {
                left:parent.left
                top:parent.top
                margins: 5
            }
            text:"FPS: " + item.fps.toFixed(1)
            color:"green"
        }
    }
    Button {
        id:quitBtn
        anchors{
            bottom:parent.bottom
            right:parent.right
            margins: 5
        }
        text:"Quit"
        onClicked: Qt.quit()
    }
    Column {
        anchors {
            left:parent.left
            bottom:parent.bottom
        }
        Row{
            Slider {id:xRotate; from:0; to:360;onValueChanged: item.xRotate = value.toFixed(0)}
            Text{text:"X alias:" + xRotate.value.toFixed(0)}
        }
        Row{
            Slider {id:yRotate; from:0; to:360;onValueChanged: item.yRotate = value.toFixed(0)}
            Text{text:"Y alias:" + yRotate.value.toFixed(0)}
        }
        Row{
            Slider {id:zRotate; from:0; to:360;onValueChanged: item.zRotate = value.toFixed(0)}
            Text{text:"Z alias:" + zRotate.value.toFixed(0)}
        }

    }
}
