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
    this->ratio = ratio;
    this->width = width;
    this->height = height;
    this->near = near;
    this->far = far;
}

void Camera::update(float delta)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, 0.0, height, near, far);
    glViewport(0, 0, width * ratio, height * ratio);

//    glLoadIdentity();
//    glTranslatef(position.x() - WIDTH * 0.5, position.y(), position.z() - HEIGHT * 0.5);
    glTranslatef(position.x(), position.y(), position.z());
    glRotatef(rotation.x(), 1, 0, 0);
    glRotatef(rotation.y(), 0, 1, 0);
    glRotatef(rotation.z(), 0, 0, 1);
    glScalef(scale.x(), scale.y() , scale.z());
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
//    glTranslatef(position.x() + WIDTH * 0.5, position.y(), position.z() + HEIGHT * 0.5);
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

QVector3D Camera::screenToWorld(QVector3D vec)
{
    this->update(0);
    vec.setX(qMin(vec.x(), width));
    vec.setZ(qMin(vec.z(), height));
    vec.setX(qMax(vec.x(), 0.0f));
    vec.setZ(qMax(vec.z(), 0.0f));

    GLfloat m[16];
    glGetFloatv (GL_MODELVIEW_MATRIX, m);
    QMatrix4x4 view(m);

    glGetFloatv (GL_PROJECTION_MATRIX, m);
    QMatrix4x4 proj(m);

    QMatrix4x4 viewproj = proj * view;

    float x = vec.x(); float z = vec.z();

    QVector4D v((2 * x) / (width * ratio) - 1,
                2 * vec.y() - 1,
                (2 * z) / (height * ratio) - 1,
                1);

    QVector4D v2 = v * viewproj.inverted();

    float w = 1.0 / v2.w();

    vec.setX( - (v2.x()) * w * 0.5);
    vec.setY(0);
    vec.setZ((v2.z()) * w);
    return vec;
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



