#ifndef BULLETCOMPONENT_H
#define BULLETCOMPONENT_H

#include <QVector3D>
#include <QtGui/QOpenGLFunctions>
#include <QImage>

#include "engine/components/component.h"
#include "engine/components/volumecomponent.h"
#include "game/enemycomponent.h"
#include "game/towercomponent.h"
#include "consts.h"

class BulletComponent : public Component
{
public:
    BulletComponent();
    static Pool<BulletComponent *> *pool;
    const QString componentName() const override;

    void release() override;

    BulletComponent *init(Entity *target, float speed, float damage);
    virtual void update(float delta);
    BulletComponent *clone();

    static const QString name;

   void setTarget(Entity *target);

private:
    Entity *target;
    float speed;
    float damage;
    QVector3D destination;
};

#endif // BULLETCOMPONENT_H
