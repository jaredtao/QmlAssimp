#include "FBOItem.h"
#include "Render.h"
#include <QQuickWindow>
#include <QOpenGLFramebufferObjectFormat>
#define ARRAYLEN(arr) (sizeof(arr)/sizeof(arr[0]))


class FBORenderer : public QQuickFramebufferObject::Renderer
{
public:
    FBORenderer(FBOItem *fboItem) : m_item(fboItem)
    {
    }
protected:
    void render() override
    {
        m_item->render();
        update();
    }
    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override
    {
        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        // 支持通过环境变量来设置采样率
        int sample = qgetenv( "sample" ).toUInt( );
        if ( sample <= 0 ) sample = 1;
        format.setSamples( sample );
        m_item->initRender(size);
        return new QOpenGLFramebufferObject(size, format);
    }
private:
    FBOItem *m_item;
};


FBOItem::FBOItem(QQuickItem *parent): QQuickFramebufferObject(parent)
{
    setObjectName("FBOItem");
}

FBOItem::~FBOItem()
{
}
void FBOItem::classBegin()
{
}

void FBOItem::componentComplete()
{
    QQuickWindow* win = window( );
    if ( win != Q_NULLPTR )
    {
        connect( win, &QQuickWindow::sceneGraphInvalidated, this, &FBOItem::releaseRender, Qt::DirectConnection);
    }
    startTimer(16);

}

void FBOItem::timerEvent(QTimerEvent *)
{
    update();
}

void FBOItem::initRender(const QSize size)
{
    m_render = new Render(m_camera, m_model);
    m_render->Init(size);
}

void FBOItem::render()
{
    if (m_render) {
        m_render->Paint();
    }
}

void FBOItem::releaseRender()
{
    if (m_render) {
        delete m_render;
        m_render = nullptr;
    }
}

QQuickFramebufferObject::Renderer *FBOItem::createRenderer() const
{
    return new FBORenderer(const_cast<FBOItem *>(this));
}

void FBOItem::qobjectListAppend(QQmlListProperty<QObject> *propertys, QObject *object)
{
    FBOItem *fboItem = qobject_cast<FBOItem *>(propertys->object);
    if (object->inherits("JCamera")) {
        auto camera = qobject_cast<JCamera *>(object);
        fboItem->setCamera(camera);
        connect(camera, &JCamera::requestWindowUpdate, fboItem, &FBOItem::update);
    }
    if (object->inherits("Model")) {
        auto model = qobject_cast<Model *>(object);
        fboItem->setModel(model);
    }
}

QQmlListProperty<QObject> FBOItem::data()
{
    return QQmlListProperty<QObject>(this,
                                     nullptr,
                                     qobjectListAppend,
                                     nullptr,
                                     nullptr,
                                     nullptr);
}
