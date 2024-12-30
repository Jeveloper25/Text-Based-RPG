#ifndef ENTITIES_H
#define ENTITIES_H
#include <string>
#include <unordered_map>

using namespace std;

class entity
{
protected:
    string id;
    int health;
    int attack;
    unordered_map<string, float> resistances;
    string damageType;
    bool alive = true;

public:
    entity(string id, int health, int attack, unordered_map<string, float> resistances, string dType)
        : id{id}, health{health}, attack{attack}, resistances{resistances}, damageType{dType}
    {
    }
    ~entity() {}
    int getHealth();
    void subHealth(int damage);
    bool isAlive();
    int getAttack();
    unordered_map<string, float> &getResistances();
    const string &getID();
    string &getdamageType();
};

class player : public entity
{
protected:
    bool isGuard = false;

public:
    player(int health, int attack, unordered_map<string, float> resistances = {{"Slash", 0.5}, {"Pierce", 0.3}, {"Magic", 0.3}}, string id = "P1")
        : entity(id, health, attack, resistances, "Slash") {}
    bool stateGuard();
    void changeGuard();
};

class knight : public entity
{
public:
    knight(int health, int attack, int idNum, unordered_map<string, float> resistances = {{"Slash", 0.5}, {"Pierce", 0.3}, {"Magic", 0.3}});
};

class mage : public entity
{
public:
    mage(int health, int attack, int idNum, unordered_map<string, float> resistances = {{"Slash", 0.1}, {"Pierce", 0.1}, {"Magic", 0.5}});
};

class archer : public entity
{
public:
    archer(int health, int attack, int idNum, unordered_map<string, float> resistances = {{"Slash", 0.2}, {"Pierce", 0.5}, {"Magic", 0.1}});
};
#endif