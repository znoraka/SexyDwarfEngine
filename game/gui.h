#ifndef GUI_H
#define GUI_H

#include <QCursor>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QStyleOption>

#include "engine/entity.h"
#include "game/towercomponent.h"

class TestScene;

class Gui
{
public:
    Gui();
};

class TowerInfoUpgradeWindow : public QWidget
{
public :
    TowerInfoUpgradeWindow(QWidget *parent = 0);
    ~TowerInfoUpgradeWindow();

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
    ~LifeAndGoldWindow();
    void update();

private:
    QHBoxLayout *layout, *sublayoutLeft, *sublayoutRight, *sublayoutRight2;
    QWidget *container1, *container2, *container3;
    QLabel *life, *gold, *healthImage, *goldImage, *wave, *waveImage;
};

class YouLoseWindow : public QWidget
{
    Q_OBJECT
public:
    YouLoseWindow(QWidget *parent = 0);
    void show(float delta);
    void paintEvent(QPaintEvent *pe);
public slots:
    void onRestartClick();
    void onQuitClick();

private:
    QLabel *youlose;
    QVBoxLayout *layout;
    QHBoxLayout *hlayout;
    QWidget *container;
    QLabel *text;
    QPushButton *restart, *quit;

    float opacity;
};

#endif // GUI_H
