#include "testscene.h"

TestScene::TestScene()
{

}

void TestScene::initialize()
{
    FMODManager::getInstance()->loadBank(":/assets/sounds/Master Bank.bank");
    FMODManager::getInstance()->loadBank(":/assets/sounds/Master Bank.strings.bank");
    FMODManager::getInstance()->loadBank(":/assets/sounds/bank.bank");

    FMODManager::getInstance()->set3DSettings(1, 1, 5);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_CULL_FACE);
    glEnable (GL_BLEND);
    glEnable(GL_POLYGON_OFFSET_LINE);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,  GL_FASTEST);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    camera->initialize(WIDTH / HEIGHT, WIDTH, HEIGHT, -1000, 1000);
    QVector3D pos = camera->getPosition();

    camera->setPosition(200, 0, 0);
    camera->setRotation(41, -90, 0);
    camera->setScale(camera->getScale() * 1.3);

    map = Entity::pool->obtain()->
            addComponent(MapComponent::pool->obtain()->init(":/assets/maps/map2/"))->
            setPosition(0, 0, 0)->
            setScale(1, 1, 1)->
            setRotation(-90, 0, 100);
    this->addEntity(map);

    MapComponent *mapComponent = static_cast<MapComponent*>(map->getComponent(MapComponent::name));
    towerComponents.insert(
                TowerComponent::TowerType::BULLET,
                TowerComponent::pool->obtain()->init(QVector3D(0, 0, 30), mapComponent->getEnemies(), 150, 3, 1, 50, TowerComponent::TowerType::BULLET));
    towerComponents.insert(
                TowerComponent::TowerType::FIRE,
                TowerComponent::pool->obtain()->init(QVector3D(0, 0, 30), mapComponent->getEnemies(), 200, 5, 2, 100, TowerComponent::TowerType::FIRE));
    towerComponents.insert(
                TowerComponent::TowerType::ICE,
                TowerComponent::pool->obtain()->init(QVector3D(0, 0, 30), mapComponent->getEnemies(), 100, 6, 3, 200, TowerComponent::TowerType::ICE));
    towerComponents.insert(
                TowerComponent::TowerType::LIGHTNING,
                TowerComponent::pool->obtain()->init(QVector3D(0, 0, 30), mapComponent->getEnemies(), 350, 10, 2, 350, TowerComponent::TowerType::LIGHTNING));

    towerGhostComponent = TowerGhostComponent::pool->obtain()->init(":/assets/maps/map2/");
    towerVolume = VolumeComponent::pool->obtain()->init(":/assets/ply/tower.ply");
    enemyComponent = EnemyComponent::pool->obtain()->init(0, 25, 0);

    map->addChild(Entity::pool->obtain()->
                  addComponent(VolumeComponent::pool->obtain()->init(":/assets/ply/church.ply"))->
                  setPosition(16, 170, 0)->
                  setRotation(90, 0, 0)->
                  setScale(10, 10, 10));

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[] = { 0, 800, 600, 0.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    QWidget *container = new QWidget(this);
    towersIconsLayout = new QHBoxLayout(container);
    createUiButton(":/assets/ui/poison.png", TowerComponent::TowerType::BULLET);
    createUiButton(":/assets/ui/fire.png", TowerComponent::TowerType::FIRE);
    createUiButton(":/assets/ui/ice.png", TowerComponent::TowerType::ICE);
    createUiButton(":/assets/ui/lightning.png", TowerComponent::TowerType::LIGHTNING);
    container->resize(towersIconsLayout->count() * 75, 69);
    container->move(WIDTH * 0.5 - container->width() * 0.5,
                          HEIGHT - container->height() * 1.2);

    upgradeTowersContainer = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(upgradeTowersContainer);

    QPushButton *button = new QPushButton();
    connect(button, SIGNAL(clicked(bool)), this, SLOT(upgradeDamageClicked()));
    layout->addWidget(button);

    button = new QPushButton();
    connect(button, SIGNAL(clicked(bool)), this, SLOT(upgradeSpeedClicked()));
    layout->addWidget(button);

    upgradeTowersContainer->setVisible(false);

    lifeLabel = new QLabel(this);
    lifeLabel->resize(100, 50);
    lifeLabel->move(20, 20);

    goldLabel = new QLabel(this);
    goldLabel->resize(100, 50);
    goldLabel->move(20, 50);

    Player::getInstance()->earnGold(175);

    towerGhost = nullptr;

    this->ready = true;
    this->setMouseTracking(true);

    dummy = Entity::pool->obtain();
    map->addChild(dummy);

    Player::getInstance()->setMaxLifePoints(10);
    Player::getInstance()->heal(10);

    FMODManager::getInstance()->setCurrentMusic("event:/musique");
    FMODManager::getInstance()->startCurrentMusic();
//    FMODManager::getInstance()->setCurrentMusicVolume(0.5);
    FMODManager::getInstance()->setCurrentMusicVolume(0);
}

void TestScene::update(float delta) {
    QVector3D v = camera->screenToWorld(
                QVector3D(WIDTH * 0.5, HEIGHT * 0.5, 0),
                dummy->getModelViewMatrix(), dummy->getProjectionMatrix());

    v.setX(v.x() / 768);
    v.setY(v.y() / 624);
    FMODManager::getInstance()->setListenerPosition(v);

    FMODManager::getInstance()->setCurrentMusicParameterValue("life", pow(Player::getInstance()->getMissingLifePercentage(), 2));

    if(towerGhost != nullptr) {

        QVector3D v = camera->screenToWorld(
                    QVector3D(mouseX, mouseY, towerGhost->getPosition().z()),
                    towerGhost->getModelViewMatrix(), towerGhost->getProjectionMatrix());

        towerGhost->setPosition(v);
        v = towerGhost->getLocalPosition();
        v.setZ(static_cast<MapComponent*>(map->getComponent(MapComponent::name))->getZ(v.x(), v.y()));

        towerGhost->setPosition(v);
    }

    lifeLabel->setText(QString() + "life = " + QString::number(Player::getInstance()->getLifePoints()));
    goldLabel->setText(QString() + "golds = " + QString::number(Player::getInstance()->getGold()));

    QVector3D cam = camera->getPosition();

    if(mouseX > WIDTH * 0.95) {
        cam.setX(cam.x() - 10);
    }

    if(mouseX < WIDTH * 0.05) {
        cam.setX(cam.x() + 10);
    }

    if(mouseY > HEIGHT * 0.95) {
        cam.setY(cam.y() + 10);
    }

    if(mouseY < HEIGHT * 0.05) {
        cam.setY(cam.y() - 10);
    }

    camera->setPosition(cam);

    this->lockCursorInsideWindow();

    Scene::update(delta);
}

void TestScene::onAddTowerButtonClicked(TowerComponent::TowerType type)
{
    qDebug() << "type = " << type;

    if(towerComponents[type]->getPrice() <= Player::getInstance()->getGold() && towerGhost == nullptr) {
        towerGhost = Entity::pool->obtain()->
                addComponent(towerVolume->clone())->
                addComponent(towerGhostComponent->clone())->
                addComponent(towerComponents[type]->clone())->
                setScale(10, 10, 10)->
                setRotation(90, 0, 0);
        map->addChild(towerGhost);
    }
}

void TestScene::onAddTowerButtonClickedInt(int type)
{
    onAddTowerButtonClicked(static_cast<TowerComponent::TowerType>(type));
}

void TestScene::upgradeDamageClicked()
{
    TowerComponent *t = static_cast<TowerComponent*>(clickedEntity->getComponent(TowerComponent::name));
    if(t->getDamageUpgradePrice() < Player::getInstance()->getGold()) {
        Player::getInstance()->spendGold(t->getDamageUpgradePrice());
        t->upgradeDamage();
        upgradeTowersContainer->setVisible(false);
    }
}

void TestScene::upgradeSpeedClicked()
{
    TowerComponent *t = static_cast<TowerComponent*>(clickedEntity->getComponent(TowerComponent::name));
    if(t->getSpeedUpgradePrice() < Player::getInstance()->getGold()) {
        Player::getInstance()->spendGold(t->getSpeedUpgradePrice());
        t->upgradeSpeed();
        upgradeTowersContainer->setVisible(false);
    }
}

bool TestScene::handleEvent(QEvent *event)
{
    QMouseEvent *mouseEvent;
    QKeyEvent *keyEvent;
    QVector3D v, v1, v2;

    switch (event->type())
    {
    case QEvent::MouseButtonRelease:
        mouseEvent = static_cast<QMouseEvent*>(event);
        if(mouseEvent->button() == Qt::LeftButton) {
            if(towerGhost != nullptr) {
                FMODManager::getInstance()->setCurrentEvent("event:/build");
                FMODManager::getInstance()->setEventInstanceVolume(2);
                FMODManager::getInstance()->setEventInstancePosition(v);
                FMODManager::getInstance()->startEventInstance();
                if(static_cast<TowerGhostComponent*>(towerGhost->getComponent(TowerGhostComponent::name))->hasRoom()) {
                    Player::getInstance()->spendGold(static_cast<TowerComponent*>(towerGhost->getComponent(TowerComponent::name))->getPrice());

                    //                v = towerGhost->getLocalPosition();
                    //                int nx = v.x() / TILE_SIZE;
                    //                v.setX(TILE_SIZE * nx);

                    //                int ny = v.y() / TILE_SIZE;
                    //                v.setY(TILE_SIZE * ny);
                    //                towerGhost->setPosition(v);

                    static_cast<TowerComponent*>(towerGhost->getComponent(TowerComponent::name))->setReady();
                    towerGhost->removeComponent(TowerGhostComponent::name);
                    towerGhost = nullptr;
                }
            } else {
                clickedEntity = Scene::clicked(mouseEvent->button(), mouseX, mouseY);
                if(clickedEntity != nullptr) {
                    if(clickedEntity->getComponent(TowerComponent::name) != nullptr) {
                        upgradeTowersContainer->move(mouseX, mouseY);
                        upgradeTowersContainer->setVisible(true);
                    }
                }
            }
        } else if (mouseEvent->button() == Qt::RightButton) {
            if(towerGhost != nullptr) {
                towerGhost->release();
                towerGhost = nullptr;
            }
            clickedEntity = nullptr;
            upgradeTowersContainer->setVisible(false);
        }
        return true;
    case QEvent::MouseMove:
        mouseEvent = static_cast<QMouseEvent*>(event);
        mouseX = mouseEvent->x();
        mouseY = mouseEvent->y();

        return true;
    case QEvent::KeyPress:
        keyEvent = static_cast<QKeyEvent*>(event);
        switch (keyEvent->key()) {
        case Qt::Key_Right:
            v = camera->getRotation();
            v.setY(v.y() + 1);
            camera->setRotation(v);
            return true;

        case Qt::Key_Left:
            v = camera->getRotation();
            v.setY(v.y() - 1);
            camera->setRotation(v);
            return true;

        case Qt::Key_Up:
            v = camera->getRotation();
            v.setX(v.x() + 1);
            camera->setRotation(v);
            return true;

        case Qt::Key_Down:
            v = camera->getRotation();
            v.setX(v.x() - 1);
            camera->setRotation(v);
            return true;

        case Qt::Key_J:
            v = camera->getScale();
            v.setX(v.x() + 0.1);
            v.setY(v.y() + 0.1);
            v.setZ(v.z() + 0.1);
            camera->setScale(v);
            return true;

        case Qt::Key_L:
            v = camera->getScale();
            v.setX(v.x() - 0.1);
            v.setY(v.y() - 0.1);
            v.setZ(v.z() - 0.1);
            camera->setScale(v);
            return true;

//        case Qt::Key_Eacute:
//            v = camera->getPosition();
//            v.setY(v.y() + 5);
//            camera->setPosition(v);
//            return true;

//        case Qt::Key_U:
//            v = camera->getPosition();
//            v.setY(v.y() - 5);
//            camera->setPosition(v);
//            return true;

//        case Qt::Key_A:
//            v = camera->getPosition();
//            v.setX(v.x() - 5);
//            camera->setPosition(v);
//            return true;

//        case Qt::Key_I:
//            v = camera->getPosition();
//            v.setX(v.x() + 5);
//            camera->setPosition(v);
//            return true;

        case Qt::Key_B:
            onAddTowerButtonClicked(TowerComponent::TowerType::BULLET);
            return true;

        case Qt::Key_Eacute:
            onAddTowerButtonClicked(TowerComponent::TowerType::FIRE);
            return true;

        case Qt::Key_P:
            onAddTowerButtonClicked(TowerComponent::TowerType::ICE);
            return true;

        case Qt::Key_O:
            onAddTowerButtonClicked(TowerComponent::TowerType::LIGHTNING);
            return true;

        case Qt::Key_Space:
            makeCurrent();
            if(displayLines) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            displayLines = !displayLines;

            qDebug() << "center pos = " << camera->screenToWorld(
                            QVector3D(WIDTH * 0.5, HEIGHT * 0.5, 0),
                            dummy->getModelViewMatrix(), dummy->getProjectionMatrix());
            return true;

        }
    default: return false;
    }
    return false;
}

void TestScene::lockCursorInsideWindow()
{
    QPoint p(QCursor::pos() - mapToGlobal(this->pos()));


    if(p.x() > WIDTH) {
        p.setX(WIDTH);
    }

    if(p.x() < 0) {
        p.setX(0);
    }

    if(p.y() > HEIGHT) {
        p.setY(HEIGHT);
    }

    if(p.y() < 0) {
        p.setY(0);
    }

    QCursor::setPos(p + mapToGlobal(this->pos()));

}

void TestScene::createUiButton(QString bgImage, TowerComponent::TowerType type)
{
    QPushButton *button = new QPushButton();
    QSignalMapper *mapper = new QSignalMapper(this);
    int n = type;
    mapper->setMapping(button, type);
    towersIconsLayout->addWidget(button);
    button->setFocusPolicy(Qt::NoFocus);
    button->setText(bgImage.split("/").last().split(".").first());
//    button->setStyleSheet(QString() + "background-image:url(" + bgImage + ");");
    button->setFixedSize(69, 69);
    connect(button, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(mapper, SIGNAL(mapped(int)), this, SLOT(onAddTowerButtonClickedInt(int)));
    towersIconsLayout->setMargin(0);
//    button->setDisabled(towersIconsLayout->count() > 2);
}
