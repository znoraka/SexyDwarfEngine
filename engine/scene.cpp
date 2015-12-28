#include "scene.h"

Scene::Scene()
{
    camera = new Camera();
}

void Scene::initialize(QMainWindow *parent)
{
    this->setParent(parent);

    camera->initialize(WIDTH / HEIGHT, WIDTH, HEIGHT, 0, 1000);
    QVector3D pos = camera->getPosition();
//    pos.setX(WIDTH * 0.5); pos.setY(HEIGHT * 0.5); pos.setZ(0);
//    camera->setPosition(WIDTH * 0.5, HEIGHT * 0.5, 0);
    camera->setPosition(0, 0, 0);
    camera->setRotation(54, -90, 0);
    camera->setScale(camera->getScale() * 1.8);
    Entity *e;
    VolumeComponent *v = VolumeComponent::pool->obtain()->init(":/assets/ply/galleon.ply");
    PathFollowerComponent *p = PathFollowerComponent::pool->obtain()->init(":/assets/maps/map1/");
    for (int i = 0; i < 1; ++i) {
        e = Entity::pool->obtain();
        e->addComponent(v->clone());
        e->addComponent(p);
        e->setScale(0.05, 0.05, 0.05);
        e->setPosition(-512 + 96, 0, -16);
        e->setRotation(-90, 0, 180);
        this->addEntity(e);
    }

    this->addEntity(Entity::pool->obtain()->
                    addComponent(VolumeComponent::pool->obtain()->init(":/assets/ply/tower.ply"))->
                    addComponent(TowerComponent::pool->obtain()->init(QVector3D(0, 30, 0), e))->
                    setScale(10, 10, 10)->
                    setPosition(-350, 34.5, -345));


    this->addEntity(Entity::pool->obtain()->
                    addComponent(MapComponent::pool->obtain()->init(":/assets/maps/map1/"))->
                    setPosition(0, 0, 0)->
                    setScale(1, 1, 1)->
                    setRotation(-90, 0, 90));

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[] = { 50, 50, 500, 0.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    this->ready = true;
}

void Scene::initializeGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_CULL_FACE);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,  GL_FASTEST);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
}

void Scene::addEntity(Entity *entity)
{
    this->entities.insert(entity);
}

void Scene::removeEntity(Entity *entity)
{
    entity->release();
    this->entities.remove(entity);
}

bool Scene::isReady() const
{
    return this->ready;
}

void Scene::update(float delta)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glPushMatrix();
    camera->initialize(WIDTH / HEIGHT, WIDTH, HEIGHT, -1000, 1000);
    camera->update(delta);

    foreach (Entity *e, entities) {
        e->update(delta);
    }
    glPopMatrix();

}


bool Scene::handleEvent(QEvent *event)
{
    QMouseEvent *mouseEvent;
    QKeyEvent *keyEvent;
    QVector3D v;

    switch (event->type())
    {
    case QEvent::MouseMove:
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
            v.setZ(v.z() + 1);
            camera->setPosition(v);
            return true;

        case Qt::Key_U:
            v = camera->getPosition();
            v.setZ(v.z() - 1);
            camera->setPosition(v);
            return true;

        case Qt::Key_A:
            v = camera->getPosition();
            v.setX(v.x() - 1);
            camera->setPosition(v);
            return true;

        case Qt::Key_I:
            v = camera->getPosition();
            v.setX(v.x() + 1);
            camera->setPosition(v);
            return true;

        case Qt::Key_Space:
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
