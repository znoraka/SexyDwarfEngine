#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QTimer>
#include <QElapsedTimer>
#include <QPixmap>

#include "openglwindow.h"
#include "game.h"

class GameWindow : public OpenGLWindow
{
public:
    GameWindow();

    void initialize();
    void render();
    void render(float delta);
    bool event(QEvent *event);

    void keyPressEvent(QKeyEvent *event);

private:

    int m_frame;
    QImage m_image;
    Game *game;
    float framerate = 1.0f / 60.0f;

    QTimer timer;
    QElapsedTimer elapsedTimer;
    float elapsed;
};


#endif // GAMEWINDOW_H
