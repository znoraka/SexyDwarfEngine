#include "component.h"

QString const Component::name = "component";
Pool<Component *> *Component::pool = new Pool<Component*>([] () {return nullptr;});

Component::Component()
{
    entity = nullptr;
}

const QString Component::componentName() const
{
    return Component::name;
}

const QString Component::systemName() const
{
    return System::name;
}

Component::~Component()
{

}

System *Component::instantiateSystem()
{
    return new System();
}

void Component::setEntity(Entity *entity)
{
    this->entity = entity;
}

Entity *Component::getEntity() const
{
    return this->entity;
}

void Component::release()
{
    Component::pool->release(this);
}

bool Component::clicked(Qt::MouseButton button, int x, int y)
{
    return false;
}

Component *Component::init()
{
    return this;
}

void Component::update(float delta)
{

}
