import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Window 2.0
import J3D 1.0
Item {
    id:root
    width:1024
    height:768
    Image {
        anchors.fill: parent
        source:"qrc:/bg.jpg"

    }

    FBOItem {
        id: fboItem
        width: 800
        height: 600
        anchors.centerIn: parent
        focus: true
        Keys.enabled: true
        Keys.onPressed: {
            camera.move(event.key)
        }
        JKeyCamera {
            id: camera
            position: Qt.vector3d(0, 8, 22)
            lookAt: Qt.vector3d(0, 8, 0)
            up: Qt.vector3d(0, -1, 0)
            aspectRatio: width / height
            nearPlane: 0.1
            farPlane: 1000
            fieldOfView: 45
            speed: 0.01
        }
        Model {
            source: "nanosuit/nanosuit.obj"
        }
    }

    Row {
        anchors{
            bottom:parent.bottom
            right:parent.right
            margins: 5
        }
        spacing: 4
        Button {
            id:quitBtn

            text:"Quit"
            onClicked: Qt.quit()
        }
    }

    Item {
        id: fpsItem
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
            }
        }

    }
    Text {
        text: "FPS:" + fpsItem.fps
        color: "red"
        font.pixelSize: 20
        font.bold: true
    }
}
