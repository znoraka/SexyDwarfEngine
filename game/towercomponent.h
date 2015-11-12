#ifndef TOWERCOMPONENT_H
#define TOWERCOMPONENT_H

#include <QVector3D>
#include <QtGui/QOpenGLFunctions>

#include "engine/components/component.h"

class TowerComponent : public Component
{
public:
    TowerComponent();
    static Pool<TowerComponent *> *pool;
    const QString componentName() const override;

    void release() override;

    TowerComponent *init(QVector3D canonPosition, Entity *target);
    virtual void update(float delta);

    static const QString name;

private:
    QVector3D canonPosition;
    Entity *target;
};

#endif // TOWERCOMPONENT_H
