#include "itemrenderer.h"
#include "item.h"
#include <QThread>
#include <QOpenGLFramebufferObjectFormat>
ItemRenderer::ItemRenderer()
    : m_render(0)
{
}

ItemRenderer::~ItemRenderer()
{
    if (m_render) {
        delete m_render;
        m_render = nullptr;
    }
}

void ItemRenderer::render()
{
    if (m_render) {
        m_render->Paint();
    }
}

QOpenGLFramebufferObject *ItemRenderer::createFramebufferObject(const QSize &size)
{
    qDebug() << "create FBO " << QThread::currentThread() << this;
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(4);
    if (m_render) {
        delete m_render;
    }
    m_render = new Render;
    m_render->Init(size);
    return new QOpenGLFramebufferObject(size, format);
}

void ItemRenderer::synchronize(QQuickFramebufferObject *fbo)
{
    if (m_render) {
        //cast fbo to Item
        Item *item =qobject_cast<Item*>(fbo);
        //here can synchronize info between item and render.
        item->setFps(m_render->GetFPS());
        auto params = item->GetParams();
        if(!params.isEmpty()) {
            m_render->SetParams(params);
            item->ClearParams();
        }
    }
}
