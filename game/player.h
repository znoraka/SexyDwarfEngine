#ifndef PLAYER_H
#define PLAYER_H

#include "engine/fmodmanager.h"

class Player
{

public:
    static Player *getInstance();

    void init(int life, int gold);
    void setMaxLifePoints(int amount);
    void heal(int amount);
    int getLifePoints() const;
    float getMissingLifePercentage() const;
    void takeDamage(int amount);
    int getGold() const;
    int getWave() const;

    void earnGold(int amount);
    void spendGold(int amount);
    void clearedWave();

private:
    Player();

    int maxLifePoints;
    int lifePoints;
    int golds;
    int waveCount;
};

#endif // PLAYER_H
