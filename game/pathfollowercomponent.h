#ifndef PATHFOLLOWERCOMPONENT_H
#define PATHFOLLOWERCOMPONENT_H


#include <QFile>
#include <QImage>

#include "engine/components/component.h"
#include "game/enemycomponent.h"
#include "engine/fmodmanager.h"
#include "game/player.h"

class MapComponent;

class PathFollowerComponent : public Component
{
public:
    PathFollowerComponent();
    static Pool<PathFollowerComponent *> *pool;
    const QString componentName() const override;

    void release() override;

    PathFollowerComponent *init(QString mapFolder, float speed);
    virtual void update(float delta);
    PathFollowerComponent *clone();

    static const QString name;

    PathFollowerComponent *setSpeed(float speed);

private:
    QImage path;
    QVector3D direction;
    float speed;
};

#endif // PATHFOLLOWERCOMPONENT_H
