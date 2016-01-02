#include "testscene.h"

TestScene::TestScene()
{

}

void TestScene::initialize()
{
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
    //    pos.setX(WIDTH * 0.5); pos.setY(HEIGHT * 0.5); pos.setZ(0);
    //    camera->setPosition(WIDTH * 0.1, 0, 0);
    camera->setPosition(200, 0, 0);
    camera->setRotation(54, -90, 0);
    camera->setScale(camera->getScale() * 1);

    enemies = new QList<Entity*>();

    map = Entity::pool->obtain()->
            addComponent(MapComponent::pool->obtain()->init(":/assets/maps/map1/"))->
            setPosition(0, 0, 0)->
            setScale(1, 1, 1)->
            setRotation(-90, 0, 100);
    this->addEntity(map);

    Entity *e;
    VolumeComponent *v = VolumeComponent::pool->obtain()->init(":/assets/ply/galleon.ply");
    towerComponent = TowerComponent::pool->obtain()->init(QVector3D(0, 0, 30), enemies, 150, 3);
    towerGhostComponent = TowerGhostComponent::pool->obtain()->init(":/assets/maps/map1/");
    towerVolume = VolumeComponent::pool->obtain()->init(":/assets/ply/tower.ply");
    enemyComponent = EnemyComponent::pool->obtain()->init(0, 10, 0);
    PathFollowerComponent *p = PathFollowerComponent::pool->
            obtain()->
            init(":/assets/maps/map1/",
                 static_cast<MapComponent*>(map->getComponent(MapComponent::name)),
                 0.1);

    for (int i = 0; i < 200; ++i) {
        e = Entity::pool->obtain();
        e->addComponent(v->clone());
        e->addComponent(enemyComponent->clone());
        e->addComponent(p->clone()->setSpeed((qrand() % 100) * 0.001));
        e->setScale(0.05, 0.05, 0.05);
        e->setPosition(16, 416, 0);
        e->setRotation(0, 0, 90);
        map->addChild(e);
        enemies->append(e);
    }

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

    slowTowerButton = new QPushButton(this);
    slowTowerButton->move(WIDTH * 0.5 - slowTowerButton->width() * 0.5,
                          HEIGHT - slowTowerButton->height() * 2);
    slowTowerButton->setFocusPolicy(Qt::NoFocus);

    connect(slowTowerButton, SIGNAL(clicked(bool)), this, SLOT(onAddTowerButtonClicked()));

    towerGhost = nullptr;

    this->ready = true;
    this->setMouseTracking(true);
}

void TestScene::update(float delta) {

    for(auto i : *enemies) {
        if(i->getParent() == nullptr) {
            enemies->removeAt(enemies->indexOf(i));
        }
    }

    Scene::update(delta);
}

void TestScene::onAddTowerButtonClicked()
{
    if(towerGhost == nullptr) {
//        this->addCallBack([=]() {
            towerGhost = Entity::pool->obtain()->
                    addComponent(towerVolume->clone())->
                    addComponent(towerGhostComponent->clone())->
                    addComponent(towerComponent->clone())->
                    setScale(10, 10, 10)->
                    setRotation(90, 0, 0);
            map->addChild(towerGhost);
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
                if(static_cast<TowerGhostComponent*>(towerGhost->getComponent(TowerGhostComponent::name))->hasRoom()) {

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
            }
        } else if (mouseEvent->button() == Qt::RightButton) {
            if(towerGhost != nullptr) {
//                map->removeChild(towerGhost);
                towerGhost->release();
                towerGhost = nullptr;
            }
        }
        return true;
    case QEvent::MouseMove:
        mouseEvent = static_cast<QMouseEvent*>(event);

        if(towerGhost != nullptr) {
            v = camera->screenToWorld(
                        QVector3D(mouseEvent->x(), mouseEvent->y(), 0),
                        towerGhost->getModelViewMatrix(), towerGhost->getProjectionMatrix(),
                        v1, v2);

            towerGhost->setPosition(v);
            v = towerGhost->getLocalPosition();
            v.setZ(static_cast<MapComponent*>(map->getComponent(MapComponent::name))->getZ(v.x(), v.y()));

            towerGhost->setPosition(v);
        }
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

        case Qt::Key_Eacute:
            v = camera->getPosition();
            v.setY(v.y() + 5);
            camera->setPosition(v);
            return true;

        case Qt::Key_U:
            v = camera->getPosition();
            v.setY(v.y() - 5);
            camera->setPosition(v);
            return true;

        case Qt::Key_A:
            v = camera->getPosition();
            v.setX(v.x() - 5);
            camera->setPosition(v);
            return true;

        case Qt::Key_I:
            v = camera->getPosition();
            v.setX(v.x() + 5);
            camera->setPosition(v);
            return true;

        case Qt::Key_E:
            onAddTowerButtonClicked();
            return true;

        case Qt::Key_Space:
            makeCurrent();
            if(displayLines) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            displayLines = !displayLines;
            return true;

        }
    default: return false;
    }
    return false;
}
