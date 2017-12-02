#ifndef RENDER_H
#define RENDER_H

#include "glfunc.h"

#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLDebugLogger>
#include <QOpenGLDebugMessage>
#include <QObject>
#include <QTime>
#include "model/Model.h"
#include "Camera.h"
class Render :  protected GLFUNC
{
public:
    Render(JCamera *camera, Model *model);
    Render(const Render & other) = delete;
    Render & operator=(const Render & other) = delete;
    void Init(const QSize &size);
    void Paint();
private:
    void initShader();
    void initMatrixs();
    void initVertices();

private:
    Model *m_model = nullptr;
    JCamera *m_camera = nullptr;

    QOpenGLShaderProgram m_program;
    QMatrix4x4 m_ModelMatrix, m_mvpMatrix;
    QTime m_time;
    QSize m_size;
#ifdef USE_GL_DEBUGGER
    QOpenGLDebugLogger logger;
#endif
};

#endif // RENDER_H
