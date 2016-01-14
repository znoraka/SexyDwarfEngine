#ifndef GOLDEARNEDCOMPONENT_H
#define GOLDEARNEDCOMPONENT_H

#include <QtGui/QOpenGLFunctions>

#include "engine/components/component.h"

#include "QPainter"


class GoldEarnedComponent : public Component
{
public:
    GoldEarnedComponent();
    static Pool<GoldEarnedComponent *> *pool;
    const QString componentName() const override;

    void release() override;

    GoldEarnedComponent *init(int amount);
    virtual void update(float delta);
    GoldEarnedComponent *clone();

    static const QString name;

private:
    int amount;
};

#endif // GOLDEARNEDCOMPONENT_H
