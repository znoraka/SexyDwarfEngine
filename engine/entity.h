#ifndef ENTITY_H
#define ENTITY_H

#include <QHash>
#include <QString>
#include <QVector>
#include <QVector3D>
#include <QSet>
#include <QMatrix4x4>
#include <QOpenGLFunctions>

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

    void addChild(Entity *child);
    void removeChild(Entity *child);

    QVector3D getPosition() const;
    QVector3D getRotation() const;
    QVector3D getScale() const;

    QVector3D getLocalPosition() const;
    QVector3D getLocalRotation() const;
    QVector3D getLocalScale() const;

    QMatrix4x4 getModelViewMatrix();
    QMatrix4x4 getProjectionMatrix();
    QMatrix4x4 getParentsTransformMatrix();

    Entity *setRotation(QVector3D v);
    Entity *setRotation(float x, float y, float z);

    Entity *setGlobalPosition(QVector3D v);
    Entity *setGlobalPosition(float x, float y, float z);

    Entity *setPosition(QVector3D v);
    Entity *setPosition(float x, float y, float z);

    Entity *setScale(QVector3D v);
    Entity *setScale(float x, float y, float z);

    bool hasParent() const;

protected:
    Entity();

    QVector3D position, localPosition;
    QVector3D rotation, localRotation;
    QVector3D scale, localScale;

    QMatrix4x4 modelview, projection;

private:
    QHash<QString, Component*> components;
    QSet<Entity*> children;
    Entity *parent;
    bool dirty;
};

#endif // ENTITY_H
