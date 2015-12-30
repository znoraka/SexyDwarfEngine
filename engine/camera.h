#ifndef CAMERA_H
#define CAMERA_H

#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>
#include <QDebug>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>

#include "consts.h"

class Camera
{
#define EPSILON 0.000001
public:
    Camera();


    void initialize(qreal ratio, qreal width, qreal height, qreal near, qreal far);
    void update(float delta);
    void setAnimated(bool b);

    QVector3D getPosition() const;
    QVector3D getRotation() const;
    QVector3D getScale() const;

    QVector3D screenToWorld(QVector3D vec, QMatrix4x4 modelview, QMatrix4x4 projection, QVector3D &out1, QVector3D &out2);
    bool intersects(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D r1, QVector3D r2, QVector3D &out);
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

    float width, height, ratio, near, far;
};

int glhUnProjectf(float winx, float winy, float winz, float *modelview, float *projection, int *viewport, float *objectCoordinate);
void MultiplyMatrices4by4OpenGL_FLOAT(float *result, float *matrix1, float *matrix2);
void MultiplyMatrixByVector4by4OpenGL_FLOAT(float *resultvector, const float *matrix, const float *pvector);
int glhInvertMatrixf2(float *m, float *out);

#define SWAP_ROWS_DOUBLE(a, b) { double *_tmp = a; (a)=(b); (b)=_tmp; }
#define SWAP_ROWS_FLOAT(a, b) { float *_tmp = a; (a)=(b); (b)=_tmp; }
#define MAT(m,r,c) (m)[(c)*4+(r)]

#endif // CAMERA_H
