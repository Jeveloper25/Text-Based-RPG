#ifndef GAME_H
#define GAME_H
#include <string>
#include <vector>
#include <memory>
#include "entities.h"

using namespace std;

unsigned int getRandom(int max);

//// DISPLAY FUNCTIONS ////

void printLine(int size);

void printTextByCharacter(const std::string &text, int delay_ms);

void printStream(ostringstream &stream);

void getSingleInput(int &input);
void getSingleInput(char &input);

int mainMenu();

int postCombat(player &p, double &totalExp);

void selectLoot(unordered_map<string, shared_ptr<weapon>> &lootTable, player &p);

//// COMBAT FUNCTIONS ////

double calcDamage(int raw, const string &dType, entity &target);

unique_ptr<entity> *getTarget(vector<unique_ptr<entity>> &enemies);

void printInfo(entity &player, vector<unique_ptr<entity>> &enemies, int turn);

attack attackTarget(entity &target, entity &attacker);

void killTarget(entity &target, vector<unique_ptr<entity>> &enemies, double &totalExp);

char startPlayerTurn(player &p, int turn);

char optionMenu();

void playerAttack(player &p, vector<unique_ptr<entity>> &enemies, double &totalExp);

void enemyAttack(player &p, vector<unique_ptr<entity>> &enemies, int turn);

//// SETUP FUNCTIONS ////

void populateEnemies(vector<unique_ptr<entity>> &enemies, int numEnemies, int combatLevel);

void determineLoot(unordered_map<string, shared_ptr<weapon>> &lootTable, vector<unique_ptr<entity>> &enemies);

void equipmentSelect(player &p);

#endif