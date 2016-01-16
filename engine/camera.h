#ifndef CAMERA_H
#define CAMERA_H

#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>
#include <QDebug>
#include <QPointF>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>

#include "consts.h"

class Game;

class Camera
{
public:
    Camera();


    void initialize(float width, float height, float n_ear, float f_ar);
    void update(float delta);
    void setAnimated(bool b);

    QVector3D getPosition() const;
    QVector3D getRotation() const;
    QVector3D getScale() const;

    static QVector3D screenToWorld(QVector3D vec, QMatrix4x4 modelview, QMatrix4x4 projection);
    static bool intersects(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D r1, QVector3D r2, QVector3D &out);
    static QPointF worldToScreen(QVector3D vec, QMatrix4x4 modelview, QMatrix4x4 projection);

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

    float width, height;
    float n_ear;
    float f_ar;
};

#endif // CAMERA_H
