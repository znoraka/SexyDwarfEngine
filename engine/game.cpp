#include "game.h"

Game *Game::instance = new Game();

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

    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start(0);
}

void Game::update(float delta)
{

    if(!this->paused && this->currentScene != nullptr && currentScene->isReady() && delta > 0.1f && delta < 1000.0f) {
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
    Game::getInstance()->addEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    Game::getInstance()->addEvent(event);
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    Game::getInstance()->addEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    Game::getInstance()->addEvent(event);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    Game::getInstance()->addEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Game::getInstance()->addEvent(event);
}
