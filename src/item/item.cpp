#include "item.h"
#include "itemrenderer.h"


Item::Item(QQuickItem *parent): QQuickFramebufferObject(parent)
{
    m_fps = 60;
}

void Item::timerEvent(QTimerEvent *)
{
    update();
}


void Item::classBegin()
{
}

void Item::componentComplete()
{
    startTimer(1);
}

QQuickFramebufferObject::Renderer *Item::createRenderer() const
{
    return new ItemRenderer;
}
qreal Item::fps() const
{
    return m_fps;
}

void Item::setFps(qreal v)
{
    if (v != m_fps) {
        m_fps = v;
        emit fpsChanged();
    }
}
