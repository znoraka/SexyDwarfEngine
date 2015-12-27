#ifndef CAMERA_H
#define CAMERA_H

#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>
#include <QDebug>
#include <QVector3D>
#include "consts.h"

class Camera
{
public:
    Camera();


    void initialize(qreal ratio, qreal width, qreal height, qreal near, qreal far);
    void update(float delta);
    void setAnimated(bool b);

    QVector3D getPosition() const;
    QVector3D getRotation() const;
    QVector3D getScale() const;

    void setRotation(QVector3D v);
    void setRotation(float x, float y, float z);

    void setPosition(QVector3D v);
    void setPosition(float x, float y, float z);

    void setScale(QVector3D v);
    void setScale(float x, float y, float z);

    bool isAnimated() const;

private:
    int etat = 0;

    QVector3D position;
    QVector3D rotation;
    QVector3D scale;

    bool animated = false;
};

#endif // CAMERA_H
