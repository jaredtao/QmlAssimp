#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix>
#include <QTransform>
class Camera
{
public:
    Camera();
    QVector3D m_pos;
    QVector3D m_front;
    QVector3D m_up;
private:


    qreal elapsed;
    qreal yaw, pitch;
    qreal aspect;
};

#endif // CAMERA_H
