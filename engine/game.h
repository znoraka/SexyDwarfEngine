#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include <QTimer>
#include <QObject>
#include <QOpenGLContext>
#include <QSurface>
#include <QLabel>
#include <QQueue>
#include <QtCore>

#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "fmod_common.h"
#include "fmod_studio_common.h"
#include "fmod_errors.h"
#include "fmod_output.h"

#include "scene.h"
#include "engine/fmodmanager.h"
#include "engine/tools/fileutility.h"

class MainWindow;

class Game : public QObject
{
    Q_OBJECT
public:
    static Game *getInstance();
    static void exit();
    void initialize();
    void update(float delta);
    void pause();
    void resume();
    void setScene(Scene *scene);
    void addEvent(QEvent *event);

    QWidget *getWidgetContainer() const;

    class Graphics {
    public:
        static float width();
        static float height();
    };

public slots:
    void update();

private:
    Game();
    bool paused;
    Scene *currentScene;

    QTimer timer;
    QElapsedTimer elapsedTimer;

    MainWindow *mainWindow;
    QWidget *container;

    QQueue<QEvent*> events1, events2, toDelete;

    bool firstEventList;

    static Game *instance;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // GAME_H
