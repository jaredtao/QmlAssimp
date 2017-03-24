#ifndef ITEM_H
#define ITEM_H

#include <QObject>

#include <QQuickItem>
#include <QQuickFramebufferObject>
#include <QJsonObject>
class ItemPrivate;
class Item : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(qreal fps READ fps WRITE setFps NOTIFY fpsChanged)
    Q_PROPERTY(qreal xRotate READ xRotate WRITE setXRotate NOTIFY xRotateChanged)
    Q_PROPERTY(qreal yRotate READ yRotate WRITE setYRotate NOTIFY yRotateChanged)
    Q_PROPERTY(qreal zRotate READ zRotate WRITE setZRotate NOTIFY zRotateChanged)

public:
    qreal fps() const;
    void setFps(qreal v);

    qreal xRotate() const;
    qreal yRotate() const;
    qreal zRotate() const;

    void setXRotate(qreal);
    void setYRotate(qreal);
    void setZRotate(qreal);
public:
    explicit Item(QQuickItem *parent = 0);
    ~Item();
    void timerEvent(QTimerEvent *) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void checkKey();
    // QQmlParserStatus interface
public:
    void classBegin() override;
    void componentComplete() override;

    // QQuickFramebufferObject interface
public:
    Renderer *createRenderer() const override;

signals:
    void fpsChanged();
    void xRotateChanged();
    void yRotateChanged();
    void zRotateChanged();
public slots:
    void on_Item_xRotateChanged();
    void on_Item_yRotateChanged();
    void on_Item_zRotateChanged();
public:
    //use json  as Command Queue.
    QJsonObject m_json;
private:
    ItemPrivate *m_dptr;

};

#endif // ITEM_H
