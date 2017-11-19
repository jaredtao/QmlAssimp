#pragma once

#include <QObject>
#include <QQuickFramebufferObject>

class Render;
class FBOItem : public QQuickFramebufferObject
{
    Q_OBJECT
public:
    explicit FBOItem(QQuickItem *parent = 0);
    ~FBOItem();
    void timerEvent(QTimerEvent *) override;

    void initRender(const QSize size);
    void render();
    void releaseRender();
public:
    void classBegin() override;
    void componentComplete() override;

    Renderer *createRenderer() const override;
private:
    Render *m_render = nullptr;
};

