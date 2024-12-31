#include "entities.h"
#include "game.h"
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

unordered_map<string, float> &entity::getResistances()
{
    return resistances;
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

attack &player::getAttack()
{
    ostringstream stream;
    int input;
    for (int i = 0; i < attacks.size(); i++)
    {
        stream << "(" << i + 1 << ") " << attacks.at(i).name << "\n"
               << "Type: " << attacks.at(i).dType << "\n"
               << "Damage: " << attacks.at(i).damage << "\n\n";
    }
    printStream(stream);
    getSingleInput(input);
    do
    {
        if (input < 1 || input > (int)attacks.size())
        {
            stream << "Invalid input";
            printStream(stream);
            getSingleInput(input);
        }
    } while (input < 1 || input > (int)attacks.size());
    printLine(40);
    return attacks.at(input - 1);
}

// KNIGHT SUBCLASS
knight::knight(int idNum, int health, vector<attack> attacks, unordered_map<string, float> resistances)
    : entity("", health, attacks, resistances, "Slash")
{
    std::ostringstream stream;
    stream << "Knight";
    if (idNum > 1)
    {
        stream << "(" << idNum << ")";
    }
    id = stream.str();
}

attack &knight::getAttack()
{
    int index = getRandom(attacks.size() - 1);
    return attacks.at(index);
}

// MAGE SUBCLASS
mage::mage(int idNum, int health, vector<attack> attacks, unordered_map<string, float> resistances)
    : entity("", health, attacks, resistances, "Magic")
{
    std::ostringstream stream;
    stream << "Mage";
    if (idNum > 1)
    {
        stream << "(" << idNum << ")";
    }
    id = stream.str();
}

attack &mage::getAttack()
{
    int index = getRandom(attacks.size() - 1);
    return attacks.at(index);
}

// ARCHER SUBCLASS

archer::archer(int idNum, int health, vector<attack> attacks, unordered_map<string, float> resistances)
    : entity("", health, attacks, resistances, "Pierce")
{
    std::ostringstream stream;
    stream << "Archer";
    if (idNum > 1)
    {
        stream << "(" << idNum << ")";
    }
    id = stream.str();
}

attack &archer::getAttack()
{
    int index = getRandom(attacks.size() - 1);
    return attacks.at(index);
}