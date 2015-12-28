#ifndef PATHFOLLOWERCOMPONENT_H
#define PATHFOLLOWERCOMPONENT_H


#include <QFile>
#include <QImage>

#include "engine/components/component.h"

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

private:
    QImage path;
    QVector3D direction;
    float speed;
};

#endif // PATHFOLLOWERCOMPONENT_H
