#include "game.h"

Game *Game::instance = new Game();

Game *Game::getInstance()
{
    return instance;
}

void Game::exit()
{
    qApp->exit();
}

void Game::initialize()
{
    mainWindow = new MainWindow();
    container = new QWidget();
    mainWindow->setCentralWidget(container);
    mainWindow->setVisible(true);
    mainWindow->setMouseTracking(true);
    mainWindow->showFullScreen();

    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    firstEventList = true;
    timer.start(0);
}

void Game::update(float delta)
{
    if(!this->paused && this->currentScene != nullptr && currentScene->isReady() && delta > 0.1f && delta < 1000.0f) {

        FMODManager::getInstance()->update();
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
    if(this->currentScene != nullptr) {
        this->currentScene->destroyData();
        this->currentScene->doneCurrent();
        delete this->currentScene;
    }
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

float Game::Graphics::width()
{
    return instance->mainWindow->width();
}

float Game::Graphics::height()
{
    return instance->mainWindow->height();
}
