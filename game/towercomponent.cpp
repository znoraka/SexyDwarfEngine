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
    //    delete enemies;
    TowerComponent::pool->release(this);
}

TowerComponent *TowerComponent::init(QVector3D canonPosition, QList<Entity *> *enemies, float range, float attackSpeed)
{
    this->canonPosition = canonPosition;
    this->enemies = enemies;
    this->range = range;
    this->target = nullptr;
    this->elapsed = 0;
    this->attackSpeed = attackSpeed;
    this->volume = VolumeComponent::pool->obtain()->init(":/assets/ply/sphere.ply");
    this->ready = false;
    return this;
}

void TowerComponent::update(float delta)
{
    this->setTarget();

    if(ready && target) {
        elapsed += delta * 0.01;
        if(elapsed > attackSpeed) {
            shoot();
            elapsed = 0;
        }

        //        glColor3f(1, 0, 0);

        //        GLfloat m[16];
        //        glGetFloatv (GL_MODELVIEW_MATRIX, m);

        //        glPopMatrix();
        //        glBegin(GL_LINES);

        //        glVertex3f(target->getLocalPosition().x(), target->getLocalPosition().y(), target->getLocalPosition().z());
        //        glVertex3f(canonPosition.x() + getEntity()->getLocalPosition().x(),
        //                   canonPosition.y() + getEntity()->getLocalPosition().y(),
        //                   canonPosition.z() + getEntity()->getLocalPosition().z());
        //        glEnd();
        //        glPushMatrix();
        //        glLoadMatrixf(m);
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
    return t;
}

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

    float range = this->range / this->getEntity()->getScale().x();

    for (int i = 0; i < steps; ++i) {
        glVertex3f(cos(angle * i) * range, 0.1, sin(angle * i) * range);
    }

    glEnd();
}

void TowerComponent::setReady()
{
    this->ready = true;
}

void TowerComponent::setTarget()
{
    if(target != nullptr) {
        if(target->getParent() == nullptr || (this->getEntity()->getLocalPosition() - target->getLocalPosition()).length() > range) {
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
    Entity *e = Entity::pool->obtain()->
            addComponent(BulletComponent::pool->obtain()->init(target, 0.5, 1))->
            addComponent(volume->clone())->
            setPosition(this->getEntity()->getPosition() + canonPosition)->
            setScale(0.02, 0.02, 0.02);
    this->getEntity()->getParent()->addChild(e);
}
