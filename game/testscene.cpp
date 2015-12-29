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
    camera->setPosition(0, 0, 0);
    camera->setRotation(54, -90, 0);
    camera->setScale(camera->getScale() * 0.5);

    map = Entity::pool->obtain()->
            addComponent(MapComponent::pool->obtain()->init(":/assets/maps/map1/"))->
            setPosition(0, 0, 0)->
            setScale(1, 1, 1)->
            setRotation(-90, 0, 90);
    this->addEntity(map);

    Entity *e;
    VolumeComponent *v = VolumeComponent::pool->obtain()->init(":/assets/ply/galleon.ply");
    PathFollowerComponent *p = PathFollowerComponent::pool->
            obtain()->
            init(":/assets/maps/map1/",
                 static_cast<MapComponent*>(map->getComponent(MapComponent::name)),
                 0.1);

    for (int i = 0; i < 1; ++i) {
        e = Entity::pool->obtain();
        e->addComponent(v->clone());
        e->addComponent(p->clone());
        e->setScale(0.05, 0.05, 0.05);
        e->setPosition(16, 416, 0);
        e->setRotation(0, 0, 90);
        map->addChild(e);
//        this->addEntity(e);
    }

    map->addChild(Entity::pool->obtain()->
                    addComponent(VolumeComponent::pool->obtain()->init(":/assets/ply/tower.ply"))->
                    addComponent(TowerComponent::pool->obtain()->init(QVector3D(0, 30, 0), e))->
                    setScale(10, 10, 10)->
                    setRotation(90, 0, 0)->
                    setPosition(345, 350, 35));

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[] = { 0, 500, 0, 0.0f };

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
}

void TestScene::onAddTowerButtonClicked()
{
    towerGhost = Entity::pool->obtain()->
                    addComponent(VolumeComponent::pool->obtain()->init(":/assets/ply/tower.ply"))->
                    setScale(10, 10, 10);
    this->addEntity(towerGhost);
}

bool TestScene::handleEvent(QEvent *event)
{
    QMouseEvent *mouseEvent;
    QKeyEvent *keyEvent;
    QVector3D v;

    switch (event->type())
    {
    case QEvent::MouseMove:
        mouseEvent = static_cast<QMouseEvent*>(event);

        if(towerGhost != nullptr) {
            v = camera->screenToWorld(QVector3D(mouseEvent->x(), 0, mouseEvent->y()));
            v.setY(static_cast<MapComponent*>(map->getComponent(MapComponent::name))->getZ(v.x(), v.z()));
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
            qDebug() << camera->getPosition();
            v = camera->getPosition();
            v.setZ(v.z() + 5);
            camera->setPosition(v);
            qDebug() << camera->getPosition();
            qDebug() << "---------";
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
    }
    return false;
}
