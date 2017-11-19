import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Window 2.0
import FBOItem 1.0
Window{
    id:root
    width:1024
    height:768
    x: (Screen.desktopAvailableWidth - width) / 2
    y: (Screen.desktopAvailableHeight - height) / 2
    visible: true

    Image {
        anchors.fill: parent
        source:"qrc:/bg.jpg"
        FBOItem {
            id: fboItem
            anchors.fill: parent
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
    Item {
        property int fps: 0
        property int frameCount: 0

        NumberAnimation on rotation {
            from: 0
            to: 360
            duration: 1000;
            loops: Animation.Infinite
        }
        onRotationChanged: ++frameCount
        Timer {
            interval: 1000
            repeat: true
            running: true
            onTriggered: {
                parent.fps = parent.frameCount
                parent.frameCount = 0;
                console.log("fps: ", parent.fps)
            }
        }
    }
}
