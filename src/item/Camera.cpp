#include "Camera.h"
#include <QKeyEvent>
#include <QDebug>
JCamera::JCamera(QObject *parent)
    : QObject( parent),
      m_position(0, 0, 10),
      m_lookAt(0, 0, 0),
      m_up(0, 1, 0),
      m_viewRect(0, 0, 0, 0),
      m_aspectRatio(1),
      m_nearPlane(1),
      m_farPlane(1000),
      m_fieldOfView(45)
{
    calculateProjectMatrix();
    calculateViewMatrix();
}

void JCamera::sync()
{
    if (m_viewMatrixIsDirty) {
        calculateViewMatrix();
        m_viewMatrixIsDirty = false;
    }
    if (m_projectMatrixIsDirty) {
        calculateProjectMatrix();
        m_projectMatrixIsDirty = false;
    }
}

void JCamera::calculateViewMatrix()
{
    m_viewMatrix.setToIdentity();
    m_viewMatrix.lookAt(m_position, m_lookAt, m_up);
    qWarning() << m_viewMatrix;
}

void JCamera::calculateProjectMatrix()
{
    m_projectMatrix.setToIdentity();
    if (m_viewRect.normalized().isEmpty()) {
        m_projectMatrix.perspective(m_fieldOfView, m_aspectRatio, m_nearPlane, m_farPlane);
    } else {
        m_projectMatrix.ortho(m_viewRect.left(), m_viewRect.right(),
                              m_viewRect.bottom(), m_viewRect.top(),
                              m_nearPlane, m_farPlane);
    }
}


const QMatrix4x4 &JCamera::viewMatrix()
{
    return m_viewMatrix;
}

const QMatrix4x4 &JCamera::projectMatrix()
{
    return m_projectMatrix;
}

const QVector3D &JCamera::position() const
{
    return m_position;
}

const QVector3D &JCamera::lookAt() const
{
    return m_lookAt;
}

const QVector3D &JCamera::up() const
{
    return m_up;
}

const QRectF &JCamera::viewRect() const
{
    return m_viewRect;
}

float JCamera::aspectRatio() const
{
    return m_aspectRatio;
}

float JCamera::farPlane() const
{
    return m_farPlane;
}

float JCamera::nearPlane() const
{
    return m_nearPlane;
}

float JCamera::fieldOfView() const
{
    return m_fieldOfView;
}

void JCamera::setPosition(const QVector3D &position)
{
    if (m_position == position)
        return;

    m_position = position;
    emit positionChanged(m_position);
    m_viewMatrixIsDirty = true;
    emit requestWindowUpdate();
}

void JCamera::setLookAt(const QVector3D &lookAt)
{
    if (m_lookAt == lookAt)
        return;

    m_lookAt = lookAt;
    emit lookAtChanged(m_lookAt);
    m_viewMatrixIsDirty = true;
    emit requestWindowUpdate();
}

void JCamera::setUp(const QVector3D &up)
{
    if (m_up == up)
        return;

    m_up = up;
    emit upChanged(m_up);
    m_viewMatrixIsDirty = true;
    emit requestWindowUpdate();
}

void JCamera::setViewRect(const QRectF &viewRect)
{
    if (m_viewRect == viewRect)
        return;

    m_viewRect = viewRect;
    emit viewRectChanged(m_viewRect);
    m_projectMatrixIsDirty = true;
    emit requestWindowUpdate();
}

void JCamera::setAspectRatio(float aspectRatio)
{
    //    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_aspectRatio, aspectRatio))
        return;

    m_aspectRatio = aspectRatio;
    emit aspectRatioChanged(m_aspectRatio);
    m_projectMatrixIsDirty = true;
    emit requestWindowUpdate();
}

void JCamera::setFarPlane(float farPlane)
{
    //    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_farPlane, farPlane))
        return;

    m_farPlane = farPlane;
    emit farPlaneChanged(m_farPlane);
    m_projectMatrixIsDirty = true;
    emit requestWindowUpdate();
}

void JCamera::setNearPlane(float nearPlane)
{
    //    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_nearPlane, nearPlane))
        return;

    m_nearPlane = nearPlane;
    emit nearPlaneChanged(m_nearPlane);
    m_projectMatrixIsDirty = true;
    emit requestWindowUpdate();
}

void JCamera::setFieldOfView(float fieldOfView)
{
    //    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_fieldOfView, fieldOfView))
        return;

    m_fieldOfView = fieldOfView;
    emit fieldOfViewChanged(m_fieldOfView);
    m_projectMatrixIsDirty = true;
    emit requestWindowUpdate();
}

void JKeyCamera::move(Qt::Key key)
{
    auto front = lookAt() - position();
    switch(key) {
    case Qt::Key_W: {
        setPosition(position() + (front * speed()));
        break;
    }
    case Qt::Key_S: {
        setPosition(position() - (front * speed()));
        break;
    }
        //Note: 不好用，有问题
        //    case Qt::Key_A: {
        //        auto direct = QVector3D::crossProduct(front, up());
        //        direct.normalize();
        //        direct = direct * speed();
        //        setPosition(position() - direct);
        //        break;
        //    }
        //    case Qt::Key_D: {
        //        auto direct = QVector3D::crossProduct(front, up());
        //        direct.normalize();
        //        direct = direct * speed();
        //        setPosition(position() + direct);
        //        break;
        //    }
    default: break;
    }
}

qreal JKeyCamera::speed() const
{
    return m_speed;
}

void JKeyCamera::setSpeed(qreal speed)
{
    if (qFuzzyCompare(m_speed, speed))
        return;

    m_speed = speed;
    emit speedChanged(m_speed);
}
