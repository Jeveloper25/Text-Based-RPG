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

unordered_map<string, float> &entity::getResistances()
{
    return resistances;
}

std::string &entity::getdamageType()
{
    return damageType;
}

// PLAYER SUBCLASS
void player::changeGuard()
{
    isGuard = isGuard == false ? true : false;
    resistances.at("Slash") += isGuard == true ? 0.3 : -0.3;
    resistances.at("Pierce") += isGuard == true ? 0.2 : -0.2;
    resistances.at("Magic") += isGuard == true ? 0.1 : -0.1;
}

bool player::stateGuard()
{
    return isGuard;
}

// KNIGHT SUBCLASS
knight::knight(int health, int attack, int idNum, unordered_map<string, float> resistances)
    : entity("", health, attack, resistances, "Slash")
{
    std::ostringstream stream;
    stream << "KNIGHT";
    if (idNum > 1)
    {
        stream << "(" << idNum << ")";
    }
    id = stream.str();
}

// MAGE SUBCLASS
mage::mage(int health, int attack, int idNum, unordered_map<string, float> resistances)
    : entity("", health, attack, resistances, "Magic")
{
    std::ostringstream stream;
    stream << "MAGE";
    if (idNum > 1)
    {
        stream << "(" << idNum << ")";
    }
    id = stream.str();
}

// ARCHER SUBCLASS

archer::archer(int health, int attack, int idNum, unordered_map<string, float> resistances)
    : entity("", health, attack, resistances, "Pierce")
{
    std::ostringstream stream;
    stream << "ARCHER";
    if (idNum > 1)
    {
        stream << "(" << idNum << ")";
    }
    id = stream.str();
}