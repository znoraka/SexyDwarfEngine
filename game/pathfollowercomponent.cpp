#include "pathfollowercomponent.h"
#include "game/mapcomponent.h"

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
    EnemyComponent *e = getEntity()->getComponent<EnemyComponent>();

    if(e != nullptr) {
        speed = e->getSpeed();
    }
    delta *= speed * 0.25;

    MapComponent *m = getEntity()->getParent()->getComponent<MapComponent>();

    if(m == nullptr) return;

    for (int i = 0; i < 4; ++i) {
        QVector3D v = getEntity()->getLocalPosition();

        v.setZ(m->getZ(v.x(), v.y()));

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
        } else if (qRed(pixel) == 255 && qBlue(pixel) == 255) {
            //reached base

            EnemyComponent *e = getEntity()->getComponent<EnemyComponent>();
            if(e == nullptr) return;
            e->die();
            Player::getInstance()->takeDamage(1);

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
        getEntity()->setPosition(v + direction);
    }
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
