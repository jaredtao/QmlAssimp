#include "Render.h"

#include <QDebug>
#include <QJsonObject>
#include <QThread>
Render::Render(JCamera *camera, Model *model) : m_model(model), m_camera(camera)
{
}
void Render::Init(const QSize &size)
{
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
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    initShader();
    initMatrixs();
    initVertices();
    if (m_model) {
        m_model->Init();
    }
    m_time.start();
}

void Render::Paint()
{
    const auto deltaTime = m_time.elapsed();
    m_time.start();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    m_program.bind();
    if (m_camera) {
        m_camera->sync(deltaTime);
        m_mvpMatrix = m_camera->projectMatrix() * m_camera->viewMatrix() * m_ModelMatrix;
        m_program.setUniformValue("mvpMat", m_mvpMatrix);
    }
    if (m_model) {
        m_model->Draw(m_program);
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
    m_ModelMatrix.setToIdentity();
	m_ModelMatrix.translate(0.0f, -1.75f, 0.0f);
	m_ModelMatrix.scale(0.2f, 0.2f, 0.2f);
    if (m_camera) {
        m_camera->sync();
    }
}

void Render::initVertices()
{
}
