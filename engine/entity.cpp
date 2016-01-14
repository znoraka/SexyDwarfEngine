#include "entity.h"

Pool<Entity *> *Entity::pool = new Pool<Entity*>([] () {return new Entity();});

Entity::Entity()
{
    this->parent = nullptr;
    this->dirty = true;
    this->ready = false;
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
        components.remove(componentName);
    }
}

Component *Entity::getComponent(const QString componentId)
{
    auto i = components.find(componentId);

    if(i == components.end()) {
//        qDebug() << "no component " << componentId << "found";
        return nullptr;
    }

    return i.value();
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
        qDebug() << "releasing " << var->componentName();
        var->release();
    }
    this->dirty = true;
    if(this->parent != nullptr)
        this->parent->removeChild(this);
    this->parent = nullptr;
    components.clear();
    foreach (Entity *e, children) {
        e->release();
    }
    children.clear();
    pool->release(this);
}

void Entity::update(float delta)
{
    GLfloat m[16];

    glGetFloatv (GL_PROJECTION_MATRIX, m);
    projection = QMatrix4x4(m);

    glGetFloatv (GL_MODELVIEW_MATRIX, m);
    modelview = QMatrix4x4(m);

    glPushMatrix();

    glTranslatef(localPosition.x(), localPosition.y(), localPosition.z());
    glRotatef(localRotation.x(), 1, 0, 0);
    glRotatef(localRotation.z(), 0, 0, 1);
    glRotatef(localRotation.y(), 0, 1, 0);
    glScalef(localScale.x(), localScale.y() , localScale.z());


    if(this->parent == nullptr) {
        this->position = localPosition;
        this->rotation = localRotation;
        this->scale = localScale;
    }

    foreach (Component *c, components) {
        c->update(delta);
    }

    foreach (Entity *child, children) {
        child->position = child->localPosition + position;
        child->rotation = child->localRotation + rotation;
        child->scale = child->localScale * scale;
        child->dirty = true;
        child->update(delta);
    }

    glPopMatrix();
}

Entity *Entity::clicked(Qt::MouseButton button, int x, int y)
{
    foreach (Component *c, components) {
        qDebug() << c->componentName() << ":" << c->clicked(button, x, y);
        if(c->clicked(button, x, y)) return this;
    }

    foreach (Entity *child, children) {
        Entity *e = child->clicked(button, x, y);
        if(e != nullptr) {
            for(Component *c : e->getComponents()) {
                qDebug() << c->componentName();
            }
            return e;
        }
    }
    return nullptr;
}

void Entity::addChild(Entity *child)
{
    this->children.insert(child);
    child->parent = this;
}

void Entity::removeChild(Entity *child)
{
    this->children.remove(child);
    child->parent = nullptr;
    dirty = true;
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

QVector3D Entity::getLocalPosition() const
{
    return this->localPosition;
}

QVector3D Entity::getLocalRotation() const
{
    return this->localRotation;
}

QVector3D Entity::getLocalScale() const
{
    return this->localScale;
}

QMatrix4x4 Entity::getModelViewMatrix()
{
    return modelview;
}

QMatrix4x4 Entity::getProjectionMatrix()
{
    return projection;
}

QMatrix4x4 Entity::getParentsTransformMatrix()
{
    QMatrix4x4 m;
    m.setToIdentity();
    if(this->parent != nullptr) {
        m *= this->parent->getModelViewMatrix();
        m *= this->parent->getParentsTransformMatrix();
    }
    return m;
}

Entity *Entity::setRotation(QVector3D v)
{
    this->localRotation = v;
    dirty = true;
    return this;
}

Entity *Entity::setRotation(float x, float y, float z)
{
    this->localRotation.setX(x);
    this->localRotation.setY(y);
    this->localRotation.setZ(z);
    dirty = true;
    return this;
}

Entity *Entity::setGlobalPosition(QVector3D v)
{
    this->localPosition = v * modelview.inverted();
    return this;
}

Entity *Entity::setGlobalPosition(float x, float y, float z)
{
    this->setGlobalPosition(QVector3D(x, y, z));
    return this;
}

Entity *Entity::setPosition(QVector3D v)
{
    this->localPosition = v;
    dirty = true;
    return this;
}

Entity *Entity::setPosition(float x, float y, float z)
{
    this->localPosition.setX(x);
    this->localPosition.setY(y);
    this->localPosition.setZ(z);
    dirty = true;
    return this;
}

Entity *Entity::setScale(QVector3D v)
{
    this->localScale = v;
    dirty = true;
    return this;
}

Entity *Entity::setScale(float x, float y, float z)
{
    this->localScale.setX(x);
    this->localScale.setY(y);
    this->localScale.setZ(z);
    dirty = true;
    return this;
}

Entity *Entity::getParent() const
{
    return this->parent;
}

QSet<Entity *> Entity::getChildren() const
{
    return this->children;
}

bool Entity::hasParent() const
{
    return parent != nullptr;
}
