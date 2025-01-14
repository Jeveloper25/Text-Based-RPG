#include "entities.h"
#include "game.h"
#include <sstream>
#include <memory>

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

int entity::getLevel()
{
    return level;
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

void player::reset()
{
    health = (baseHealth * (1 + (level * 0.2)));
    if (stateGuard())
    {
        changeGuard();
    }
}

double player::getExpThreshold()
{
    return expThreshold;
}

bool player::gainExp(double exp)
{
    this->exp += exp;
    if (exp >= expThreshold)
    {
        level++;
        this->exp -= expThreshold;
        expThreshold *= 1.2;
        return true;
    }
    return false;
}

attack &player::getAttack()
{
    ostringstream stream;
    int input;
    vector<attack> &attacks = currWeapon->getAttacks();
    for (int i = 0; i < attacks.size(); i++)
    {
        stream << "(" << i + 1 << ") " << attacks.at(i).name << "\n"
               << "Type: " << attacks.at(i).dType << "\n"
               << "Damage: " << attacks.at(i).damageMultiplier * currWeapon->getDamage() << "\n\n";
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
knight::knight(int idNum, int level, int health, double exp, unordered_map<string, float> resistances)
    : entity("", level, health, exp, resistances)
{
    this->health = (int)(health * (1 + (level * 0.3)));
    currWeapon = make_shared<sword>();

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
    vector<attack> &attacks = currWeapon->getAttacks();
    int index = getRandom(attacks.size() - 1);
    return attacks.at(index);
}

double knight::getExp()
{
    return (exp * (1 + (level * 0.3)));
}

// MAGE SUBCLASS
mage::mage(int idNum, int level, int health, double exp, unordered_map<string, float> resistances)
    : entity("", level, health, exp, resistances)
{
    this->health = (int)(health * (1 + (level * 0.1)));
    currWeapon = make_shared<staff>();

    std::ostringstream stream;
    stream << "Mage";
    if (idNum > 1)
    {
        stream << "(" << idNum << ")";
    }
    id = stream.str();
}

double mage::getExp()
{
    return (exp * (1 + (level * 0.1)));
}

attack &mage::getAttack()
{
    vector<attack> &attacks = currWeapon->getAttacks();
    int index = getRandom(attacks.size() - 1);
    return attacks.at(index);
}

// ARCHER SUBCLASS

archer::archer(int idNum, int level, int health, double exp, unordered_map<string, float> resistances)
    : entity("", level, health, exp, resistances)
{
    this->health = (int)(health * (1 + (level * 0.2)));
    currWeapon = make_shared<bow>();

    std::ostringstream stream;
    stream << "Archer";
    if (idNum > 1)
    {
        stream << "(" << idNum << ")";
    }
    id = stream.str();
}

double archer::getExp()
{
    return (exp * (1 + (level * 0.2)));
}

attack &archer::getAttack()
{
    vector<attack> &attacks = currWeapon->getAttacks();
    int index = getRandom(attacks.size() - 1);
    return attacks.at(index);
}