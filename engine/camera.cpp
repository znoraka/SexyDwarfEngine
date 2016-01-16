#include "camera.h"
#include "game.h"

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

void Camera::initialize(float ratio, float width, float height, float near, float far)
{
    this->ratio = 1;
    this->width = width;
    this->height = height;
    this->camNear = near;
    this->camFar = far;
}

void Camera::update(float delta)
{

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, WIDTH, 0.0, HEIGHT, camNear * getScale().x(), camFar * getScale().y());
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glViewport(0, 0, Game::Graphics::width(), Game::Graphics::height());

    glColor3f(1, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex3f(10000, 0, 0);
    glVertex3f(0, 10000, 0);
    glVertex3f(10000, 10000, 0);
    glEnd();

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

QVector3D Camera::screenToWorld(QVector3D vec, QMatrix4x4 modelview, QMatrix4x4 projection)
{
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    float winX = vec.x();
    float winY = vec.y();
    winY = viewport[3] - winY;
    float winZ;
    glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    QVector4D v((winX-(float)viewport[0])/(float)viewport[2]*2.0-1.0,
                   (winY-(float)viewport[1])/(float)viewport[3]*2.0-1.0,
                   0, 1);


    QVector4D v2((winX-(float)viewport[0])/(float)viewport[2]*2.0-1.0,
                   (winY-(float)viewport[1])/(float)viewport[3]*2.0-1.0,
                   1, 1);

    QMatrix4x4 mat(modelview * projection);

    QVector4D t = v * mat.inverted();
    float w = 1 / t.w();
    QVector3D pt1(t.x() * w, t.y() * w, t.z() * w);

    t = v2 * mat.inverted();
    w = 1 / t.w();
    QVector3D pt2(t.x() * w, t.y() * w, t.z() * w);

    QVector3D out;
    intersects(QVector3D(1, 0, vec.z()), QVector3D(0, 1, vec.z()), QVector3D(1, 1, vec.z()),
               pt1, pt2, out);
    return out;
}

bool Camera::intersects(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D r1, QVector3D r2, QVector3D &out)
{
    QVector3D v1 = p2 - p1;
    QVector3D v2 = p3 - p1;

    QVector3D v3 = QVector3D::crossProduct(v1, v2);

    QVector3D vRotRay1 = QVector3D(QVector3D::dotProduct(v1, r1 - p1),
                                   QVector3D::dotProduct(v2, r1 - p1),
                                   QVector3D::dotProduct(v3, r1 - p1));

    QVector3D vRotRay2 = QVector3D(QVector3D::dotProduct(v1, r2 - p1),
                                   QVector3D::dotProduct(v2, r2 - p1),
                                   QVector3D::dotProduct(v3, r2 - p1));

      // Return now if ray will never intersect plane (they're parallel)
      if (vRotRay1.z() == vRotRay2.z()) return false;

      // Find 2D plane coordinates (fX, fY) that the ray interesects with
      float fPercent = vRotRay1.z() / (vRotRay2.z()-vRotRay1.z());
//      QVector3D vIntersect2d = vRotRay1 + (vRotRay1-vRotRay2) * fPercent;
//      fX = vIntersect2d.x;
//      fY = vIntersect2d.y;

      // Note that to find the 3D point on the world-space ray use this
      // vInstersect = R1 + (R1-R2) * fPercent;
      out = r1 + (r1 - r2) * fPercent;

}

QPointF Camera::worldToScreen(QVector3D vec, QMatrix4x4 modelview, QMatrix4x4 projection)
{
    QPointF p;
    QVector4D tmp(vec.x(), vec.y(), vec.z(), 1);
    tmp = tmp * modelview;
    tmp = tmp * projection;


    tmp.setX(tmp.x() / tmp.w());
    tmp.setY(tmp.y() / tmp.w());
    tmp.setZ(tmp.z() / tmp.w());

    tmp.setX(tmp.x() * 0.5 + 0.5);
    tmp.setY(tmp.y() * 0.5 + 0.5);
    tmp.setZ(tmp.z() * 0.5 + 0.5);

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

//    p.setX(tmp.x() * (viewport[2] - viewport[0]));
//    p.setY(tmp.y() * (viewport[3] - viewport[1]));

    p.setX(tmp.x() * Game::Graphics::width());
    p.setY(Game::Graphics::height() - tmp.y() * Game::Graphics::height());

    return p;
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
