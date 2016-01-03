#include "enemycomponent.h"

QString const EnemyComponent::name = "EnemyComponent";
Pool<EnemyComponent *> *EnemyComponent::pool = new Pool<EnemyComponent*>([] () {return new EnemyComponent();});

EnemyComponent::EnemyComponent()
{
    this->lifeDisplayWidth = 25;
    this->lifeDisplayHeight = 4;
}

const QString EnemyComponent::componentName() const
{
    return EnemyComponent::name;
}

void EnemyComponent::release()
{
    EnemyComponent::pool->release(this);
}

EnemyComponent *EnemyComponent::init(float speed, float life, float goldOnDeath)
{
    this->life = life;
    this->maxLife = life;
    this->speed = speed;
    this->goldOnDeath = goldOnDeath;
    return this;
}

void EnemyComponent::update(float delta)
{
    displayLife();
}

EnemyComponent *EnemyComponent::clone()
{
    EnemyComponent *t = EnemyComponent::pool->obtain();
    t->life = maxLife;
    t->maxLife = maxLife;
    t->speed = speed;
    t->goldOnDeath = goldOnDeath;

    return t;
}

void EnemyComponent::takeDamage(float amount)
{
    this->life -= amount;
    if(dead()) {
        die();
    }
}

void EnemyComponent::die()
{
    QVector3D v = getEntity()->getPosition();
    v.setX(v.x() / 768);
    v.setY(v.y() / 624);
    qDebug() << v;

    if(maxLife > 50) {
        for (int i = 0; i < 20; ++i) {
            FMODManager::getInstance()->setCurrentEvent("event:/death");
            FMODManager::getInstance()->setEventInstancePosition(v);
            FMODManager::getInstance()->startEventInstance();
        }
        FMODManager::getInstance()->setCurrentMusicParameterValue("boss", 0);
        FMODManager::getInstance()->setCurrentMusicVolume(0.5);

    } else {
        FMODManager::getInstance()->setCurrentEvent("event:/death");
        FMODManager::getInstance()->setEventInstancePosition(v);
        FMODManager::getInstance()->startEventInstance();
    }
    this->getEntity()->release();
}

bool EnemyComponent::dead()
{
    return life <= 0;
}

void EnemyComponent::displayLife()
{
    glColor3f(1, 0, 0);

    auto unRotate = []() {
        GLfloat m[16];
        glGetFloatv (GL_MODELVIEW_MATRIX, m);

        float d = sqrt(m[0] * m[0] + m[4] * m[4] + m[8] * m[8]);

        m[0] = d;
        m[1] = 0;
        m[2] = 0;
        m[4] = 0;
        m[5] = d;
        m[6] = 0;
        m[8] = 0;
        m[9] = 0;
        m[10] = d;

        glLoadMatrixf(m);
    };

    glPushMatrix();
    unRotate();
    glRotatef(90, 1, 0, 0);

    float x1 = - ((life / maxLife) * lifeDisplayWidth * 0.5) / getEntity()->getScale().x();
    float x2 = ((life / maxLife) * lifeDisplayWidth * 0.5) / getEntity()->getScale().x();

    float z1 = - lifeDisplayHeight * 0.5 / getEntity()->getScale().z() - 20  / getEntity()->getScale().x();
    float z2 = lifeDisplayHeight * 0.5 / getEntity()->getScale().z() - 20 / getEntity()->getScale().x();

    glBegin(GL_QUADS);

    glVertex3f(x1, 0, z1);
    glVertex3f(x1, 0, z2);
    glVertex3f(x2, 0, z2);
    glVertex3f(x2, 0, z1);

    glEnd();
    glPopMatrix();
}
