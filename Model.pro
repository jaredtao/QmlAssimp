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
} else {
    CONFIG += link_pkgconfig
    PKGCONFIG += assimp
}

#use QOpenGLDebuggerLogger to debug OpenGL
#DEFINES += USE_GL_DEBUGGER

# show time cost
#DEFINES += SHOW_TIME_COST

# show assimp info
DEFINES += SHOW_ASSIMP_INFO

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
    src/item/Camera.h \
    src/stb_image.h

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

DISTFILES += \
    glsl/lamp.vsh \
    glsl/fragment.fsh \
    glsl/lamp.fsh
lupdate_only {
    HEADERS +=$${ASSIMPPATH}/include/assimp/*.h
}
