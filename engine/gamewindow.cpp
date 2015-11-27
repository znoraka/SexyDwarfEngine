#include "gamewindow.h"

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

GameWindow::GameWindow()
{
    game = Game::getInstance();
}

void GameWindow::initialize()
{

    timer.setInterval(framerate * 1000);
    timer.start();
    elapsedTimer.start();
    this->connect(&timer, SIGNAL(timeout()), this, SLOT(renderNow()));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_CULL_FACE);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,  GL_FASTEST);

    Game::getInstance()->initialize();
}

void GameWindow::render()
{

    bool needsInitialize = false;
    if (!m_context) {
        m_context = new QOpenGLContext(this);
        m_context->setFormat(requestedFormat());
        m_context->create();
        needsInitialize = true;
    }

    m_context->makeCurrent(this);

    if (needsInitialize)
    {
        initializeOpenGLFunctions();
    }

    if (!m_device)
        m_device = new QOpenGLPaintDevice();

    m_device->setSize(size());

    QPainter p;

    elapsed = elapsedTimer.elapsed();
    if(elapsed > timer.interval() * 0.5) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        this->render((float) elapsed * 0.0005f);
//        glDisableVertexAttribArray(GL_DEPTH_TEST);
        p.begin(m_device);
        p.setPen(Qt::yellow);
        p.setFont(QFont("Arial", 20));
        p.drawText(10, 30, QString::number((int) (1.0 / (elapsed * 0.001f))));
        p.end();
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        elapsed -= timer.interval();
        elapsedTimer.restart();
    }
}

void GameWindow::render(float delta)
{
    game->update(delta);
    ++m_frame;
}

bool GameWindow::event(QEvent *event)
{
    Game::getInstance()->addEvent(event);
    switch (event->type())
    {
    case QEvent::UpdateRequest:

        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    Game::getInstance()->addEvent(event);
    switch(event->key())
    {
    case Qt::Key_Escape:
        qApp->exit();
        break;

    }
    renderNow();
}
