#ifndef GAME_H
#define GAME_H
#include <string>
#include <vector>
#include "entities.h"

using namespace std;

void printTextByCharacter(const std::string &text, int delay_ms);

void printStream(ostringstream &stream);

void getSingleInput(int &input);
void getSingleInput(char &input);

int calcDamage(int raw, entity *target);

entity *getTarget(vector<entity *> &enemies);

void printInfo(entity *player, vector<entity *> &enemies, int turn);

int attackTarget(entity *target, entity *attacker);

void killTarget(entity *target, vector<entity *> &enemies);

char optionMenu();

#endif