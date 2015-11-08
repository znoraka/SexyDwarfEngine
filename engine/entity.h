#ifndef ENTITY_H
#define ENTITY_H

#include <QHash>
#include <QString>
#include <QVector>
#include <QVector3D>

#include "engine/components/component.h"

class Compenent;

class Entity
{
    friend class Pool<Entity *>;

public:
    Entity *addComponent(Component *component);
    void removeComponent(const QString componentName);
    Component *getComponent(const QString componentId);
    QVector<Component *> getComponents();
    static Pool<Entity *> *pool;
    void release();
    void update(float delta);

    QVector3D getPosition() const;
    QVector3D getRotation() const;
    QVector3D getScale() const;

    void setRotation(QVector3D v);
    void setRotation(float x, float y, float z);

    void setPosition(QVector3D v);
    void setPosition(float x, float y, float z);

    void setScale(QVector3D v);
    void setScale(float x, float y, float z);

protected:
    Entity();
    QVector3D position;
    QVector3D rotation;
    QVector3D scale;

private:
    QHash<QString, Component*> components;
};

#endif // ENTITY_H
