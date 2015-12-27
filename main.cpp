//#include "engine/gamewindow.h"
#include "engine/game.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>

#include <QtCore/qmath.h>
#include <QMouseEvent>
#include <QKeyEvent>
#include <time.h>
#include <sys/time.h>
#include <iostream>

#include <QtCore>
#include <QtGui>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QApplication>
#include "engine/camera.h"
#include "engine/components/component.h"
#include "engine/components/positioncomponent.h"
#include "engine/entity.h"
#include "game/trianglecomponent.h"

#include "consts.h"

#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "fmod_common.h"

int main(int argc, char **argv)
{
    srand(time(NULL));
//    QSurfaceFormat::setDefaultFormat(QSurfaceFormat::OpenGL);
    QApplication app(argc, argv);

//    QSurfaceFormat format;
//    format.setSamples(16);

//    QMainWindow mainWindow;
//    mainWindow.resize(WIDTH, HEIGHT);
//    QWidget *container = new QWidget();
//    QVBoxLayout *layout = new QVBoxLayout();
//    mainWindow.setCentralWidget(container);
//    container->setLayout(layout);

//    GameWindow *w = new GameWindow();
//    w->setFormat(format);
//    w->resize(WIDTH, HEIGHT * 0.8);
//    w->show();
////    w->setAnimating(true);

//    layout->addWidget(w);
    Game::getInstance()->initialize();

    Game::getInstance()->setScene(new Scene());

    return app.exec();
}



