#ifndef GUI_H
#define GUI_H

#include <QCursor>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "engine/entity.h"
#include "game/towercomponent.h"

class Gui
{
public:
    Gui();
};

class TowerInfoUpgradeWindow : public QWidget
{
public :
    TowerInfoUpgradeWindow(QWidget *parent = 0);
    void show(int x, int y, Entity *tower);
    void hide();
    void update();

    QPushButton *getDamageUpgradeButton();
    QPushButton *getSpeedUpgrageButton();

private:
    QVBoxLayout *vlayout;
    QHBoxLayout *hlayout;
    QPushButton *upgradeDamage, *upgradeSpeed;
    QLabel *damagePrice, *speedPrice, *damage, *range, *speed;
    QWidget *container;
    Entity *lastEntity;
};

class LifeAndGoldWindow : public QWidget
{
public:
    LifeAndGoldWindow(QWidget *parent = 0);
    void update();

private:
    QHBoxLayout *layout, *sublayoutLeft, *sublayoutRight;
    QWidget *container1, *container2;
    QLabel *life, *gold, *healthImage, *goldImage;
};

#endif // GUI_H
