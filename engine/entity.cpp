#include "entity.h"

Pool<Entity *> *Entity::pool = new Pool<Entity*>([] () {return new Entity();});

Entity::Entity()
{

}

Entity *Entity::addComponent(Component *component)
{
    removeComponent(component->componentName());
    component->setEntity(this);
    components[component->componentName()] = component;
    return this;
}

void Entity::removeComponent(const QString componentName)
{
    QHash<QString, Component*>::iterator i = components.find(componentName);
    if(i != components.end()) {
        i.value()->release();
    }
}

Component *Entity::getComponent(const QString componentId)
{
    return components[componentId];
}

QVector<Component *> Entity::getComponents()
{
    QVector<Component *> v;
    foreach (Component *c, components) {
        v.push_back(c);
    }
    return v;
}

void Entity::release()
{
    foreach (Component *var, components) {
        var->release();
    }
    pool->release(this);
}

void Entity::update(float delta)
{
    foreach (Component *c, components) {
        c->update(delta);
    }
}

QVector3D Entity::getPosition() const
{
    return this->position;
}

QVector3D Entity::getRotation() const
{
    return this->rotation;
}

QVector3D Entity::getScale() const
{
    return this->scale;
}

Entity *Entity::setRotation(QVector3D v)
{
    this->rotation = v;
    return this;
}

Entity *Entity::setRotation(float x, float y, float z)
{
    this->rotation.setX(x);
    this->rotation.setY(y);
    this->rotation.setZ(z);
    return this;
}

Entity *Entity::setPosition(QVector3D v)
{
    this->position = v;
    return this;
}

Entity *Entity::setPosition(float x, float y, float z)
{
    this->position.setX(x);
    this->position.setY(y);
    this->position.setZ(z);
    return this;
}

Entity *Entity::setScale(QVector3D v)
{
    this->scale = v;
    return this;
}

Entity *Entity::setScale(float x, float y, float z)
{
    this->scale.setX(x);
    this->scale.setY(y);
    this->scale.setZ(z);
    return this;
}
