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
#include "game/enemycomponent.h"
#include "engine/fmodmanager.h"
#include "player.h"
#include "consts.h"

class TestScene : public Scene
{
    Q_OBJECT
public:
    TestScene();
    void initialize();
    void update(float delta);

public slots:
    void onAddTowerButtonClicked(TowerComponent::TowerType type);

private:
    bool handleEvent(QEvent *event);
    QPushButton *slowTowerButton;
    Entity *towerGhost, *towerGhost2, *towerGhost3;
    Entity *map, *dummy, *boss;
    TowerComponent *towerComponent;
    VolumeComponent *towerVolume;
    TowerGhostComponent *towerGhostComponent;
    EnemyComponent *enemyComponent;

    QList<Entity*> *enemies;
    QHash<TowerComponent::TowerType, TowerComponent*> towerComponents;

    float mouseX, mouseY;
};

#endif // TESTSCENE_H
