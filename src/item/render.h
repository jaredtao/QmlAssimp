#ifndef RENDER_H
#define RENDER_H

#include "glfunc.h"

#include <QOpenGLShader>
#include<QOpenGLShaderProgram>
#include <QOpenGLDebugLogger>
#include <QOpenGLDebugMessage>

class Render : protected GLFUNC
{
public:
    Render();
    void Init();
    void Paint();
    qreal GetFPS();
private:
    void initShader();
    void initMatrixs();
    void initVertices();

    void calcFPS();
    void updateFPS(qreal );
private:
    QOpenGLShaderProgram m_program;

    QMatrix4x4 mModelMatrix;
    QMatrix4x4 mViewMatrix;
    QMatrix4x4 mProjectionMatrix;

    qreal fps;
#ifdef USE_GL_DEBUGGER
    QOpenGLDebugLogger logger;
#endif
};

#endif // RENDER_H
