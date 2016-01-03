#include "player.h"

Player *Player::getInstance()
{
    static Player *instance = new Player();
    return instance;
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
}
