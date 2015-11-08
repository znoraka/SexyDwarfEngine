#ifndef GAME_H
#define GAME_H

#include "scene.h"
class Game
{
public:
    static Game *getInstance();
    void initialize();
    void update(float delta);
    void pause();
    void resume();
    void setScene(Scene *scene);
    void addEvent(QEvent *event);

private:
    Game();
    bool paused;
    Scene *currentScene;

    static Game *instance;
};

#endif // GAME_H
