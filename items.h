#ifndef ITEMS_H
#define ITEMS_H

#include <string>
#include <vector>

using namespace std;

struct attack
{
    string name = "";
    string dType = "";
    double damageMultiplier = 0;

public:
    attack() = default;
    attack(string name, string dType, double dM)
        : name(name), dType(dType), damageMultiplier(dM) {}
};

class weapon
{
protected:
    int baseDamage;
    vector<attack> attacks;

public:
    weapon() {}
    vector<attack> &getAttacks() { return attacks; }
    int getDamage() { return baseDamage; }
};

// TEMP
class playerWep : public weapon
{
public:
    playerWep()
    {
        baseDamage = 60;
        attacks.emplace_back(attack("Strong Slash", "Slash", 1.0));
        attacks.emplace_back(attack("Thrust", "Pierce", 1.0));
        attacks.emplace_back(attack("Fireball", "Magic", 1.0));
    }
};

class sword : public weapon
{
public:
    sword()
    {
        baseDamage = 10;
        attacks.emplace_back("Weak Slash", "Slash", 1.0);
        attacks.emplace_back("Thrust", "Pierce", 1.5);
        attacks.emplace_back("Focused Slash", "Slash", 3.0);
    }
};

class staff : public weapon
{
public:
    staff()
    {
        baseDamage = 5;
        attacks.emplace_back("Incinerate", "Magic", 5.0);
        attacks.emplace_back("Smite", "Magic", 8.0);
        attacks.emplace_back("Ice Shard", "Pierce", 3.0);
    }
};

class bow : public weapon
{
public:
    bow()
    {
        baseDamage = 5;
        attacks.emplace_back("Weak Shot", "Pierce", 3.0);
        attacks.emplace_back("Charged Shot", "Pierce", 5.0);
        attacks.emplace_back("Rain of Arrows", "Pierce", 8.0);
    }
};

#endif