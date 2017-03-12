#include "itemrenderer.h"
#include "item.h"
#include <QOpenGLFramebufferObjectFormat>
ItemRenderer::ItemRenderer()
{
}


void ItemRenderer::render()
{
    m_render.Paint();
}

QOpenGLFramebufferObject *ItemRenderer::createFramebufferObject(const QSize &size)
{
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(4);
    m_render.Init(size);
    return new QOpenGLFramebufferObject(size, format);
}

void ItemRenderer::synchronize(QQuickFramebufferObject *fbo)
{
    Item *item =qobject_cast<Item*>(fbo);
    //here can synchronize info between item and render.
    item->setFps(m_render.GetFPS());
}
