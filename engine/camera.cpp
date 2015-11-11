#include "camera.h"

Camera::Camera()
{
    qDebug() << "new camera";
    position.setX(0);
    position.setY(0);
    position.setZ(0);

    scale.setX(1);
    scale.setY(1);
    scale.setZ(1);

    rotation.setX(0);
    rotation.setY(0);
    rotation.setZ(0);
}

void Camera::initialize(qreal ratio, qreal width, qreal height, qreal near, qreal far)
{
    glViewport(0, 0, width * ratio, height * ratio);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width * ratio, 0.0, height * ratio, near, far);
}

void Camera::update(float delta)
{
//    glLoadIdentity();
    glTranslatef(position.x(), position.y(), position.z());
    glRotatef(rotation.x(), 1, 0, 0);
    glRotatef(rotation.y(), 0, 1, 0);
    glRotatef(rotation.z(), 0, 0, 1);
    glScalef(scale.x(), scale.y() , scale.z());
}


QVector3D Camera::getPosition() const
{
    return this->position;
}

QVector3D Camera::getRotation() const
{
    return this->rotation;
}

QVector3D Camera::getScale() const
{
    return this->scale;
}

void Camera::setRotation(QVector3D v)
{
    this->rotation = v;
}

void Camera::setRotation(float x, float y, float z)
{
    this->rotation.setX(x);
    this->rotation.setY(y);
    this->rotation.setZ(z);
}

void Camera::setPosition(QVector3D v)
{
    this->position = v;
}

void Camera::setPosition(float x, float y, float z)
{
    this->position.setX(x);
    this->position.setY(y);
    this->position.setZ(z);
}

void Camera::setScale(QVector3D v)
{
    this->scale = v;
}

void Camera::setScale(float x, float y, float z)
{
    this->scale.setX(x);
    this->scale.setY(y);
    this->scale.setZ(z);
}


void Camera::setAnimated(bool b)
{
    this->animated = b;
}

bool Camera::isAnimated() const
{
    return this->animated;
}



