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
#include "gui.h"

#include <QCursor>
#include <QHBoxLayout>
#include <QSignalMapper>
#include <QLabel>

class TestScene : public Scene
{
    Q_OBJECT
public:
    TestScene();
    void initialize();
    void update(float delta);

public slots:
    void onAddTowerButtonClicked(TowerComponent::TowerType type);
    void onAddTowerButtonClickedInt(int type);
    void upgradeDamageClicked();
    void upgradeSpeedClicked();


private:
    bool handleEvent(QEvent *event);
    QPushButton *slowTowerButton;
    Entity *towerGhost, *towerGhost2, *towerGhost3;
    Entity *map, *dummy, *boss, *clickedEntity;
    TowerComponent *towerComponent;
    VolumeComponent *towerVolume;
    TowerGhostComponent *towerGhostComponent;
    EnemyComponent *enemyComponent;

    QHBoxLayout *towersIconsLayout;
//    QWidget *upgradeTowersContainer;
//    QLabel *towersInfosLabel;

    TowerInfoUpgradeWindow *tiuw;
    LifeAndGoldWindow *lag;

    QHash<TowerComponent::TowerType, TowerComponent*> towerComponents;

    float mouseX, mouseY;

    void lockCursorInsideWindow();
    void createUiButton(QString bgImage, TowerComponent::TowerType type);
};

#endif // TESTSCENE_H
