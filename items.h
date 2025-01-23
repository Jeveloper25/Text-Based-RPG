#ifndef ITEMS_H
#define ITEMS_H

#include <string>
#include <vector>
#include <sstream>

using namespace std;

struct attack
{
    string name = "";
    string dType = "";
    double damageMultiplier = 0;
    int staminaCost = 0;

public:
    attack() = default;
    attack(string name, string dType, double dM, int sC)
        : name{name}, dType{dType}, damageMultiplier{dM}, staminaCost{sC} {}
};

struct buff
{
    bool isActive = true;
    int duration;
    string buffType;
    string bonusType;
    int flatBonus;
    double multBonus;
};

class item
{
protected:
    string name;
    string type;

public:
    string getName() { return name; }
};

//// CONSUMBALES ////

/***
 * Only one instance of a given subclass should exist at any given time,
 * keep track of how many "are left" using the count variable.
 * Adding to the count is the responsibility of the user.
 */
class consumable : public item
{
protected:
    int count = 1;
    string cType;

public:
    consumable() { type = "consumable"; }
    virtual int useItem() { return -1; }
    /***
     *  Overload is not used in implementation,
     *  only used for overloading the function.
     *  By convention, input true.
     */
    virtual buff useItem(bool overload)
    {
        buff nullBuff;
        nullBuff.buffType = "NULL";
        return nullBuff;
    }
    void addCons() { count++; }
    string getCType() { return cType; }
    virtual string getEffect() { return "l"; }
    int getCount() { return count; }
    virtual int getDuration() { return -1; }
};

class healer : public consumable
{
private:
    int healAmount = 40;

public:
    healer()
    {
        cType = "Heal";
        name = "Health Potion";
    }
    virtual int useItem()
    {
        count--;
        return healAmount;
    }
    string getEffect()
    {
        ostringstream stream;
        stream << "Heals " << healAmount << " HP";
        return stream.str();
    }
};

class buffer : public consumable
{
protected:
    buff appliedBuff;

public:
    buffer() { cType = "Buffer"; }
    virtual buff useItem(bool overload)
    {
        count--;
        return appliedBuff;
    }
    int getDuration() { return appliedBuff.duration; }
};

class strengthener : public buffer
{
public:
    strengthener()
    {
        name = "Strength Potion";
        appliedBuff.buffType = "Attack";
        appliedBuff.bonusType = "Flat";
        appliedBuff.flatBonus = 20;
        appliedBuff.duration = 3;
        appliedBuff.multBonus = 0;
    }
    string getEffect()
    {
        ostringstream stream;
        stream << "Raises weapon damage of the next " << appliedBuff.duration << " attacks by" << appliedBuff.flatBonus << " points";
        return stream.str();
    }
};

class defender : public buffer
{
public:
    defender()
    {
        name = "Defense Potion";
        appliedBuff.buffType = "Defense";
        appliedBuff.bonusType = "Mult";
        appliedBuff.multBonus = 1.2;
        appliedBuff.duration = 6;
        appliedBuff.flatBonus = 0;
    }
    string getEffect()
    {
        ostringstream stream;
        stream << "Raises all defences by " << appliedBuff.multBonus << "x for the next " << appliedBuff.duration << " attacks";
        return stream.str();
    }
};

//// WEAPONS ////

class weapon : public item
{
protected:
    int baseDamage;
    vector<attack> attacks;

public:
    weapon() { type = "weapon"; }
    vector<attack> &getAttacks() { return attacks; }
    int getDamage() { return baseDamage; }
};

// TEMP
class playerWep : public weapon
{
public:
    playerWep()
    {
        name = "MyWep";
        baseDamage = 60;
        attacks.emplace_back(attack("Strong Slash", "Slash", 1.0, 60));
        attacks.emplace_back(attack("Thrust", "Pierce", 1.0, 60));
        attacks.emplace_back(attack("Fireball", "Magic", 1.0, 60));
    }
};

class sword : public weapon
{
public:
    sword()
    {
        name = "Steel Sword";
        baseDamage = 10;
        attacks.emplace_back("Weak Slash", "Slash", 1.0, 30);
        attacks.emplace_back("Thrust", "Pierce", 1.5, 45);
        attacks.emplace_back("Focused Slash", "Slash", 3.0, 60);
    }
};

class staff : public weapon
{
public:
    staff()
    {
        name = "Wooden staff";
        baseDamage = 5;
        attacks.emplace_back("Incinerate", "Magic", 5.0, 60);
        attacks.emplace_back("Smite", "Magic", 8.0, 80);
        attacks.emplace_back("Ice Shard", "Pierce", 3.0, 40);
    }
};

class bow : public weapon
{
public:
    bow()
    {
        name = "Long bow";
        baseDamage = 5;
        attacks.emplace_back("Weak Shot", "Pierce", 3.0, 20);
        attacks.emplace_back("Charged Shot", "Pierce", 5.0, 40);
        attacks.emplace_back("Rain of Arrows", "Pierce", 8.0, 70);
    }
};

#endif