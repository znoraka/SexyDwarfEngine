#include "scene.h"

Scene::Scene()
{
    camera = new Camera();
}

void Scene::initialize()
{
    camera->initialize(WIDTH / HEIGHT, WIDTH, HEIGHT, -1000, 1000);
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
    camera->update(delta);

//    glLoadIdentity();
//    glOrtho(0.0, width * ratio, 0.0, height * ratio, near, far);

    foreach (Entity *e, entities) {
        e->update(delta);
    }
    glPopMatrix();

}


bool Scene::handleEvent(QEvent *event)
{

}
