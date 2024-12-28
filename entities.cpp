#include "entities.h"
#include <sstream>

// ENTITY CLASS

// entity::~entity();

const std::string &entity::getID()
{
    return this->id;
}

void entity::subHealth(int damage)
{
    this->health -= damage;
    if (this->health <= 0)
    {
        this->alive = false;
    }
}

int entity::getHealth()
{
    return this->health;
}

bool entity::isAlive()
{
    return this->alive;
}

int entity::getAttack()
{
    return this->attack;
}

float entity::getDefense()
{
    return this->defense;
}

// PLAYER SUBCLASS

player::player(int health, int attack, float defense, std::string id)
{
    this->health = health;
    this->attack = attack;
    this->defense = defense;
    this->id = id;
}

float player::getDefense()
{
    return this->defense;
}

void player::changeGuard()
{
    this->isGuard = this->isGuard == false ? true : false;
    this->defense += this->isGuard == true ? 0.3 : -0.3;
}

bool player::stateGuard()
{
    return this->isGuard;
}

// KNIGHT SUBCLASS
knight::knight(int health, int attack, float defense, int idNum)
{
    this->health = health;
    this->attack = attack;
    this->defense = defense;

    std::ostringstream stream;
    stream << "Knight";
    if (idNum > 1)
    {
        stream << "(" << idNum << ")";
    }
    this->id = stream.str();
}

// MAGE SUBCLASS
mage::mage(int health, int attack, float defense, int idNum)
{
    this->health = health;
    this->attack = attack;
    this->defense = defense;

    std::ostringstream stream;
    stream << "Mage";
    if (idNum > 1)
    {
        stream << "(" << idNum << ")";
    }
    this->id = stream.str();
}