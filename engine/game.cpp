#include "game.h"

Game *Game::instance = new Game();

Game *Game::getInstance()
{
    return instance;
}

void Game::initialize()
{
    if(this->currentScene != nullptr) {
        this->currentScene->initialize();
    }
}

void Game::update(float delta)
{
    if(!this->paused && this->currentScene != nullptr && currentScene->isReady()) {
        this->currentScene->update(delta);
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
}

void Game::addEvent(QEvent *event)
{
    currentScene->handleEvent(event);
}

Game::Game()
{
    paused = false;
    this->currentScene = nullptr;
}
