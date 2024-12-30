#include "entities.h"
#include <sstream>

// ENTITY CLASS

const std::string &entity::getID()
{
    return id;
}

void entity::subHealth(int damage)
{
    health -= damage;
    if (health <= 0)
    {
        alive = false;
    }
}

int entity::getHealth()
{
    return health;
}

bool entity::isAlive()
{
    return alive;
}

int entity::getAttack()
{
    return attack;
}

float entity::getDefense()
{
    return defense;
}

// PLAYER SUBCLASS
float player::getDefense()
{
    return defense;
}

void player::changeGuard()
{
    isGuard = isGuard == false ? true : false;
    defense += isGuard == true ? 0.3 : -0.3;
}

bool player::stateGuard()
{
    return isGuard;
}

// KNIGHT SUBCLASS
knight::knight(int health, int attack, float defense, int idNum)
    : entity("", health, attack, defense)
{
    std::ostringstream stream;
    stream << "Knight";
    if (idNum > 1)
    {
        stream << "(" << idNum << ")";
    }
    id = stream.str();
}

// MAGE SUBCLASS
mage::mage(int health, int attack, float defense, int idNum)
    : entity("", health, attack, defense)
{
    std::ostringstream stream;
    stream << "Mage";
    if (idNum > 1)
    {
        stream << "(" << idNum << ")";
    }
    id = stream.str();
}