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
    this->getEntity()->setGlobalPosition(0, 0, 0);
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
    EnemyComponent *enemy = target->getComponent<EnemyComponent>();

    if(enemy != nullptr) {
        destination = target->getPosition();
    }
    QVector3D dir = -(this->getEntity()->getPosition() - destination);
    if(dir.length() < 10) {
        if(enemy != nullptr) {
            enemy->takeDamage(damage);
            QVector3D v = getEntity()->getPosition();
            v.setX(v.x() / 768);
            v.setY(v.y() / 624);
            v.setZ(0);

            FMODManager::getInstance()->setCurrentEvent("event:/hit");
            FMODManager::getInstance()->setEventInstancePosition(v);
            FMODManager::getInstance()->setEventInstanceVolume(0.4);
            FMODManager::getInstance()->setParameterValue("pitch", 0.3 + (qrand() % 200) * 0.001);
            FMODManager::getInstance()->startEventInstance();
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
