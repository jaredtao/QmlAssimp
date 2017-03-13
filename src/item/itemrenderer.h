#ifndef RENDERER_H
#define RENDERER_H

#include "render.h"
#include <QQuickFramebufferObject>
class ItemRenderer : public QQuickFramebufferObject::Renderer
{
public:
    ItemRenderer();

    // Renderer interface
protected:
    void render() override;
    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override;
    void synchronize(QQuickFramebufferObject *) override;

    Render m_render;
};

#endif // RENDERER_H
