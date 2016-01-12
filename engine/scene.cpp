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

    while(!callbacks.empty()) {
        auto f = callbacks.dequeue();
        f();
    }

    foreach (Entity *e, entities) {
        if(!e->hasParent()) {
            e->update(delta);
        }
    }
}

void Scene::addCallBack(std::function<void()> f)
{
    this->callbacks.enqueue(f);
}

Entity *Scene::clicked(Qt::MouseButton button, int x, int y)
{
    foreach (Entity *e, entities) {
        Entity *e1 = e->clicked(button, x, y);
        if(e1 != nullptr) return e1;
    }
    return nullptr;
}


bool Scene::handleEvent(QEvent *event)
{
}
