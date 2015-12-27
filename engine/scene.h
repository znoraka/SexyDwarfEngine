#ifndef SCENE_H
#define SCENE_H

#include <QSet>
#include <QList>
#include <QHash>
#include <QEvent>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_3_Core>
#include <QPainter>
#include <QElapsedTimer>
#include <QTimer>
#include <QMainWindow>

#include "camera.h"
#include "entity.h"
#include "components/positioncomponent.h"
#include "systems/system.h"
#include "game/trianglecomponent.h"
#include "components/volumecomponent.h"
#include "game/mapcomponent.h"
#include "game/pathfollowercomponent.h"
#include "game/towercomponent.h"
#include "consts.h"

class Scene : public QOpenGLWidget
{
    friend class Game;

public:
    Scene();
    void addEntity(Entity *entity);
    void removeEntity(Entity *entity);
    bool isReady() const;
    void update(float delta);
    void initializeGL();
//    void paintGL();

    bool handleEvent(QEvent *event);

protected:
    Camera *camera;

private:
    bool containsSystem(const QString s) const;
    void addSystem(System* system);
    void initialize(QMainWindow *parent);

    QSet<Entity*> entities;
    QList<System*> systems;
    QHash<QString, System*> systemsHash;
    bool ready = false;
    bool displayLines = false;

    float framerate = 1.0f / 60.0f;

    QTimer timer;
    QElapsedTimer elapsedTimer;
    float elapsed;

};

#endif // SCENE_H
