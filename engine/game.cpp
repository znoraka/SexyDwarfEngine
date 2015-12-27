#include "game.h"

Game *Game::instance = new Game();

Game *Game::getInstance()
{
    return instance;
}

void Game::initialize()
{
    mainWindow = new QMainWindow();
    mainWindow->resize(WIDTH, HEIGHT);
    container = new QWidget();
    mainWindow->setCentralWidget(container);
    mainWindow->setVisible(true);

    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start(0);
}

void Game::update(float delta)
{
    if(!this->paused && this->currentScene != nullptr && currentScene->isReady()) {
        this->currentScene->makeCurrent();
        this->currentScene->update(delta);
        mainWindow->repaint();
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
    scene->initialize(mainWindow);
    this->mainWindow->setCentralWidget(scene);
}

void Game::addEvent(QEvent *event)
{
    currentScene->handleEvent(event);
}

QWidget *Game::getWidgetContainer() const
{
    return this->container;
}

void Game::update()
{
    this->update(this->elapsedTimer.elapsed());
    this->elapsedTimer.restart();
}

Game::Game()
{
    paused = false;
    this->currentScene = nullptr;
}
