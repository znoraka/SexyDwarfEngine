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

PathFollowerComponent *PathFollowerComponent::init(QString mapFolder, MapComponent *map, float speed)
{
    path = QImage(mapFolder + "p.png");
    qDebug() << path.width();
    this->speed = speed;
    this->map = map;
    return this;
}

void PathFollowerComponent::update(float delta)
{
    delta *= speed;
    QVector3D v = getEntity()->getLocalPosition();

    //    red = right
    //    blue = left
    //    green = down
    //    white = up

    float x = v.x();
    float y = path.height() - v.y();

    if(x < 0 || y < 0 || x > path.width() || y > path.height()) {
        return;
    }

    QRgb pixel = path.pixel(x, y);

    if(qRed(pixel) == 255 && qGreen(pixel) == 255 && qBlue(pixel) == 255) {
        //up
        getEntity()->setRotation(0, 0, 0);
        direction.setX(0);
        direction.setY(1 * delta);
    } else if (qRed(pixel) == 255) {
        //right
        getEntity()->setRotation(0, 0, 0);
        direction.setX(1 * delta);
        direction.setY(0);
    } else if (qGreen(pixel) == 255) {
        //down
        getEntity()->setRotation(0, 0, 180);
        direction.setX(0);
        direction.setY(-1 * delta);
    } else if (qBlue(pixel) == 255) {
        //left
        getEntity()->setRotation(0, 0, 270);
        direction.setX(-1 * delta);
        direction.setY(0);
    }
    getEntity()->setPosition(getEntity()->getLocalPosition() + direction);

}

PathFollowerComponent *PathFollowerComponent::clone()
{
    PathFollowerComponent *p = PathFollowerComponent::pool->obtain();
    p->path = path;
    p->direction = direction;
    p->speed = speed;
    p->map = map;
}

PathFollowerComponent *PathFollowerComponent::setSpeed(float speed)
{
    this->speed = speed;
    return this;
}
