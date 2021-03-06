#include "towercomponent.h"

QString const TowerComponent::name = "towercomponent";
Pool<TowerComponent *> *TowerComponent::pool = new Pool<TowerComponent*>([] () {return new TowerComponent();});

TowerComponent::TowerComponent()
{
}

const QString TowerComponent::componentName() const
{
    return TowerComponent::name;
}

void TowerComponent::release()
{
    volume->deleteBuffersData();
    TowerComponent::pool->release(this);
}

TowerComponent *TowerComponent::init(QVector3D canonPosition, QList<Entity *> *enemies, float range, float attackSpeed, float damage, int price, TowerType type)
{
    this->canonPosition = canonPosition;
    this->enemies = enemies;
    this->range = range;
    this->target = nullptr;
    this->elapsed = 0;
    this->attackSpeed = attackSpeed;
    this->volume = VolumeComponent::pool->obtain()->init(":/assets/ply/sphere.ply");
    this->type = type;
    this->damage = damage;
    this->price = price;
    this->damageUpgradePrice = price * 0.3;
    this->speedUpgradePrice = price * 0.5;
    this->ready = false;
    return this;
}

void TowerComponent::update(float delta)
{
    switch (type) {
    case BULLET:
        getEntity()->getComponent<VolumeComponent>()->setColor(QVector4D(0, 0.70, 0.01, 1));
        break;
    case FIRE:
        getEntity()->getComponent<VolumeComponent>()->setColor(QVector4D(1, 0.32, 0, 1));
        break;
    case ICE:
        getEntity()->getComponent<VolumeComponent>()->setColor(QVector4D(0, 0.55, 0.70, 1));
        break;
    case LIGHTNING:
        getEntity()->getComponent<VolumeComponent>()->setColor(QVector4D(0.8, 0.8, 0.8, 1));
        break;
    default:
        break;
    }

    this->setTarget();

    if(ready && target) {
        elapsed += delta * 0.01;
        if(elapsed > attackSpeed) {
            shoot();
            elapsed = 0;
        }
    }
}

TowerComponent *TowerComponent::clone()
{
    TowerComponent *t = TowerComponent::pool->obtain();
    t->canonPosition = canonPosition;
    t->target = target;
    t->enemies = enemies;
    t->range = range;
    t->attackSpeed = attackSpeed;
    t->elapsed = elapsed;
    t->volume = volume;
    t->ready = false;
    t->type = type;
    t->damage = damage;
    t->price = price;
    t->damageUpgradePrice = damageUpgradePrice;
    t->speedUpgradePrice = speedUpgradePrice;
    return t;
}

//TowerComponent *TowerComponent::setColorFromType(TowerComponent::TowerType type)
//{

//    return this;
//}

float TowerComponent::getRange() const
{
    return range;
}

void TowerComponent::drawRange(QVector3D color)
{
    int steps = 50;
    float angle = 360.0 / steps;

    glColor3f(color.x(), color.y(), color.z());
    glBegin(GL_LINE_STRIP);

    float range = this->range / this->getEntity()->getLocalScale().x();

    for (int i = 0; i < steps; ++i) {
        glVertex3f(cos(angle * i) * range, 0.1, sin(angle * i) * range);
    }

    glEnd();
}

void TowerComponent::setReady()
{
    this->ready = true;
}

int TowerComponent::getPrice() const
{
    return this->price;
}

void TowerComponent::upgradeDamage()
{
    this->damage *= 1.5;
    this->damageUpgradePrice *= 2;
}

void TowerComponent::upgradeSpeed()
{
    this->attackSpeed /= 1.2;
    this->speedUpgradePrice *= 4;
}

int TowerComponent::getDamageUpgradePrice() const
{
    return damageUpgradePrice;
}

int TowerComponent::getSpeedUpgradePrice() const
{
    return speedUpgradePrice;
}

float TowerComponent::getDamage() const
{
    return damage;
}

float TowerComponent::getSpeed() const
{
    return attackSpeed;
}

void TowerComponent::setTarget()
{
    if(target != nullptr) {
        if(target->getComponent(EnemyComponent::name) == nullptr || (this->getEntity()->getLocalPosition() - target->getLocalPosition()).length() > range) {
            target = nullptr;
        }
    }

    if(target == nullptr) {
        float maxDist = range;
        foreach (Entity *enemy, *enemies) {
            QVector3D d = this->getEntity()->getLocalPosition() - enemy->getLocalPosition();
            if(d.length() < maxDist) {
                maxDist = d.length();
                target = enemy;
            }
        }
    }
}

void TowerComponent::shoot()
{
    QVector3D v = getEntity()->getPosition();
    v.setX(v.x() / 768);
    v.setY(v.y() / 624);
    v.setZ(0);

    switch (type) {
    case FIRE:
        FMODManager::getInstance()->setCurrentEvent("event:/fireshot");
        FMODManager::getInstance()->setEventInstanceVolume(0.5);
        break;
    case ICE:
        FMODManager::getInstance()->setCurrentEvent("event:/iceshot");
        FMODManager::getInstance()->setEventInstanceVolume(0.3);
        break;
    case LIGHTNING:
        FMODManager::getInstance()->setCurrentEvent("event:/lightning");
        FMODManager::getInstance()->setEventInstanceVolume(0.5);
       break;
    case BULLET:
        FMODManager::getInstance()->setCurrentEvent("event:/firebullet");
        FMODManager::getInstance()->setEventInstanceVolume(0.3);
        break;
    }
    FMODManager::getInstance()->setEventInstancePosition(v);
    FMODManager::getInstance()->setParameterValue("pitch", 0.4 + (qrand() % 100) * 0.001);
    FMODManager::getInstance()->startEventInstance();

    switch (type) {
    case BULLET:
        volume->setColor(QVector4D(0, 0.70, 0.01, 1));
        break;
    case FIRE:
        volume->setColor(QVector4D(1, 0.32, 0, 1));
        break;
    case ICE:
        volume->setColor(QVector4D(0, 0.55, 0.70, 1));
        break;
    case LIGHTNING:
        volume->setColor(QVector4D(0.8, 0.8, 0.8, 1));
        break;
    default:
        break;
    }

    Entity *e = Entity::pool->obtain()->
            addComponent(BulletComponent::pool->obtain()->init(target, 0.5, damage))->
            addComponent(volume->clone())->
            setPosition(this->getEntity()->getPosition() + canonPosition)->
            setScale(0.02, 0.02, 0.02);
    this->getEntity()->getParent()->addChild(e);
}
