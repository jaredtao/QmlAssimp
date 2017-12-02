#include "Camera.h"
#include <QKeyEvent>
#include <QDebug>
#include <QtMath>
JCamera::JCamera(QObject *parent)
    : QObject( parent),
      m_position(0, 0, 10),
      m_front(0, 0, -1),
      m_up(0, 1, 0),
      m_viewRect(0, 0, 0, 0),
      m_aspectRatio(1),
      m_farPlane(1000),
      m_nearPlane(1),
      m_fieldOfView(45)
{
    calculateProjectMatrix();
    calculateViewMatrix();
}

void JCamera::sync(int deltaTime)
{
    Q_UNUSED(deltaTime)
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
    m_viewMatrix.lookAt(m_position, m_position + m_front, m_up);
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

const QVector3D &JCamera::front() const
{
    return m_front;
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

void JCamera::setFront(const QVector3D &front)
{
    if (m_front == front)
        return;

    m_front = front;
    emit frontChanged(m_front);
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
    if (qFuzzyCompare(m_aspectRatio, aspectRatio))
        return;

    m_aspectRatio = aspectRatio;
    emit aspectRatioChanged(m_aspectRatio);
    m_projectMatrixIsDirty = true;
    emit requestWindowUpdate();
}

void JCamera::setFarPlane(float farPlane)
{
    if (qFuzzyCompare(m_farPlane, farPlane))
        return;

    m_farPlane = farPlane;
    emit farPlaneChanged(m_farPlane);
    m_projectMatrixIsDirty = true;
    emit requestWindowUpdate();
}

void JCamera::setNearPlane(float nearPlane)
{
    if (qFuzzyCompare(m_nearPlane, nearPlane))
        return;

    m_nearPlane = nearPlane;
    emit nearPlaneChanged(m_nearPlane);
    m_projectMatrixIsDirty = true;
    emit requestWindowUpdate();
}

void JCamera::setFieldOfView(float fieldOfView)
{
    if (qFuzzyCompare(m_fieldOfView, fieldOfView))
        return;

    m_fieldOfView = fieldOfView;
    emit fieldOfViewChanged(m_fieldOfView);
    m_projectMatrixIsDirty = true;
    emit requestWindowUpdate();
}

qreal JKeyCamera::speed() const
{
    return m_speed;
}

qreal JKeyCamera::sensitivity() const
{
    return m_sensitivity;
}

void JKeyCamera::setSpeed(qreal speed)
{
    if (qFuzzyCompare(m_speed, speed))
        return;

    m_speed = speed;
    emit speedChanged(m_speed);
}

void JKeyCamera::setSensitivity(qreal sensitivity)
{
    if (qFuzzyCompare(m_sensitivity, sensitivity))
        return;

    m_sensitivity = sensitivity;
    emit sensitivityChanged(m_sensitivity);
}

JKeyCamera::JKeyCamera(QObject *parent) : JCamera(parent)
{
}

void JKeyCamera::mousePress(int x, int y)
{
    m_lastMousePos = QPoint(x, y);
}

void JKeyCamera::mouseMove(int x, int y)
{
    auto pos = QPoint(x, y);
	QPointF diff;
    diff = pos - m_lastMousePos;
    m_lastMousePos = pos;

    qreal xOffset = diff.x() * sensitivity();
    qreal yOffset = -diff.y() * sensitivity();

    m_yaw += xOffset;
    m_pitch += yOffset;

    if (m_pitch > 89.0) {
        m_pitch = 89.0;
    } else if (m_pitch < -89.0) {
        m_pitch = -89.0;
    }
    QVector3D front;
    front.setX(cos(qDegreesToRadians(m_pitch)) * cos( qDegreesToRadians(m_yaw)));
    front.setY(sin(qDegreesToRadians(m_pitch)));
    front.setZ(cos(qDegreesToRadians(m_pitch)) * sin(qDegreesToRadians(m_yaw)));
    setFront(front.normalized());
}

void JKeyCamera::wheel(QPoint angleDelta)
{
    int offset = angleDelta.y() < 0 ? -1.0 : 1.0;
    qreal field = fieldOfView() + offset * m_aspectSpeed;

    if (1.0 <= field && field <= 45.0) {
        setFieldOfView(field);
    }
}

void JKeyCamera::keyPress(Qt::Key key)
{
    if (0 <= key && key < (int)(sizeof(m_keys)/ sizeof(m_keys[0]))) {
        m_keys[key] = true;
    }
}

void JKeyCamera::keyRelease(Qt::Key key)
{
    if (0 <= key && key < (int)(sizeof(m_keys)/ sizeof(m_keys[0]))) {
        m_keys[key] = false;
    }
}

void JKeyCamera::sync(int deltaTime)
{
    JCamera::sync(deltaTime);
    auto cameraSpeed = speed() * deltaTime / 1000.0;
    if (m_keys[Qt::Key_A]) {
        setPosition(position() - QVector3D::crossProduct(front(), up()).normalized() * cameraSpeed);
    }
    if (m_keys[Qt::Key_D])  {
        setPosition(position() + QVector3D::crossProduct(front(), up()).normalized() * cameraSpeed);
    }
    if(m_keys[Qt::Key_W]) {
        setPosition(position() + cameraSpeed * front());
    }
    if (m_keys[Qt::Key_S])  {
        setPosition(position() - cameraSpeed * front());
    }
}
