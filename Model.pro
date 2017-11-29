#-------------------------------------------------
#
# Project created by QtCreator 2017-03-11T19:12:11
#
#-------------------------------------------------

QT       += core gui quick qml
CONFIG += C+=11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Model
TEMPLATE = app

ASSIMPPATH = $$PWD/assimp-4.0.0
INCLUDEPATH +=$${ASSIMPPATH}/include
DEPENDPATH  +=$${ASSIMPPATH}/include
win32{
    CONFIG(debug, debug|release) {
        LIBS += -L$${ASSIMPPATH}/lib32 -lassimp-vc140-mtd
    } else {
        LIBS += -L$${ASSIMPPATH}/lib32 -lassimp-vc140-mt
    }
}

linux{
    LIBS += -lassimp -lGL
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


#use QOpenGLDebuggerLogger to debug OpenGL
#DEFINES += USE_GL_DEBUGGER

INCLUDEPATH += src
SOURCES += src/main.cpp \
    src/model/model.cpp \
    src/model/mesh.cpp \
    src/item/FBOItem.cpp \
    src/item/Render.cpp \
    src/item/Camera.cpp

HEADERS += \
    src/glfunc.h \
    src/model/model.h \
    src/model/mesh.h \
    src/item/FBOItem.h \
    src/item/Render.h \
    src/item/Camera.h

RESOURCES += \
    qml/qml.qrc \
    img/img.qrc \
    glsl/glsl.qrc

#自动拷贝 文件夹到运行目录
#CONFIG += autoDeploymentDir autoDeploymentFiles
#这里写到最后一级文件夹
#deployDirs = $$PWD/model/nanosuit

#自动拷贝 文件到运行目��#CONFIG += autoDeploymentFiles
#deployFiles = $$PWD/model/nanosuit/nanosuit.obj
#deployFiles = $$PWD/model/t700.fbx
#include (deployment.pri)

DESTDIR = $$PWD/bin

# 便于构建用的一些属性
theList = $$split( _PRO_FILE_, / )
projectPrefix = $$member( theList, -2 )

UI_DIR = $$PWD/bin/build/$$projectPrefix
MOC_DIR = $$PWD/bin/build/$$projectPrefix
RCC_DIR = $$PWD/bin/build/$$projectPrefix
OBJECTS_DIR = $$PWD/bin/build/$$projectPrefix
