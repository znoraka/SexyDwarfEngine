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
        glBegin(GL_LINES);
        qDebug() << "target position = " << target->getPosition();
        qDebug() << "entity position = " << getEntity()->getPosition();
        glVertex3f(target->getPosition().x(), target->getPosition().y(), target->getPosition().z());
        glVertex3f(canonPosition.x() + getEntity()->getPosition().x(),
                   canonPosition.y() + getEntity()->getPosition().y(),
                   canonPosition.z() + getEntity()->getPosition().z());
        glEnd();

    }
}
