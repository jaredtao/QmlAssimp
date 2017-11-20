#pragma once
#include <QMatrix4x4>
#include <QVector3D>
class JCamera: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D lookAt READ lookAt WRITE setLookAt NOTIFY lookAtChanged)
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

    void sync();

    const QVector3D& position() const;

    const QVector3D& lookAt() const;

    const QVector3D& up() const;

    const QRectF& viewRect() const;

    float aspectRatio() const;

    float farPlane() const;

    float nearPlane() const;

    float fieldOfView() const;

public slots:
    void setPosition(const QVector3D &position);

    void setLookAt(const QVector3D &lookAt);

    void setUp(const QVector3D &up);

    void setViewRect(const QRectF &viewRect);

    void setAspectRatio(float aspectRatio);

    void setFarPlane(float farPlane);

    void setNearPlane(float nearPlane);

    void setFieldOfView(float fieldOfView);

signals:
    void positionChanged(const QVector3D &position);

    void lookAtChanged(const QVector3D &lookAt);

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
    QVector3D m_lookAt;
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
class JMouseCamera : public JCamera
{
    Q_OBJECT
public:
    JMouseCamera(QObject *parent = nullptr);
    Q_INVOKABLE void startRotate();
    Q_INVOKABLE void rotate(qreal x, qreal y);
    Q_INVOKABLE void moveForward(qreal step);
    Q_INVOKABLE void startMove();
    Q_INVOKABLE void move(qreal x, qreal y);
protected:
    QVector3D m_prevPosition;
    QVector3D m_prevLookAt;
    QVector3D m_prevUp;
    QVector3D m_hNormal;
    QVector3D m_vNormal;
};
