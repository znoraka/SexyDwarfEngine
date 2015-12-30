#ifndef TESTSCENE_H
#define TESTSCENE_H

#include "engine/scene.h"
#include "engine/camera.h"
#include "engine/entity.h"
#include "engine/components/positioncomponent.h"
#include "game/trianglecomponent.h"
#include "engine/components/volumecomponent.h"
#include "game/mapcomponent.h"
#include "game/pathfollowercomponent.h"
#include "game/towercomponent.h"
#include "game/towerghostcomponent.h"
#include "consts.h"

class TestScene : public Scene
{
    Q_OBJECT
public:
    TestScene();
    void initialize();

public slots:
    void onAddTowerButtonClicked();

private:
    bool handleEvent(QEvent *event);
    QPushButton *slowTowerButton;
    Entity *towerGhost, *towerGhost2, *towerGhost3;
    Entity *map;
    TowerComponent *towerComponent;
    VolumeComponent *towerVolume;
};

#endif // TESTSCENE_H
