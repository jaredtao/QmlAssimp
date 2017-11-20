#include "Render.h"

#include <QDebug>
#include <QJsonObject>
#include <QThread>
Render::Render()
{
    qDebug() << "Render construct" << this;
    m_fps = 60;
}

void Render::Init(const QSize &size)
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
    m_time.start();
}

void Render::Paint()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    m_program.bind();
    m_mvpMatrix = m_camera.projectMatrix() * m_camera.viewMatrix() * m_ModelMatrix;
    m_program.setUniformValue("mvpMat", m_mvpMatrix);
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
    m_ModelMatrix.setToIdentity();
//    m_ModelMatrix.translate(0.0f, 1.0f, 0.0f);
    m_ModelMatrix.scale(0.4f, 0.4f, 0.4f);

    m_camera.setFarPlane(100);
    m_camera.sync();
}

void Render::initVertices()
{

}

void Render::calcFPS()
{
    static int frame = 0;
    frame++;
    if (frame > 10) {
        qreal elasped = m_time.elapsed();
        updateFPS(frame * 1000.0/ elasped );
        m_time.restart();
        frame = 0;
    }
}
void Render::updateFPS(qreal v)
{
    m_fps = v;
    qWarning() << "FPS: " << m_fps;
}
