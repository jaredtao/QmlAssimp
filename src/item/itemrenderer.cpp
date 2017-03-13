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
    //cast fbo to Item
    Item *item =qobject_cast<Item*>(fbo);
    //here can synchronize info between item and render.
    item->setFps(m_render.GetFPS());
    if(!item->m_json.isEmpty()) {
        m_render.SetParams(item->m_json);
        item->m_json = QJsonObject();
    }
}
