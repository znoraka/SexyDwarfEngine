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

TowerComponent *TowerComponent::init(QVector3D canonPosition, Entity *target, float range)
{
    this->canonPosition = canonPosition;
    this->target = target;
    this->range = range;
    return this;
}

void TowerComponent::update(float delta)
{
    if(target) {
        glColor3f(1, 0, 0);

        GLfloat m[16];
        glGetFloatv (GL_MODELVIEW_MATRIX, m);

        glPopMatrix();
        glBegin(GL_LINES);

        glVertex3f(target->getLocalPosition().x(), target->getLocalPosition().y(), target->getLocalPosition().z());
        glVertex3f(canonPosition.x() + getEntity()->getLocalPosition().x(),
                   canonPosition.y() + getEntity()->getLocalPosition().y(),
                   canonPosition.z() + getEntity()->getLocalPosition().z());
        glEnd();
        glPushMatrix();
        glLoadMatrixf(m);
    }
}

TowerComponent *TowerComponent::clone()
{
    TowerComponent *t = TowerComponent::pool->obtain();
    t->canonPosition = canonPosition;
    t->target = target;
    t->range = range;
    return t;
}

float TowerComponent::getRange() const
{
    return range;
}

void TowerComponent::drawRange()
{
    int steps = 50;
    float angle = 360.0 / steps;
    float range = 10;

    glColor3f(0, 1, 0);
    glBegin(GL_LINE_STRIP);

    for (int i = 0; i < steps; ++i) {
//        glVertex3f(0, 0, 0);
        glVertex3f(cos(angle * i) * range, 0.1, sin(angle * i) * range);
    }

    glEnd();
}
