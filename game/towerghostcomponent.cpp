#include "towerghostcomponent.h"

QString const TowerGhostComponent::name = "towerghostcomponent";
Pool<TowerGhostComponent *> *TowerGhostComponent::pool = new Pool<TowerGhostComponent*>([] () {return new TowerGhostComponent();});

TowerGhostComponent::TowerGhostComponent()
{
}

const QString TowerGhostComponent::componentName() const
{
    return TowerGhostComponent::name;
}

void TowerGhostComponent::release()
{
    TowerGhostComponent::pool->release(this);
}

TowerGhostComponent *TowerGhostComponent::init()
{

    return this;
}

void TowerGhostComponent::update(float delta)
{
    if(tc == nullptr) {
        tc = static_cast<TowerComponent*>(getEntity()->getComponent(TowerComponent::name));
        if(tc == nullptr) {
            qDebug() << "no TowerComponent found";
        }
    } else {
        tc->drawRange();
    }
}

TowerGhostComponent *TowerGhostComponent::clone()
{

}
