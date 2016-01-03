#ifndef ENEMYCOMPONENT_H
#define ENEMYCOMPONENT_H



#include <QVector3D>
#include <QtGui/QOpenGLFunctions>
#include <QImage>
#include <engine/camera.h>
#include <engine/fmodmanager.h>

#include "engine/components/component.h"
#include "engine/components/volumecomponent.h"
#include "consts.h"

class EnemyComponent : public Component
{
public:
    EnemyComponent();
    static Pool<EnemyComponent *> *pool;
    const QString componentName() const override;

    void release() override;

    EnemyComponent *init(float speed, float life, float goldOnDeath);
    virtual void update(float delta);
    EnemyComponent *clone();

    static const QString name;

    void takeDamage(float amount);
    void die();
    bool dead();
    void displayLife();

private:
    float speed;
    float life;
    float maxLife;
    float goldOnDeath;

    float lifeDisplayWidth;
    float lifeDisplayHeight;

    FMODEventInstance eventInstance;
};


#endif // ENEMYCOMPONENT_H
