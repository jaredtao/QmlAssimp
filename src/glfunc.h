#pragma once

//Qt封装的OpenGL functions 统一在这个头文件包含
//修改OpenGL 版本的时候，只要修改这里

#ifdef Q_PROCESSOR_ARM
#include <QOpenGLExtraFunctions>
#define GLFUNC QOpenGLExtraFunctions
#else
#include <QOpenGLFunctions_3_3_Core>
#define GLFUNC QOpenGLFunctions_3_3_Core
#endif

