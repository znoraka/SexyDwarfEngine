#ifndef PATHFOLLOWERCOMPONENT_H
#define PATHFOLLOWERCOMPONENT_H


#include <QFile>
#include <QImage>

#include "engine/components/component.h"
#include "game/enemycomponent.h"
#include "game/mapcomponent.h"
#include "engine/fmodmanager.h"

class PathFollowerComponent : public Component
{
public:
    PathFollowerComponent();
    static Pool<PathFollowerComponent *> *pool;
    const QString componentName() const override;

    void release() override;

    PathFollowerComponent *init(QString mapFolder, MapComponent *map, float speed);
    virtual void update(float delta);
    PathFollowerComponent *clone();

    static const QString name;

    PathFollowerComponent *setSpeed(float speed);

private:
    QImage path;
    MapComponent *map;
    QVector3D direction;
    float speed;
};

#endif // PATHFOLLOWERCOMPONENT_H
