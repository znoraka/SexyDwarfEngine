#ifndef TOWERCOMPONENT_H
#define TOWERCOMPONENT_H

#include <QVector3D>
#include <QVector>
#include <QtGui/QOpenGLFunctions>

#include "engine/components/component.h"

class TowerComponent : public Component
{
public:
    TowerComponent();
    static Pool<TowerComponent *> *pool;
    const QString componentName() const override;

    void release() override;

    TowerComponent *init(QVector3D canonPosition, QVector<Entity*> *enemies, float range);
    virtual void update(float delta);
    TowerComponent *clone();

    static const QString name;

    float getRange() const;
    void drawRange(QVector3D color);


private:
    QVector3D canonPosition;
    Entity *target;
    float range;
    QVector<Entity*> *enemies;

    void setTarget();
};

#endif // TOWERCOMPONENT_H
