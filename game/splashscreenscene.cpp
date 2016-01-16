#include "splashscreenscene.h"

SplashScreenScene::SplashScreenScene()
{

}

SplashScreenScene::~SplashScreenScene()
{
    delete text;
    delete button;
    delete layout;
}

void SplashScreenScene::initialize()
{
    text = new QLabel("SexyDwarfEngine Demo v1.0");
    text->setStyleSheet("font:75pt");
    text->setAlignment(Qt::AlignCenter);

    button = new QPushButton("Play");
    connect(button, SIGNAL(clicked(bool)), this, SLOT(onStartClicked()));

    layout = new QVBoxLayout(this);
    layout->addWidget(text);
    layout->addWidget(button);
    button->setStyleSheet("font:50pt");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_CULL_FACE);
    glEnable (GL_BLEND);
    glEnable(GL_POLYGON_OFFSET_LINE);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,  GL_FASTEST);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    camera->initialize(Game::Graphics::width(), Game::Graphics::height(), -1000, 1000);
    QVector3D pos = camera->getPosition();
    camera->setScale(camera->getScale());

    e = Entity::pool->obtain();
    e->setScale(40, 40, 40);

    e->addChild(Entity::pool->obtain()->
                addComponent(VolumeComponent::pool->obtain()->init(":/assets/ply/tower.ply"))->
                setScale(1, 1, 1)->
                setRotation(0, 45, 45));

    this->addEntity(e);

    this->ready = true;
}

void SplashScreenScene::update(float delta)
{
    button->setFixedSize(Game::Graphics::width() * 0.2, Game::Graphics::height() * 0.2);
    button->move(Game::Graphics::width() * 0.5 - button->width() * 0.5,
                 Game::Graphics::height() * 0.7 - button->height() * 0.5);

    e->setPosition(Game::Graphics::width() * 0.5 * (1366 / Game::Graphics::width()), Game::Graphics::height() * 0.78 * (768 / Game::Graphics::height()), 0);
    e->setRotation(0, e->getRotation().y() + 1 * delta * 0.1, 0);


    Scene::update(delta);
}

void SplashScreenScene::onStartClicked()
{
    Game::getInstance()->setScene(new TestScene());
}

