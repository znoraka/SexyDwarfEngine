#include "pathfollowercomponent.h"

QString const PathFollowerComponent::name = "pathfollowercomponent";
Pool<PathFollowerComponent *> *PathFollowerComponent::pool = new Pool<PathFollowerComponent*>([] () {return new PathFollowerComponent();});

PathFollowerComponent::PathFollowerComponent()
{
}

const QString PathFollowerComponent::componentName() const
{
    return PathFollowerComponent::name;
}

void PathFollowerComponent::release()
{
    PathFollowerComponent::pool->release(this);
}

PathFollowerComponent *PathFollowerComponent::init(QString mapFolder, float speed)
{
    path = QImage(mapFolder + "p.png").mirrored();
    qDebug() << path.width();
    this->speed = speed;
    return this;
}

void PathFollowerComponent::update(float delta)
{
    delta *= speed;
    QVector3D v = getEntity()->getPosition();

    //    red = right
    //    blue = left
    //    green = down
    //    white = up

    QRgb pixel = path.pixel(-v.z(), -v.x());

    if(qRed(pixel) == 255 && qGreen(pixel) == 255 && qBlue(pixel) == 255) {
        //up
        getEntity()->setRotation(0, 0, 0);
        direction.setX(-1 * delta);
        direction.setZ(0);
    } else if (qRed(pixel) == 255) {
        //right
        getEntity()->setRotation(0, 0, 0);
        direction.setX(0);
        direction.setZ(-1 * delta);
    } else if (qGreen(pixel) == 255) {
        //down
        getEntity()->setRotation(0, 0, 180);
        direction.setX(1 * delta);
        direction.setZ(0);

    } else if (qBlue(pixel) == 255) {
        //left
        getEntity()->setRotation(0, 0, 270);
        direction.setX(0);
        direction.setZ(1 * delta);
    }
    getEntity()->setPosition(v + direction);

}

PathFollowerComponent *PathFollowerComponent::clone()
{
    PathFollowerComponent *p = PathFollowerComponent::pool->obtain();
    p->path = path;
    p->direction = direction;
    p->speed = speed;
}
