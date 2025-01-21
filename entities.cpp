#include "entities.h"
#include "game.h"
#include <sstream>
#include <memory>
#include <algorithm>

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

attack entity::getAttack()
{
    int index;
    vector<attack> filtered;
    vector<attack> &attacks = currWeapon->getAttacks();
    copy_if(attacks.begin(), attacks.end(), back_inserter(filtered), [*this](attack a) mutable
            { return a.staminaCost <= stamina; });
    index = getRandom(filtered.size() - 1);
    return filtered.at(index);
}

bool entity::checkStam()
{
    for (attack &at : currWeapon->getAttacks())
    {
        if (at.staminaCost <= stamina)
        {
            return true;
        }
    }
    return false;
}

// Regen stam if regen=True, spend stam if false
void entity::changeStam(bool regen, int staminaCost)
{
    regen ? stamina += staminaRegen : stamina -= staminaCost;
}

shared_ptr<weapon> entity::getLoot()
{
    int random = getRandom(9);
    if (random > 4)
    {
        return currWeapon;
    }
    return nullptr;
}

// PLAYER SUBCLASS
player::player()
    : entity("P1", 0, 100, 100, 40, 0, {{"Slash", 0.5}, {"Pierce", 0.3}, {"Magic", 0.3}})
{
    currWeapon = make_shared<playerWep>();
    baseHealth = health;
    baseStamina = stamina;
    inventory[currWeapon->getName()] = currWeapon;
}

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
    stamina = (baseStamina * (1 + (level * 0.2)));
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

attack player::getAttack()
{
    ostringstream stream;
    int input;
    attack *chosen;
    vector<attack> &attacks = currWeapon->getAttacks();

    for (int i = 0; i < attacks.size(); i++)
    {
        stream << "(" << i + 1 << ") " << attacks.at(i).name << "\n"
               << "Type: " << attacks.at(i).dType << "\n"
               << "Damage: " << attacks.at(i).damageMultiplier * currWeapon->getDamage() << "\n"
               << "Stamina Cost: " << attacks.at(i).staminaCost << "\n";
        if (attacks.at(i).staminaCost > stamina)
        {
            stream << "Insufficient stamina" << "\n";
        }
        stream << "\n";
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

    do
    {
        chosen = &attacks.at(input - 1);
        if (chosen->staminaCost > stamina)
        {
            stream << "Insufficient stamina";
            printStream(stream);
            getSingleInput(input);
        }

    } while (chosen->staminaCost > stamina);

    printLine(40);
    return *chosen;
}

void player::insertInventory(shared_ptr<weapon> wep)
{
    inventory.try_emplace(wep->getName(), wep);
}

// KNIGHT SUBCLASS
knight::knight(int idNum, int level)
    : entity("", level, 75, 50, 30, 40, {{"Slash", 0.5}, {"Pierce", 0.2}, {"Magic", 0.2}})
{
    this->health = (int)(health * (1 + (level * 0.3)));
    this->stamina = (int)(stamina * (1 + (level * 0.3)));
    currWeapon = make_shared<sword>();

    std::ostringstream stream;
    stream << "Knight";
    if (idNum > 1)
    {
        stream << "(" << idNum << ")";
    }
    id = stream.str();
}

double knight::getExp()
{
    return (exp * (1 + (level * 0.3)));
}

// MAGE SUBCLASS
mage::mage(int idNum, int level)
    : entity("", level, 50, 100, 30, 30, {{"Slash", 0.1}, {"Pierce", 0.1}, {"Magic", 0.5}})
{
    this->health = (int)(health * (1 + (level * 0.1)));
    this->stamina = (int)(stamina * (1 + (level * 0.1)));
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

// ARCHER SUBCLASS

archer::archer(int idNum, int level)
    : entity("", level, 50, 80, 30, 25, {{"Slash", 0.2}, {"Pierce", 0.5}, {"Magic", 0.1}})
{
    this->health = (int)(health * (1 + (level * 0.2)));
    this->stamina = (int)(stamina * (1 + (level * 0.2)));
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
