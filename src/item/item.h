#ifndef ITEM_H
#define ITEM_H

#include <QObject>

#include <QQuickItem>
#include <QQuickFramebufferObject>

class Item : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(qreal fps READ fps WRITE setFps NOTIFY fpsChanged)
public:
    qreal fps() const;
    void setFps(qreal v);

    explicit Item(QQuickItem *parent = 0);
    void timerEvent(QTimerEvent *) override;

    // QQmlParserStatus interface
public:
    void classBegin() override;
    void componentComplete() override;

    // QQuickFramebufferObject interface
public:
    Renderer *createRenderer() const override;

signals:
    void fpsChanged();
private:
    qreal m_fps;
};

#endif // ITEM_H
