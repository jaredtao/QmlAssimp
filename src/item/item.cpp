#include "item.h"
#include "itemrenderer.h"

#define ARRAYLEN(arr) (sizeof(arr)/sizeof(arr[0]))

class ItemPrivate{
public:
    ItemPrivate() {
        fps = 60;
        xRotate = 0;
        yRotate = 0;
        zRotate = 0;
    }
    qreal xRotate;
    qreal yRotate;
    qreal zRotate;
    qreal fps;
};

Item::Item(QQuickItem *parent): QQuickFramebufferObject(parent)
  , m_dptr(new ItemPrivate)
{
    setObjectName("Item");
    QMetaObject::connectSlotsByName(this);
}

Item::~Item()
{
    delete m_dptr;
}
void Item::classBegin()
{
}

void Item::componentComplete()
{
    startTimer(1);
}

void Item::timerEvent(QTimerEvent *)
{
    update();
}

void Item::keyPressEvent(QKeyEvent *event)
{

}

void Item::keyReleaseEvent(QKeyEvent *event)
{

}

void Item::mousePressEvent(QMouseEvent *event)
{

}

void Item::mouseMoveEvent(QMouseEvent *event)
{

}

void Item::wheelEvent(QWheelEvent *event)
{

}

QQuickFramebufferObject::Renderer *Item::createRenderer() const
{
    return new ItemRenderer;
}
qreal Item::fps() const
{
    return m_dptr->fps;
}

void Item::setFps(qreal v)
{
    if (v != m_dptr->fps) {
        m_dptr->fps = v;
        emit fpsChanged();
    }
}

qreal Item::xRotate() const
{
    return m_dptr->xRotate;
}

qreal Item::yRotate() const
{
    return m_dptr->yRotate;
}

qreal Item::zRotate() const
{
    return m_dptr->zRotate;
}

void Item::setXRotate(qreal v)
{
    if (v != m_dptr->xRotate) {
        m_dptr->xRotate = v;
        emit xRotateChanged();
    }
}

void Item::setYRotate(qreal v)
{
    if (v != m_dptr->yRotate) {
        m_dptr->yRotate = v;
        emit yRotateChanged();
    }
}

void Item::setZRotate(qreal v)
{
    if (v != m_dptr->zRotate) {
        m_dptr->zRotate = v;
        emit zRotateChanged();
    }
}
void Item::on_Item_xRotateChanged()
{
    m_json["xRotate"] = m_dptr->xRotate;
}
void Item::on_Item_yRotateChanged()
{
    m_json["yRotate"] = m_dptr->yRotate;
}
void Item::on_Item_zRotateChanged()
{
    m_json["zRotate"] = m_dptr->zRotate;
}
