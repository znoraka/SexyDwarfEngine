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
    TowerComponent::pool->release(this);
}

TowerComponent *TowerComponent::init(QVector3D canonPosition, Entity *target)
{
    this->canonPosition = canonPosition;
    this->target = target;
    return this;
}

void TowerComponent::update(float delta)
{
    if(target) {
        glColor3f(1, 0, 0);
        glPopMatrix();
        glBegin(GL_LINES);

        glVertex3f(target->getLocalPosition().x(), target->getLocalPosition().y(), target->getLocalPosition().z());
        glVertex3f(canonPosition.x() + getEntity()->getLocalPosition().x(),
                   canonPosition.y() + getEntity()->getLocalPosition().y(),
                   canonPosition.z() + getEntity()->getLocalPosition().z());
        glEnd();
        glPushMatrix();
    }
}

TowerComponent *TowerComponent::clone()
{
    TowerComponent *t = TowerComponent::pool->obtain();
    t->canonPosition = canonPosition;
    t->target = target;
}
