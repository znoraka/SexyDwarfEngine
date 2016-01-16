#ifndef SPLASHSCREENSCENE_H
#define SPLASHSCREENSCENE_H

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "engine/game.h"
#include "game/testscene.h"
#include "engine/scene.h"
#include "engine/components/volumecomponent.h"

class SplashScreenScene : public Scene
{
    Q_OBJECT
public:
    SplashScreenScene();
    ~SplashScreenScene();
    void initialize();
    void update(float delta);

public slots:
    void onStartClicked();

private:
    QVBoxLayout *layout;
    QLabel *text;
    QPushButton *button;
    Entity *e;
};

#endif // SPLASHSCREENSCENE_H
