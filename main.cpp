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

//

//    FMOD::Studio::Bank* masterBank = NULL;
//    ERRCHECK( system->loadBankFile(FileUtility::loadTempFile(":/assets/sounds/Master Bank.bank").toStdString().c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank) );

//    FMOD::Studio::Bank* stringsBank = NULL;
//    ERRCHECK( system->loadBankFile(FileUtility::loadTempFile(":/assets/sounds/Master Bank.strings.bank").toStdString().c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank) );

//    FMOD::Studio::Bank* weaponBank = NULL;
//    ERRCHECK( system->loadBankFile(FileUtility::loadTempFile(":/assets/sounds/Vehicles.bank").toStdString().c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &weaponBank) );

//    FMOD::Studio::EventDescription* eventDescription = NULL;
//    ERRCHECK( system->getEvent("event:/Vehicles/Basic Engine", &eventDescription) );

//    FMOD::Studio::EventInstance* eventInstance = NULL;
//    ERRCHECK( eventDescription->createInstance(&eventInstance) );

//    FMOD::Studio::ParameterInstance* rpm = NULL;
//    ERRCHECK( eventInstance->getParameter("RPM", &rpm) );

//    ERRCHECK( rpm->setValue(650) );

//    ERRCHECK( eventInstance->start() );

    Game::getInstance()->initialize();

    Game::getInstance()->setScene(new TestScene());

    return app.exec();
}



