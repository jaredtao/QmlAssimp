#ifndef CAMERA_H
#define CAMERA_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>
class CameraPrivate;
class Camera
{
public:
    Camera();
    ~Camera();

    /*
     * Get Camera status
     */
    QVector3D GetPosition();
    QVector3D GetView();
    QVector3D GetUpVector();
    QVector3D GetSpeed();

    //Set Camera speed
    void SetSpeed(qreal speed);
    //Set camera position, view and upVector
    void SetCamera(qreal positionX,
                   qreal positionY,
                   qreal positionZ,
                   qreal viewX,
                   qreal viewY,
                   qreal viewZ,
                   qreal upVectorX,
                   qreal upVectorY,
                   qreal upVectorZ);
    //rotate Camera
    void RotateView(qreal angle, qreal x, qreal y, qreal z);

    /*!
     * \brief YawCamera move camera left and right
     * \param speed
     */
    void YawCamera(qreal speed);
    void moveCamera(qreal speed);

private:
    CameraPrivate *m_dptr;
};

#endif // CAMERA_H
