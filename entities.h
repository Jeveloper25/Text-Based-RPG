#ifndef ENTITIES_H
#define ENTITIES_H
#include <string>

class entity
{
protected:
    std::string id;
    int health;
    int attack;
    bool alive = true;
    float defense;

public:
    // entity();
    // ~entity();
    int getHealth();
    void subHealth(int damage);
    bool isAlive();
    int getAttack();
    float getDefense();
    const std::string &getID();
};

class player : public entity
{
protected:
    bool isGuard = false;

public:
    player(int health, int attack, float defense, std::string id = "P1");
    float getDefense();
    bool stateGuard();
    void changeGuard();
};

class knight : public entity
{
public:
    knight(int health, int attack, float defense, int idNum);
};

class mage : public entity
{
public:
    mage(int health, int attack, float defense, int idNum);
};
#endif