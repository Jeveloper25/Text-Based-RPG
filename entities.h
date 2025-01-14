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
    double exp;
    unordered_map<string, float> resistances;
    shared_ptr<weapon> currWeapon;
    bool alive = true;

public:
    entity(string id, int level, int health, double exp, unordered_map<string, float> resistances)
        : id{id}, level{level}, health{health}, exp{exp}, resistances{resistances}
    {
    }
    ~entity() {}
    int getHealth();
    int getLevel();
    void subHealth(int damage);
    bool isAlive();
    unordered_map<string, float> &getResistances();
    const string &getID();
    virtual attack &getAttack()
    {
        return currWeapon->getAttacks()[0];
    }
    virtual double getExp()
    {
        return exp;
    }
    int getRaw()
    {
        return currWeapon->getDamage();
    }
};

class player : public entity
{
protected:
    bool isGuard = false;
    int baseHealth;
    double expThreshold = 100;

public:
    player(int level = 0,
           int health = 100,
           double exp = 0,
           unordered_map<string, float> resistances = {{"Slash", 0.5}, {"Pierce", 0.3}, {"Magic", 0.3}},
           string id = "P1")
        : entity(id, level, health, exp, resistances)
    {
        currWeapon = make_shared<playerWep>();
        baseHealth = health;
    }
    ~player() {}
    bool stateGuard();
    void changeGuard();
    void reset();
    double getExpThreshold();
    bool gainExp(double exp);
    virtual attack &getAttack();
};

class knight : public entity
{
public:
    knight(int idNum,
           int level = 0,
           int health = 75,
           double exp = 40,
           unordered_map<string, float> resistances = {{"Slash", 0.5}, {"Pierce", 0.2}, {"Magic", 0.2}});
    virtual attack &getAttack();
    virtual double getExp();
};

class mage : public entity
{
public:
    mage(int idNum,
         int level = 0,
         int health = 50,
         double exp = 30,
         unordered_map<string, float> resistances = {{"Slash", 0.1}, {"Pierce", 0.1}, {"Magic", 0.5}});
    virtual attack &getAttack();
    virtual double getExp();
};

class archer : public entity
{
public:
    archer(int idNum,
           int level = 0,
           int health = 50,
           double exp = 25,
           unordered_map<string, float> resistances = {{"Slash", 0.2}, {"Pierce", 0.5}, {"Magic", 0.1}});
    virtual attack &getAttack();
    virtual double getExp();
};
#endif