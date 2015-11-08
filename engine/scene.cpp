#include "scene.h"

Scene::Scene()
{
    camera = new Camera();
}

void Scene::initialize()
{
    camera->initialize(WIDTH / HEIGHT, WIDTH, HEIGHT, 0, 1000);
    Entity *e;
    VolumeComponent *v = VolumeComponent::pool->obtain()->init(":/assets/ply/galleon.ply");
    for (int i = 0; i < 10; ++i) {
        e = Entity::pool->obtain();
        e->addComponent(v->clone());
        e->setScale(0.5, 0.5, 0.5);
        e->setPosition(400 + i, 400 + i, -400);
        e->setRotation(-90 + i * 0.5, 0, 90);
        this->addEntity(e);
    }
    v->release();

    qDebug() << e->getComponent(VolumeComponent::name)->componentName();

    this->ready = true;
}


void Scene::addEntity(Entity *entity)
{
    //    foreach (Component *component, entity->getComponents()) {
    //        if(!this->containsSystem(component->systemName())) {
    //            qDebug() << "inserting system" << component->systemName();
    //            this->addSystem(component->instantiateSystem());
    //        }
    //        System *system = systemsHash.find(component->systemName()).value();
    //        system->addComponent(component);
    //    }
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
    glPushMatrix();
    camera->initialize(WIDTH / HEIGHT, WIDTH, HEIGHT, 0, 1000);
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

    switch (event->type())
    {
    case QEvent::MouseMove:
        return true;
    case QEvent::KeyPress:
        keyEvent = static_cast<QKeyEvent*>(event);
        switch (keyEvent->key()) {
            case Qt::Key_Right:
            foreach (Entity *e, entities) {
                QVector3D v = e->getPosition();
                v.setX(v.x() + 10);
                e->setPosition(v);
            }
            return true;

        case Qt::Key_Left:
        foreach (Entity *e, entities) {
            QVector3D v = e->getPosition();
            v.setX(v.x() - 10);
            e->setPosition(v);
        }
        return true;

        case Qt::Key_Up:
        foreach (Entity *e, entities) {
            QVector3D v = e->getPosition();
            v.setY(v.y() + 10);
            e->setPosition(v);
        }
        return true;

        case Qt::Key_Down:
        foreach (Entity *e, entities) {
            QVector3D v = e->getPosition();
            v.setY(v.y() - 10);
            e->setPosition(v);
        }
        return true;
        }
    }
    return false;
}

bool Scene::containsSystem(const QString s) const
{
    //    return systemsHash.find(s) != systemsHash.end();
}

void Scene::addSystem(System *system)
{
    //    systemsHash.insert(system->systemName(), system);
}
