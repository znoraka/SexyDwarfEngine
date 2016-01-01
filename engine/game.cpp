#include "game.h"

Game *Game::instance = new Game();

void ERRCHECK(FMOD_RESULT result) {
    if(result != FMOD_OK) {
        qDebug() << "error : " << FMOD_ErrorString(result);
    }
}

Game *Game::getInstance()
{
    return instance;
}

void Game::initialize()
{
    mainWindow = new MainWindow();
    mainWindow->resize(WIDTH, HEIGHT);
    container = new QWidget();
    mainWindow->setCentralWidget(container);
    mainWindow->setVisible(true);
    mainWindow->setMouseTracking(true);

    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    firstEventList = true;
    timer.start(0);

    ERRCHECK( FMOD::Studio::System::create(&system) );
    ERRCHECK( system->getLowLevelSystem(&lowLevelSystem) );
    ERRCHECK( lowLevelSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0) );
    ERRCHECK( system->initialize(32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0) );

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

//    ERRCHECK( rpm->setValue(2150) );

//    ERRCHECK( eventInstance->start() );

//    // Position the listener at the origin
//        FMOD_3D_ATTRIBUTES attributes = { { 0 } };
//        attributes.forward.z = 1.0f;
//        attributes.up.y = 1.0f;
//        ERRCHECK( system->setListenerAttributes(0, &attributes) );

//        // Position the event 2 units in front of the listener
//        attributes.position.z = 2.0f;
//        ERRCHECK( eventInstance->set3DAttributes(&attributes) );
}

void Game::update(float delta)
{

    ERRCHECK( system->update() );
    if(!this->paused && this->currentScene != nullptr && currentScene->isReady() && delta > 0.1f && delta < 1000.0f) {

        if(firstEventList) {
            while (!events1.empty()) {
                toDelete.enqueue(events1.first());
                currentScene->handleEvent(events1.dequeue());
            }
        } else {
            while (!events2.empty()) {
                toDelete.enqueue(events2.first());
                currentScene->handleEvent(events2.dequeue());
            }
        }
        firstEventList = !firstEventList;

        this->currentScene->makeCurrent();
        this->currentScene->update(delta);
        QPainter p;
        p.begin(currentScene);
        p.setPen(Qt::yellow);
        p.setFont(QFont("Arial", 20));
        p.drawText(10, 30, QString::number((int) (1.0 / (delta * 0.001f))));
        p.end();
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        mainWindow->repaint();

        while (!toDelete.empty()) {
            delete toDelete.dequeue();
        }
    }
}

void Game::pause()
{
    this->paused = true;
}

void Game::resume()
{
    this->paused = false;
}

void Game::setScene(Scene *scene)
{
    this->currentScene = scene;
    scene->setParent(mainWindow);
    scene->initialize();
    this->mainWindow->setCentralWidget(scene);
}

void Game::addEvent(QEvent *event)
{
//    if(firstEventList) {
//        events2.enqueue(event);
//    } else {
//        events1.enqueue(event);
//    }
    currentScene->handleEvent(event);
}

QWidget *Game::getWidgetContainer() const
{
    return this->container;
}

void Game::update()
{
    float elapsed = this->elapsedTimer.elapsed();
    this->elapsedTimer.restart();
    this->update(elapsed);
}

Game::Game()
{
    paused = false;
    this->currentScene = nullptr;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
//    QKeyEvent *q = new QKeyEvent(*event);
//    Game::getInstance()->addEvent(q);
    Game::getInstance()->addEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
//    QKeyEvent *q = new QKeyEvent(*event);
//    Game::getInstance()->addEvent(q);
    Game::getInstance()->addEvent(event);
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
//    QMouseEvent *q = new QMouseEvent(*event);
//    Game::getInstance()->addEvent(q);
    Game::getInstance()->addEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
//    QMouseEvent *q = new QMouseEvent(*event);
//    Game::getInstance()->addEvent(q);
    Game::getInstance()->addEvent(event);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
//    QMouseEvent *q = new QMouseEvent(*event);
//    Game::getInstance()->addEvent(q);
    Game::getInstance()->addEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
//    QMouseEvent *q = new QMouseEvent(*event);
//    Game::getInstance()->addEvent(q);
    Game::getInstance()->addEvent(event);
}
