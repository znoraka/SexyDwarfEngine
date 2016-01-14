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
    wave = new QLabel("10000");
    healthImage = new QLabel("+");
    goldImage = new QLabel("$");
    waveImage = new QLabel("w");

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

    container3 = new QWidget();
    container3->setStyleSheet("border:1px solid black;");
    wave->setStyleSheet("border:0px");
    waveImage->setStyleSheet("border:0px");
    sublayoutRight2 = new QHBoxLayout(container3);
    QPixmap pic2("://assets/ui/wave.png");
    waveImage->resize(20, 20);
    container3->updateGeometry();
    QPixmap scaled2 = pic2.scaled ( 20, 20, Qt::IgnoreAspectRatio, Qt::FastTransformation );

    waveImage->setPixmap(scaled2);
    sublayoutRight2->addWidget(waveImage);
    sublayoutRight2->addWidget(wave);


    layout->addWidget(container1);
    layout->addWidget(container2);
    layout->addWidget(container3);
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
    wave->setText(QString::number(Player::getInstance()->getWave()));
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

TowerButtons::TowerButtons(QWidget *parent) : QWidget(parent)
{
    auto createButton = [=] (QString text, QSignalMapper *mapper) {
        QPushButton *p = new QPushButton(text);
        p->setFixedSize(69, 69);
        p->setFocusPolicy(Qt::NoFocus);
        layout->addWidget(p);
        connect(p, SIGNAL(clicked(bool)), mapper, SLOT(map()));
        return p;
    };

    layout = new QHBoxLayout(this);
    poisonMapper = new QSignalMapper();
    fireMapper = new QSignalMapper();
    iceMapper = new QSignalMapper();
    lightningMapper = new QSignalMapper();

    poison = createButton("Poison", poisonMapper);
    fire = createButton("Fire", fireMapper);
    ice = createButton("Ice", iceMapper);
    lightning = createButton("Lightning", lightningMapper);

//    QPushButton *button = new QPushButton();
//    QSignalMapper *mapper = new QSignalMapper(this);
//    int n = type;
//    towersIconsLayout->addWidget(button);
//    button->setText(bgImage.split("/").last().split(".").first());
//    //    button->setStyleSheet(QString() + "background-image:url(" + bgImage + ");");
//    button->setFixedSize(69, 69);
//    towersIconsLayout->setMargin(0);
    //    //    button->setDisabled(towersIconsLayout->count() > 2);
}

void TowerButtons::update()
{
    this->setFixedSize(Game::Graphics::width() * 0.17, Game::Graphics::width() * 0.1);
    this->move(Game::Graphics::width() * 0.5 - this->width() * 0.5, Game::Graphics::height() - this->height() * 0.8);

    poison->setEnabled(poisonPrice < Player::getInstance()->getGold());
    fire->setEnabled(firePrice < Player::getInstance()->getGold());
    ice->setEnabled(icePrice < Player::getInstance()->getGold());
    lightning->setEnabled(lightningPrice < Player::getInstance()->getGold());
}

void TowerButtons::connectPoisonButton(TowerComponent::TowerType type, int price, TestScene *scene)
{
    poisonMapper->setMapping(poison, type);
    poisonPrice = price;
    connect(poisonMapper, SIGNAL(mapped(int)), scene, SLOT(onAddTowerButtonClickedInt(int)));
}

void TowerButtons::connectFireButton(TowerComponent::TowerType type, int price, TestScene *scene)
{
    fireMapper->setMapping(fire, type);
    firePrice = price;
    connect(fireMapper, SIGNAL(mapped(int)), scene, SLOT(onAddTowerButtonClickedInt(int)));
}

void TowerButtons::connectIceButton(TowerComponent::TowerType type, int price, TestScene *scene)
{
    iceMapper->setMapping(ice, type);
    icePrice = price;
    connect(iceMapper, SIGNAL(mapped(int)), scene, SLOT(onAddTowerButtonClickedInt(int)));
}

void TowerButtons::connectLightningButton(TowerComponent::TowerType type, int price, TestScene *scene)
{
    lightningMapper->setMapping(lightning, type);
    lightningPrice = price;
    connect(lightningMapper, SIGNAL(mapped(int)), scene, SLOT(onAddTowerButtonClickedInt(int)));
}
