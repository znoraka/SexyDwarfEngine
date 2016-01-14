#include "player.h"

Player *Player::getInstance()
{
    static Player *instance = new Player();
    return instance;
}

void Player::init(int life, int gold)
{
    this->lifePoints = life;
    this->maxLifePoints = life;
    this->golds = gold;
    this->waveCount = 0;
}

void Player::setMaxLifePoints(int amount)
{
    maxLifePoints = amount;
}

void Player::heal(int amount)
{
    lifePoints += amount;
}

int Player::getLifePoints() const
{
    return lifePoints;
}

float Player::getMissingLifePercentage() const
{
    return (float) (maxLifePoints - lifePoints) / (float) maxLifePoints;
}

void Player::takeDamage(int amount)
{
    FMODManager::getInstance()->setCurrentEvent("event:/lifelost");
    FMODManager::getInstance()->startEventInstance();

    lifePoints -= amount;

    if(lifePoints < 0) lifePoints = 0;
}

int Player::getGold() const
{
    return golds;
}

int Player::getWave() const
{
    return waveCount;
}

void Player::earnGold(int amount)
{
    golds += amount;
}

void Player::spendGold(int amount)
{
    golds -= amount;
}

void Player::clearedWave()
{
    waveCount++;
}

Player::Player()
{
    waveCount = 0;
    lifePoints = 0;
    golds = 0;
}
