#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include <QTimer>
#include <QObject>
#include <QOpenGLContext>
#include <QSurface>

#include "scene.h"

class Game : public QObject
{
    Q_OBJECT
public:
    static Game *getInstance();
    void initialize();
    void update(float delta);
    void pause();
    void resume();
    void setScene(Scene *scene);
    void addEvent(QEvent *event);

    QWidget *getWidgetContainer() const;

public slots:
    void update();

private:
    Game();
    bool paused;
    Scene *currentScene;

    QTimer timer;
    QElapsedTimer elapsedTimer;

    QMainWindow *mainWindow;
    QWidget *container;

    static Game *instance;
};

#endif // GAME_H
