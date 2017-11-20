#ifndef RENDER_H
#define RENDER_H

#include "glfunc.h"

#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLDebugLogger>
#include <QOpenGLDebugMessage>
#include <QObject>
#include <QTime>
#include "model/model.h"
#include "Camera.h"
class Render :  protected GLFUNC
{
public:
    Render();
    Render(const Render & other) = delete;
    Render & operator=(const Render & other) = delete;
    void Init(const QSize &size);
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
    Model m_model;
    JCamera m_camera;
    QMatrix4x4 m_ModelMatrix, m_mvpMatrix;
    QTime m_time;
    QSize m_size;
    qreal m_fps;
#ifdef USE_GL_DEBUGGER
    QOpenGLDebugLogger logger;
#endif
};

#endif // RENDER_H
