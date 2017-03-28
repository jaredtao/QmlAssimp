#include "render.h"
#include <QTime>
#include <QDebug>
#include <QJsonObject>
#include <QThread>
Render::Render()
{
    qDebug() << "Render construct" << this;
    m_fps = 60;
    m_xRotate = 0;
    m_yRotate = 0;
    m_zRotate = 0;
}

void Render::Init(QSize size)
{
    QTime time;
    time.start();
    qDebug() << "time start" << QThread::currentThreadId() << this;
    m_size = size;
    initializeOpenGLFunctions();
#ifdef USE_GL_DEBUGGER
    logger.initialize();
    QObject::connect(&logger, &QOpenGLDebugLogger::messageLogged, [=](const QOpenGLDebugMessage &msg){
        qDebug() << msg;
    });
    logger.startLogging();
#endif
    glViewport(0, 0, size.width(), size.height());
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    initShader();
    initMatrixs();
    initVertices();
    auto cost1 = time.elapsed();
    qDebug() << "init OpenGL datas cost time :" << cost1;
    m_model.Init("nanosuit/nanosuit.obj");
    auto cost2 = time.elapsed() - cost1;
    qDebug() << "init Model cost time:" << cost2;
}

void Render::Paint()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    m_program.bind();
    m_program.setUniformValue("model", mModelMatrix);
    m_program.setUniformValue("view", mViewMatrix);
    m_program.setUniformValue("projection", mProjectionMatrix);

    m_model.Draw(m_program);
    calcFPS();
}

qreal Render::GetFPS()
{
    return m_fps;
}

void Render::SetParams(const QJsonObject & params)
{
    for (auto i : params.keys()) {
        if (i == "xRotate") {
            xRotate(params[i].toDouble());
        } else if (i == "yRotate") {
            yRotate(params[i].toDouble());
        } else if (i == "zRotate") {
            zRotate(params[i].toDouble());
        } else {
            qDebug() << "not support command:" << i;
        }
    }
}

void Render::initShader()
{
    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex.vsh")) {
        qDebug() << " add vertex shader failed:" << m_program.log();
        return ;
    }
    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment.fsh")) {
        qDebug() << "add fragment shader failed:" << m_program.log();
    }
    m_program.link();
    m_program.bind();
}

void Render::initMatrixs()
{
    mModelMatrix.setToIdentity();
    mModelMatrix.translate(0.0f, -1.75f, 0.0f);
    mModelMatrix.scale(0.2f, 0.2f, 0.2f);

    mProjectionMatrix.setToIdentity();
    mProjectionMatrix.perspective(45, m_size.width() / (m_size.height() ? m_size.height() : 1), 0.1f , 100.0f);

    mViewMatrix.setToIdentity();
    mViewMatrix.lookAt(QVector3D(0.0, 0.0, 1.0), QVector3D(0.0, 0.0, 0.0), QVector3D(0.0, 1.0, 0.0));
    //    mViewMatrix.lookAt(QVector3D(0.0f, 0.0f, 1.001f), QVector3D(0.0f, 0.0f, -5.0f), QVector3D(0.0f, 1.0f, 0.0f));
}

void Render::initVertices()
{

}

void Render::xRotate(qreal v)
{
    m_xRotate = v;
    updateRotate();
}

void Render::yRotate(qreal v)
{
    m_yRotate = v;
    updateRotate();
}

void Render::zRotate(qreal v)
{
    m_zRotate = v;
    updateRotate();
}

void Render::updateRotate()
{
    mModelMatrix.setToIdentity();
    mModelMatrix.rotate(QQuaternion(m_xRotate, QVector3D(1, 0, 0)));
    mModelMatrix.rotate(QQuaternion(m_yRotate, QVector3D(0, 1, 0)));
    mModelMatrix.rotate(QQuaternion(m_zRotate, QVector3D(0, 0, 1)));
}
void Render::calcFPS()
{
    static QTime time;
    static int once = [=](){time.start(); return 0;}();
    Q_UNUSED(once)
    static int frame = 0;
    frame++;
    if (frame > 10) {
        qreal elasped = time.elapsed();
        updateFPS(frame * 1000.0/ elasped );
        time.restart();
        frame = 0;
    }
}
void Render::updateFPS(qreal v)
{
    m_fps = v;
}
