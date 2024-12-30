#ifndef ENTITIES_H
#define ENTITIES_H
#include <string>

class entity
{
protected:
    std::string id{};
    int health{};
    int attack{};
    float defense{};
    bool alive = true;

public:
    entity(std::string id, int health, int attack, float defense)
        : id{id}, health{health}, attack{attack}, defense{defense}
    {
    }
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
    player(int health, int attack, float defense, std::string id = "P1")
        : entity(id, health, attack, defense) {}
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