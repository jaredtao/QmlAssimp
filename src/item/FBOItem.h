#pragma once

#include <QObject>
#include <QQuickFramebufferObject>

#include "model/model.h"
#include "Camera.h"
class Render;
class FBOItem : public QQuickFramebufferObject
{
    Q_OBJECT

    Q_PROPERTY(QQmlListProperty<QObject> data READ data)
    Q_CLASSINFO("DefaultProperty", "data")
public:
    explicit FBOItem(QQuickItem *parent = 0);
    ~FBOItem();
    void timerEvent(QTimerEvent *) override;

    void initRender(const QSize size);
    void render();
    void releaseRender();

    inline void setCamera(JCamera *camera) {m_camera = camera;}
    inline void setModel(Model *model) {m_model = model;}
public:
    void classBegin() override;
    void componentComplete() override;

    Renderer *createRenderer() const override;
    QQmlListProperty<QObject> data();
private:
    static void qobjectListAppend(QQmlListProperty<QObject> *propertys, QObject *obj);
    Render *m_render = nullptr;

    JCamera *m_camera = nullptr;
    Model *m_model = nullptr;
};

