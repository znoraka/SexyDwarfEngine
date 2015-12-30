#ifndef TOWERGHOSTCOMPONENT_H
#define TOWERGHOSTCOMPONENT_H



#include <QVector3D>
#include <QtGui/QOpenGLFunctions>
#include <QImage>

#include "engine/components/component.h"
#include "game/towercomponent.h"

class TowerGhostComponent : public Component
{
public:
    TowerGhostComponent();
    static Pool<TowerGhostComponent *> *pool;
    const QString componentName() const override;

    void release() override;

    TowerGhostComponent *init();
    virtual void update(float delta);
    TowerGhostComponent *clone();

    static const QString name;

private:
    QImage map;
    TowerComponent *tc;

};

#endif // TOWERGHOSTCOMPONENT_H
