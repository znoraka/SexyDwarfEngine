#include "goldearnedcomponent.h"

QString const GoldEarnedComponent::name = "GoldEarnedComponent";
Pool<GoldEarnedComponent *> *GoldEarnedComponent::pool = new Pool<GoldEarnedComponent*>([] () {return new GoldEarnedComponent();});

GoldEarnedComponent::GoldEarnedComponent()
{

}

const QString GoldEarnedComponent::componentName() const
{
    return GoldEarnedComponent::name;
}

void GoldEarnedComponent::release()
{
    pool->release(this);
}

GoldEarnedComponent *GoldEarnedComponent::init(int amount)
{
    this->amount = amount;
}

void GoldEarnedComponent::update(float delta)
{

}

GoldEarnedComponent *GoldEarnedComponent::clone()
{

}
