#include "Camera.h"

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


JMouseCamera::JMouseCamera(QObject *parent) : JCamera(parent)
{

}

void JMouseCamera::startRotate()
{
    m_prevPosition = m_position;
    m_prevUp = m_up;
}

void JMouseCamera::rotate(qreal x, qreal y)
{
    QMatrix4x4 transformMatrix, rotateMatrix;
    const auto yUp = QVector3D(0.0f, 1.0f, 0.0f);
    auto side = QVector3D::crossProduct(m_lookAt - m_prevPosition, yUp).normalized();
    rotateMatrix.rotate(-x, yUp);
    rotateMatrix.rotate(-y * m_prevUp.y() / qAbs(m_prevUp.y()), side);

    transformMatrix.translate(m_lookAt);
    transformMatrix *= rotateMatrix;
    transformMatrix.translate(-m_lookAt);

    setPosition(transformMatrix * m_prevPosition);
    setUp(rotateMatrix * m_up);
}

void JMouseCamera::moveForward(qreal step)
{
    QVector3D forward = m_lookAt - m_position;
    qreal forwardLength = forward.length();
    if (forwardLength < step) return ;
    setPosition(m_position + forward.normalized() * step);
}

void JMouseCamera::startMove()
{
    m_vNormal = m_up.normalized();
    m_hNormal = QVector3D::crossProduct( m_up, m_lookAt - m_position);
    m_prevPosition = m_position;
    m_prevLookAt = m_lookAt;
}

void JMouseCamera::move(qreal x, qreal y)
{
    QVector3D delta = m_hNormal * x + m_vNormal * y * (m_lookAt - m_position).length();
    setPosition(m_prevPosition + delta);
    setLookAt(m_prevLookAt + delta);
}
