#include "bulletcomponent.h"

QString const BulletComponent::name = "bulletcomponent";
Pool<BulletComponent *> *BulletComponent::pool = new Pool<BulletComponent*>([] () {return new BulletComponent();});

BulletComponent::BulletComponent()
{
}

const QString BulletComponent::componentName() const
{
    return BulletComponent::name;
}

void BulletComponent::release()
{
    BulletComponent::pool->release(this);
}

BulletComponent *BulletComponent::init(Entity *target, float speed)
{
    this->target = target;
    this->speed = speed;
    return this;
}

void BulletComponent::update(float delta)
{
    QVector3D dir = -(this->getEntity()->getPosition() - target->getPosition());
    if(dir.length() < 1) {
        this->getEntity()->release();
    } else {
        dir.normalize();
        this->getEntity()->setPosition(this->getEntity()->getPosition() + dir * speed * delta);
    }
}
BulletComponent *BulletComponent::clone()
{
    BulletComponent *t = BulletComponent::pool->obtain();
    t->target = target;
    t->speed = speed;

    return t;
}

void BulletComponent::setTarget(Entity *target)
{
    this->target = target;
}
