#include "render.h"
#include <QTime>
#include <QDebug>
Render::Render()
{
    fps = 60;
}

void Render::Init()
{
    initializeOpenGLFunctions();
#ifdef USE_GL_DEBUGGER
    logger.initialize();
    connect(&logger, &QOpenGLDebugLogger::messageLogged, [=](const QOpenGLDebugMessage &msg){
        qDebug() << msg;
    });
    logger.startLogging();
#endif

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
}

void Render::Paint()
{
    glClearColor(0.0f, 0.1f, 0.0f, 1.0f);

    calcFPS();
}

qreal Render::GetFPS()
{
    return fps;
}

void Render::initShader()
{
    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, "qrc:/vertex.vsh")) {


    }
}

void Render::initMatrixs()
{

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
    fps = v;
}
