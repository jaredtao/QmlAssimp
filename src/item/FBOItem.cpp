#include "FBOItem.h"
#include "Render.h"
#include <QOpenGLFramebufferObjectFormat>
#define ARRAYLEN(arr) (sizeof(arr)/sizeof(arr[0]))

class FBORenderer : public QQuickFramebufferObject::Renderer
{
public:
    FBORenderer(FBOItem *fboItem) : m_item(fboItem)
    {
    }
    ~FBORenderer()
    {
        m_item->releaseRender();
    }
    // Renderer interface
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
        int sample = qgetenv( "sample" ).toUInt( );// 支持通过环境变量来设置采样率
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
    startTimer(1);
}

void FBOItem::timerEvent(QTimerEvent *)
{
    update();
}

void FBOItem::initRender(const QSize size)
{
    m_render = new Render;
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

