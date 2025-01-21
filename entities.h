#ifndef ENTITIES_H
#define ENTITIES_H
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "items.h"

using namespace std;

class entity
{
protected:
    string id;
    int level;
    int health;
    int stamina;
    int staminaRegen;
    double exp;
    unordered_map<string, float> resistances;
    shared_ptr<weapon> currWeapon;
    bool alive = true;

public:
    entity(string id, int level, int health, int stamina, int staminaRegen, double exp, unordered_map<string, float> resistances)
        : id{id}, level{level}, health{health}, stamina{stamina}, staminaRegen{staminaRegen}, exp{exp}, resistances{resistances}
    {
    }
    ~entity() {}
    int getHealth();
    int getLevel();
    void subHealth(int damage);
    bool isAlive();
    unordered_map<string, float> &getResistances();
    const string &getID();
    virtual attack getAttack();
    void changeStam(bool regen, int staminaCost = 0);
    virtual double getExp()
    {
        return exp;
    }
    int getRaw()
    {
        return currWeapon->getDamage();
    }
    int getStam() { return stamina; }
    bool checkStam();
    shared_ptr<weapon> getLoot();
};

class player : public entity
{
protected:
    int baseHealth;
    int baseStamina;
    bool isGuard = false;
    double expThreshold = 100;
    unordered_map<string, shared_ptr<weapon>> inventory;

public:
    player();
    ~player() {}
    bool stateGuard();
    void changeGuard();
    void reset();
    double getExpThreshold();
    bool gainExp(double exp);
    virtual attack getAttack();
    void insertInventory(shared_ptr<weapon> wep);
    unordered_map<string, shared_ptr<weapon>> &getInv() { return inventory; }
    void setCurrWeapon(shared_ptr<weapon> newWeapon) { currWeapon = newWeapon; }
};

class knight : public entity
{
public:
    knight(int idNum,
           int level = 0);
    virtual double getExp();
};

class mage : public entity
{
public:
    mage(int idNum,
         int level = 0);
    virtual double getExp();
};

class archer : public entity
{
public:
    archer(int idNum,
           int level = 0);
    virtual double getExp();
};
#endif