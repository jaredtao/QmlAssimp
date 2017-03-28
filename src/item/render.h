#ifndef RENDER_H
#define RENDER_H

#include "glfunc.h"

#include <QOpenGLShader>
#include<QOpenGLShaderProgram>
#include <QOpenGLDebugLogger>
#include <QOpenGLDebugMessage>
#include <QObject>

#include "model/model.h"
class Render :  protected GLFUNC
{
public:
    Render();
    Render(const Render & other) = delete;
    Render & operator=(const Render & other) = delete;
    void Init(QSize);
    void Paint();
    qreal GetFPS();
    void SetParams(const QJsonObject &params );
private:
    void initShader();
    void initMatrixs();
    void initVertices();

    void xRotate(qreal );
    void yRotate(qreal );
    void zRotate(qreal );
    void updateRotate();

    void calcFPS();
    void updateFPS(qreal );
private:
    QOpenGLShaderProgram m_program;
    Model m_model;
    QMatrix4x4 mModelMatrix;
    QMatrix4x4 mViewMatrix;
    QMatrix4x4 mProjectionMatrix;

    QSize m_size;
    qreal m_fps;
    qreal m_xRotate;
    qreal m_yRotate;
    qreal m_zRotate;
#ifdef USE_GL_DEBUGGER
    QOpenGLDebugLogger logger;
#endif
};

#endif // RENDER_H
