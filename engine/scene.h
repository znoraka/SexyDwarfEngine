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
#include <QVBoxLayout>
#include <QPushButton>
#include <QQueue>
#include <functional>

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
    virtual void update(float delta);
    void addCallBack(std::function<void()> f);
    Entity *clicked(Qt::MouseButton button, int x, int y);

    virtual bool handleEvent(QEvent *event);

protected:
    Camera *camera;
    virtual void initialize();
    bool ready = false;
    bool displayLines = false;

    QTimer timer;
    QElapsedTimer elapsedTimer;
    float elapsed;

private:
    QSet<Entity*> entities;
    QList<System*> systems;
    QQueue<std::function<void()> > callbacks;
};

#endif // SCENE_H
