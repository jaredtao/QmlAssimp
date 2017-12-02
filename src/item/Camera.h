#pragma once
#include <QMatrix4x4>
#include <QVector3D>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
class JCamera: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D front READ front WRITE setFront NOTIFY frontChanged)
    Q_PROPERTY(QVector3D up READ up WRITE setUp NOTIFY upChanged)
    Q_PROPERTY(QRectF viewRect READ viewRect WRITE setViewRect NOTIFY viewRectChanged)
    Q_PROPERTY(float aspectRatio READ aspectRatio WRITE setAspectRatio NOTIFY aspectRatioChanged)
    Q_PROPERTY(float nearPlane READ nearPlane WRITE setNearPlane NOTIFY nearPlaneChanged)
    Q_PROPERTY(float farPlane READ farPlane WRITE setFarPlane NOTIFY farPlaneChanged)
    Q_PROPERTY(float fieldOfView READ fieldOfView WRITE setFieldOfView NOTIFY fieldOfViewChanged)

public:
    JCamera (QObject *parent = nullptr);

    const QMatrix4x4 &viewMatrix();

    const QMatrix4x4 &projectMatrix();

    virtual void sync(int deltaTime  = 16);

    const QVector3D &position() const;

    const QVector3D &front() const;

    const QVector3D &up() const;

    const QRectF &viewRect() const;

    float aspectRatio() const;

    float farPlane() const;

    float nearPlane() const;

    float fieldOfView() const;

public slots:
    void setPosition(const QVector3D &position);

    void setFront(const QVector3D &front);

    void setUp(const QVector3D &up);

    void setViewRect(const QRectF &viewRect);

    void setAspectRatio(float aspectRatio);

    void setFarPlane(float farPlane);

    void setNearPlane(float nearPlane);

    void setFieldOfView(float fieldOfView);

signals:
    void positionChanged(const QVector3D &position);

    void frontChanged(const QVector3D &front);

    void upChanged(const QVector3D &up);

    void viewRectChanged(const QRectF &viewRect);

    void aspectRatioChanged(float aspectRatio);

    void farPlaneChanged(float farPlane);

    void nearPlaneChanged(float nearPlane);

    void fieldOfViewChanged(float fieldOfView);

    void requestWindowUpdate();

protected:
    void calculateViewMatrix();

    void calculateProjectMatrix();

    QVector3D m_position;
    QVector3D m_front;
    QVector3D m_up;
    QRectF m_viewRect;

    float m_aspectRatio;
    float m_farPlane;
    float m_nearPlane;
    float m_fieldOfView;

    QMatrix4x4 m_viewMatrix, m_projectMatrix;

    bool m_viewMatrixIsDirty = false;
    bool m_projectMatrixIsDirty = false;
};

class JKeyCamera : public JCamera {
    Q_OBJECT
    //照相机移动速度
    Q_PROPERTY(qreal speed READ speed WRITE setSpeed NOTIFY speedChanged)
    //照相机转动系数
    Q_PROPERTY(qreal sensitivity READ sensitivity WRITE setSensitivity NOTIFY sensitivityChanged)

public:
    JKeyCamera(QObject *parent = nullptr);
    Q_INVOKABLE void mousePress(int x, int y);
    Q_INVOKABLE void mouseMove(int x, int y);

    Q_INVOKABLE void wheel(QPoint angleDelta);

    Q_INVOKABLE void keyPress(Qt::Key key);
    Q_INVOKABLE void keyRelease(Qt::Key key);

    virtual void sync(int deltaTime) override;

    qreal speed() const;
    qreal sensitivity() const;

public slots:
    void setSpeed(qreal speed);
    void setSensitivity(qreal sensitivity);

signals:
    void speedChanged(qreal speed);
    void sensitivityChanged(qreal sensitivity);

private:
    qreal m_speed = 0.0f;
    bool m_keys[1024] = {false};
    QPoint m_lastMousePos;
    qreal m_sensitivity = 0.05;
    qreal m_aspectSpeed = 10;
    qreal m_yaw = -90;
    qreal m_pitch = 0;
};
