#ifndef RENDER_H
#define RENDER_H

#include "glfunc.h"

#include <QOpenGLShader>
#include<QOpenGLShaderProgram>
#include <QOpenGLDebugLogger>
#include <QOpenGLDebugMessage>
#include <QObject>

#include "model/model.h"
class Render : public QObject,  protected GLFUNC
{
    Q_OBJECT
public:
    Render(QObject *parent = nullptr);
    ~Render();
    void Init(QSize);
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
    Model *m_model;
    QMatrix4x4 mModelMatrix;
    QMatrix4x4 mViewMatrix;
    QMatrix4x4 mProjectionMatrix;

    QSize m_size;
    qreal m_fps;
#ifdef USE_GL_DEBUGGER
    QOpenGLDebugLogger logger;
#endif
};

#endif // RENDER_H
