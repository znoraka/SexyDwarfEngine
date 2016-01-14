#include "gui.h"
#include "engine/game.h"

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


LifeAndGoldWindow::LifeAndGoldWindow(QWidget *parent) : QWidget(parent)
{
    layout = new QHBoxLayout(this);
    life = new QLabel("10000");
    gold = new QLabel("10000");
    healthImage = new QLabel("+");
    goldImage = new QLabel("$");

    container1 = new QWidget();
    container1->setStyleSheet("border:1px solid black;");
    life->setStyleSheet("border:0px");
    healthImage->setStyleSheet("border:0px");
    sublayoutLeft = new QHBoxLayout(container1);
    QPixmap pic("://assets/ui/health.png");
    healthImage->resize(20, 20);
    container1->updateGeometry();
    QPixmap scaled = pic.scaled ( 20, 20, Qt::IgnoreAspectRatio, Qt::FastTransformation );

    healthImage->setPixmap(scaled);
    sublayoutLeft->addWidget(healthImage);
    sublayoutLeft->addWidget(life);

    container2 = new QWidget();
    container2->setStyleSheet("border:1px solid black;");
    gold->setStyleSheet("border:0px");
    goldImage->setStyleSheet("border:0px");
    sublayoutRight = new QHBoxLayout(container2);
    QPixmap pic1("://assets/ui/coin.png");
    goldImage->resize(20, 20);
    container2->updateGeometry();
    QPixmap scaled1 = pic1.scaled ( 20, 20, Qt::IgnoreAspectRatio, Qt::FastTransformation );

    goldImage->setPixmap(scaled1);
    sublayoutRight->addWidget(goldImage);
    sublayoutRight->addWidget(gold);

    layout->addWidget(container1);
    layout->addWidget(container2);
    this->setStyleSheet("background-color:white;");
    this->setVisible(true);
}

void LifeAndGoldWindow::update()
{
    life->setText(QString::number(Player::getInstance()->getLifePoints()));
    gold->setText(QString::number(Player::getInstance()->getGold()));
    move(Game::Graphics::width() * 0.5 - this->width() * 0.5, this->height() * 0.2);
}
