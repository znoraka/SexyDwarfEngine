#ifndef TOWERCOMPONENT_H
#define TOWERCOMPONENT_H

#include <QVector3D>
#include <QVector>
#include <QtGui/QOpenGLFunctions>

#include "engine/components/component.h"
#include "game/enemycomponent.h"
#include "game/bulletcomponent.h"
#include "engine/fmodmanager.h"

class TowerComponent : public Component
{
public:
    enum TowerType {FIRE, ICE, LIGHTNING, BULLET};
    TowerComponent();
    static Pool<TowerComponent *> *pool;
    const QString componentName() const override;

    void release() override;

    TowerComponent *init(QVector3D canonPosition, QList<Entity*> *enemies, float range, float attackSpeed, float damage, int price, TowerType type);
    virtual void update(float delta);
    TowerComponent *clone();

    static const QString name;

    float getRange() const;
    void drawRange(QVector3D color);
    void setReady();

    int getPrice() const;
    void upgradeDamage();
    void upgradeSpeed();
    int getDamageUpgradePrice() const;
    int getSpeedUpgradePrice() const;
    float getDamage() const;
    float getSpeed() const;

private:
    QVector3D canonPosition;
    Entity *target;
    float range;
    float attackSpeed;
    QList<Entity*> *enemies;
    float elapsed;
    TowerType type;
    float damage;
    int price;
    int damageUpgradePrice;
    int speedUpgradePrice;

    bool ready;

    VolumeComponent *volume;

    void setTarget();
    void shoot();
};

Q_DECLARE_METATYPE(TowerComponent::TowerType)

#endif // TOWERCOMPONENT_H
