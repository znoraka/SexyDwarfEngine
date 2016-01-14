#include "gui.h"
#include "engine/game.h"
#include "testscene.h"

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

TowerInfoUpgradeWindow::~TowerInfoUpgradeWindow()
{
    delete vlayout;
    delete hlayout;
    delete upgradeDamage;
    delete upgradeSpeed;
    delete damagePrice;
    delete speedPrice;
    delete damage;
    delete range;
    delete speed;
    delete container;
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

LifeAndGoldWindow::~LifeAndGoldWindow()
{
    delete layout;
    delete sublayoutLeft;
    delete sublayoutRight;
    delete container1;
    delete container2;
//    delete life;
//    delete gold;
//    delete healthImage;
//    delete goldImage;
}

void LifeAndGoldWindow::update()
{
    life->setText(QString::number(Player::getInstance()->getLifePoints()));
    gold->setText(QString::number(Player::getInstance()->getGold()));
    move(Game::Graphics::width() * 0.5 - this->width() * 0.5, this->height() * 0.2);
}

YouLoseWindow::YouLoseWindow(QWidget *parent) : QWidget(parent)
{
    this->setVisible(false);
    youlose = new QLabel("Perdu !");
    youlose->setAlignment(Qt::AlignCenter);
//    youlose->setStyleSheet("font:136pt;background-color:rgba(20, 20, 20, 0);");
//    this->setStyleSheet("background-color:rgba(120, 20, 20, 0)");

    container = new QWidget();
    container->setStyleSheet("background-color:rgba(20, 20, 20, 0)");
    restart = new QPushButton("Recommencer");
    quit = new QPushButton("Quitter");
    restart->setStyleSheet("background-color: white; font:30pt");
    quit->setStyleSheet("background-color: white; font:30pt");
    hlayout = new QHBoxLayout(container);
    hlayout->addWidget(restart);
    hlayout->addWidget(quit);

    layout = new QVBoxLayout(this);
    layout->addWidget(youlose);
    layout->addWidget(container);
    this->opacity = 0;

    connect(restart, SIGNAL(clicked(bool)), this, SLOT(onRestartClick()));
    connect(quit, SIGNAL(clicked(bool)), this, SLOT(onQuitClick()));
}

void YouLoseWindow::show(float delta)
{
    this->setVisible(true);
    opacity += delta * 0.1;
    if(opacity > 150) {
        opacity = 150;
    }
    restart->setMaximumWidth(Game::Graphics::width() * 0.2);
    restart->setMinimumHeight(Game::Graphics::height() * 0.1);
    quit->setMaximumWidth(Game::Graphics::width() * 0.2);
    quit->setMinimumHeight(Game::Graphics::height() * 0.1);
    youlose->setStyleSheet("font:136pt;background-color:rgba(20, 20, 20, 0)");
    this->setStyleSheet("background:rgba(120, 20, 20, " + QString::number(opacity) + ")");
    this->resize(Game::Graphics::width(), Game::Graphics::height());
    this->move(Game::Graphics::width() * 0.5 - this->width() * 0.5, Game::Graphics::height() * 0.5 - this->height() * 0.5);
}

void YouLoseWindow::onRestartClick()
{
    Game::getInstance()->setScene(new TestScene());
}

void YouLoseWindow::onQuitClick()
{
    Game::exit();
}

void YouLoseWindow::paintEvent(QPaintEvent *pe)
{
  QStyleOption o;
  o.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(
    QStyle::PE_Widget, &o, &p, this);
}
