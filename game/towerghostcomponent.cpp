#include "towerghostcomponent.h"

QString const TowerGhostComponent::name = "towerghostcomponent";
Pool<TowerGhostComponent *> *TowerGhostComponent::pool = new Pool<TowerGhostComponent*>([] () {return new TowerGhostComponent();});

TowerGhostComponent::TowerGhostComponent()
{
}

const QString TowerGhostComponent::componentName() const
{
    return TowerGhostComponent::name;
}

void TowerGhostComponent::release()
{
    TowerGhostComponent::pool->release(this);
}

TowerGhostComponent *TowerGhostComponent::init(QString mapFolder)
{
    map = QImage();
    this->mapFolder = mapFolder;
    tc = nullptr;
    vc = nullptr;
    return this;
}

void TowerGhostComponent::update(float delta)
{
    if(map.width() != 0) {
       initDependencies();

       if(hasRoom()) {
           tc->drawRange(QVector3D(0, 1, 0));
       } else {
           tc->drawRange(QVector3D(1, 0, 0));
       }

    } else {
        map = QImage(mapFolder + "p.png");
        return;
    }
}

TowerGhostComponent *TowerGhostComponent::clone()
{
    TowerGhostComponent *t = TowerGhostComponent::pool->obtain();
    t->map = map;
    t->tc = tc;
    t->vc = vc;
    t->mapFolder = mapFolder;

    return t;
}

void TowerGhostComponent::initDependencies()
{
    if(tc == nullptr || vc == nullptr) {
        tc = static_cast<TowerComponent*>(getEntity()->getComponent(TowerComponent::name));
        if(tc == nullptr) {
            qDebug() << "no TowerComponent found";
            return;
        }
        vc = static_cast<VolumeComponent*>(getEntity()->getComponent(VolumeComponent::name));
        if(vc == nullptr) {
            qDebug() << "no VolumeComponent found";
            return;
        }
    }
}

bool TowerGhostComponent::hasRoom()
{
    initDependencies();
    if(vc != nullptr && tc != nullptr) {
        QVector3D v = getEntity()->getLocalPosition();

        if(v.x() < 0 || v.y() < 0 || v.x() > map.width() || v.y() > map.height()) {
            tc->drawRange(QVector3D(1, 0, 0));
            return false;
        }

        QRectF bounds = vc->getBounds();

        float width = bounds.width() * getEntity()->getScale().x();
        float height = bounds.height() * getEntity()->getScale().y();

        width *= 0.5;
        height *= 0.5;

        int sum = 0;

        sum += qAlpha(map.pixel(v.x() - width, map.height() - v.y() - height));
        sum += qAlpha(map.pixel(v.x() + width, map.height() - v.y() - height));
        sum += qAlpha(map.pixel(v.x() - width, map.height() - v.y() + height));
        sum += qAlpha(map.pixel(v.x() + width, map.height() - v.y() + height));

        return sum == 0;
    }
    return false;
}
