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

BulletComponent *BulletComponent::init(Entity *target, float speed, float damage)
{
    this->target = target;
    this->speed = speed;
    this->damage = damage;
    return this;
}

void BulletComponent::update(float delta)
{
    QVector3D dir = -(this->getEntity()->getPosition() - target->getPosition());
    if(dir.length() < 3) {

        EnemyComponent *enemy = static_cast<EnemyComponent*>(target->getComponent(EnemyComponent::name));

        if(enemy != nullptr) {
            enemy->takeDamage(damage);
        }

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
    t->damage = damage;
    return t;
}

void BulletComponent::setTarget(Entity *target)
{
    this->target = target;
}
