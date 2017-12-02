import QtQuick 2.0
import QtQuick.Controls 1.4
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
            camera.keyPress(event.key)
            event.accepted = true
        }
        Keys.onReleased: {
            camera.keyRelease(event.key)
            event.accepted = true
        }

        JKeyCamera {
            id: camera
            position: Qt.vector3d(0, 0, 3.0)
            front: Qt.vector3d(0, 0, -1)
            up: Qt.vector3d(0, -1, 0)
            aspectRatio: width / height
            nearPlane: 0.1
            farPlane: 100
            fieldOfView: 45
            speed: 1
        }
        Model {
            source: "nanosuit/nanosuit.obj"
        }
    }
    MouseArea {
        anchors.fill: fboItem
        onPressed: {
            camera.mousePress(mouseX, mouseY)
        }
        onPositionChanged: {
            camera.mouseMove(mouseX, mouseY)
        }
        onWheel: {
            camera.wheel(wheel.angleDelta)
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
