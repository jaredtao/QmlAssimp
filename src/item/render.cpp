#include "render.h"
#include <QTime>
#include <QDebug>
Render::Render(QObject *parent)
    : QObject(parent)
{
    m_fps = 60;
}

void Render::Init(QSize size)
{
    m_size = size;
    initializeOpenGLFunctions();
#ifdef USE_GL_DEBUGGER
    logger.initialize();
    connect(&logger, &QOpenGLDebugLogger::messageLogged, [=](const QOpenGLDebugMessage &msg){
        qDebug() << msg;
    });
    logger.startLogging();
#endif

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    m_model.Init("model/nanosuit/nanosuit.obj");
    initShader();
    initMatrixs();
    initVertices();
}

void Render::Paint()
{
    glClearColor(0.0f, 0.1f, 0.0f, 1.0f);
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
//    mViewMatrix.lookAt(QVector3D(0.0, 0.0, 1.0), QVector3D(0.0, 0.0, 0.0), QVector3D(0.0, 1.0, 0.0));
    mViewMatrix.lookAt(QVector3D(0.0f, 0.0f, 1.001f), QVector3D(0.0f, 0.0f, -5.0f), QVector3D(0.0f, 1.0f, 0.0f));
}

void Render::initVertices()
{

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
