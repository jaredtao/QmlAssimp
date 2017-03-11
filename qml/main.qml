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
        width:860
        height:640
        anchors.centerIn: parent

        Text {
            anchors {
                left:parent.left
                top:parent.top
                margins: 5
            }
            text:"FPS: " + item.fps.toFixed(1)
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
}
