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
#include "game/testscene.h"
#include "engine/tools/fileutility.h"

#include "consts.h"

int main(int argc, char **argv)
{
    srand(time(NULL));

    QApplication app(argc, argv);

    Game::getInstance()->initialize();

    Game::getInstance()->setScene(new TestScene());

    return app.exec();
}



