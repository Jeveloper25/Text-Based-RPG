#ifndef GAME_H
#define GAME_H
#include <string>
#include <vector>
#include <memory>
#include "entities.h"

using namespace std;

unsigned int getRandom(int max);

void printLine(int size);

void printTextByCharacter(const std::string &text, int delay_ms);

void printStream(ostringstream &stream);

void getSingleInput(int &input);
void getSingleInput(char &input);

int calcDamage(int raw, const string &dType, entity &target);

unique_ptr<entity> *getTarget(vector<unique_ptr<entity>> &enemies);

void printInfo(entity &player, vector<unique_ptr<entity>> &enemies, int turn);

attack attackTarget(entity &target, entity &attacker);

void killTarget(entity &target, vector<unique_ptr<entity>> &enemies);

char optionMenu();

void populateEnemies(vector<unique_ptr<entity>> &enemies, int numEnemies);

#endif