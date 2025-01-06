#ifndef ENTITIES_H
#define ENTITIES_H
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct attack
{
    string name;
    string dType;
    int damage;
};

class entity
{
protected:
    string id;
    int health;
    vector<attack> attacks;
    unordered_map<string, float> resistances;
    bool alive = true;

public:
    entity(string id, int health, vector<attack> attacks, unordered_map<string, float> resistances)
        : id{id}, health{health}, attacks{attacks}, resistances{resistances}
    {
    }
    ~entity() {}
    int getHealth();
    void subHealth(int damage);
    bool isAlive();
    unordered_map<string, float> &getResistances();
    const string &getID();
    virtual attack &getAttack()
    {
        return attacks[0];
    }
};

class player : public entity
{
protected:
    bool isGuard = false;
    int baseHealth;

public:
    player(int health = 100,
           vector<attack> attacks = {{"Strong Slash", "Slash", 60}, {"Thrust", "Pierce", 60}, {"Fireball", "Magic", 60}},
           unordered_map<string, float> resistances = {{"Slash", 0.5}, {"Pierce", 0.3}, {"Magic", 0.3}},
           string id = "P1")
        : entity(id, health, attacks, resistances)
    {
        baseHealth = health;
    }
    ~player() {}
    bool stateGuard();
    void changeGuard();
    void reset();
    virtual attack &getAttack();
};

class knight : public entity
{
public:
    knight(int idNum,
           int health = 75,
           vector<attack> attacks = {{"Weak Slash", "Slash", 10}, {"Thrust", "Pierce", 15}, {"Focused Slash", "Slash", 30}},
           unordered_map<string, float> resistances = {{"Slash", 0.5}, {"Pierce", 0.2}, {"Magic", 0.2}});
    virtual attack &getAttack();
};

class mage : public entity
{
public:
    mage(int idNum,
         int health = 50,
         vector<attack> attacks = {{"Incinerate", "Magic", 25}, {"Smite", "Magic", 40}, {"Ice Shard", "Pierce", 15}},
         unordered_map<string, float> resistances = {{"Slash", 0.1}, {"Pierce", 0.1}, {"Magic", 0.5}});
    virtual attack &getAttack();
};

class archer : public entity
{
public:
    archer(int idNum,
           int health = 50,
           vector<attack> attacks = {{"Weak Shot", "Pierce", 15}, {"Charged Shot", "Pierce", 25}, {"Rain of Arrows", "Pierce", 40}},
           unordered_map<string, float> resistances = {{"Slash", 0.2}, {"Pierce", 0.5}, {"Magic", 0.1}});
    virtual attack &getAttack();
};
#endif