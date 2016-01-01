#ifndef TOWERGHOSTCOMPONENT_H
#define TOWERGHOSTCOMPONENT_H

#include <QVector3D>
#include <QtGui/QOpenGLFunctions>
#include <QImage>

#include "engine/components/component.h"
#include "engine/components/volumecomponent.h"
#include "game/towercomponent.h"
#include "consts.h"

class TowerGhostComponent : public Component
{
public:
    TowerGhostComponent();
    static Pool<TowerGhostComponent *> *pool;
    const QString componentName() const override;

    void release() override;

    TowerGhostComponent *init(QString mapFolder);
    virtual void update(float delta);
    TowerGhostComponent *clone();

    static const QString name;

    bool hasRoom();

private:
    QImage map;
    TowerComponent *tc;
    VolumeComponent *vc;
    QString mapFolder;

    void initDependencies();
};

#endif // TOWERGHOSTCOMPONENT_H
