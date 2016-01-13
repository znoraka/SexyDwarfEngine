#include "gui.h"

Gui::Gui()
{

}

TowerInfoUpgradeWindow::TowerInfoUpgradeWindow(QWidget *parent) : QWidget(parent)
{
    vlayout = new QVBoxLayout(this);
    container = new QWidget();
    hlayout = new QHBoxLayout(container);

    vlayout->addWidget(container);

    upgradeDamage = new QPushButton("Damage");
    upgradeSpeed = new QPushButton("Speed");

    hlayout->addWidget(upgradeDamage);
    hlayout->addWidget(upgradeSpeed);

    damagePrice = new QLabel();
    speedPrice = new QLabel();
    damage = new QLabel();
    speed = new QLabel();
    range = new QLabel();

    vlayout->addWidget(damagePrice);
    vlayout->addWidget(speedPrice);
    vlayout->addWidget(damage);
    vlayout->addWidget(speed);
    vlayout->addWidget(range);

    this->setStyleSheet("background-color:white;");

    this->setVisible(false);
}

void TowerInfoUpgradeWindow::show(int x, int y, Entity *tower)
{
    this->lastEntity = tower;
    this->update();
    this->setVisible(true);
    this->move(x, y);
}

void TowerInfoUpgradeWindow::hide()
{
    this->setVisible(false);
}

void TowerInfoUpgradeWindow::update()
{
    TowerComponent *t = lastEntity->getComponent<TowerComponent>();
    upgradeDamage->setEnabled(t->getDamageUpgradePrice() < Player::getInstance()->getGold());
    upgradeSpeed->setEnabled(t->getSpeedUpgradePrice() < Player::getInstance()->getGold());
    damagePrice->setText(QString() + "Damage upgrade = " + QString::number(t->getDamageUpgradePrice()) + "$");
    speedPrice->setText(QString() + "Speed upgrade = " + QString::number(t->getSpeedUpgradePrice()) + "$");
    damage->setText(QString() + "Damage = " + QString::number(t->getDamage()));
    speed->setText(QString() + "Speed = " + QString::number(t->getSpeed()));
    range->setText(QString() + "Range = " + QString::number(t->getRange()));
}

QPushButton *TowerInfoUpgradeWindow::getDamageUpgradeButton()
{
    return this->upgradeDamage;
}

QPushButton *TowerInfoUpgradeWindow::getSpeedUpgrageButton()
{
    return this->upgradeSpeed;
}

