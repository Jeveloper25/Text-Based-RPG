#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include <vector>
#include <algorithm>
#include <memory>
#include <random>
#include "entities.h"
#include "game.h"

#define TEXTDELAY 15

using namespace std;

// RANDOM NUMBER GENERATOR
// *INCLUDES MAX*
unsigned int getRandom(int max)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, max);
    return dist(gen);
}

// DISPLAY FUNCTIONS

void printLine(int size)
{
    ostringstream stream;
    string line(size, '~');
    stream << line;
    printStream(stream);
}

void printTextByCharacter(const std::string &text, int delay_ms)
{
    for (char c : text)
    {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    }
    std::cout << std::endl;
}

void printStream(ostringstream &stream)
{
    printTextByCharacter(stream.str(), TEXTDELAY);
    stream.str("");
    stream.clear();
}

void getSingleInput(int &input)
{
    cout << ">>> ";
    cin >> input;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
void getSingleInput(char &input)
{
    cout << ">>> ";
    cin >> input;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

char optionMenu()
{
    ostringstream stream;
    vector<char> options = {'A', 'G'};
    char option;
    stream << "What would you like to do?\n(A)ATTACK\n(G)GUARD\n";
    printStream(stream);
    getSingleInput(option);
    option = (char)toupper(option);
    do
    {
        if (find(options.begin(), options.end(), option) == options.end())
        {
            stream << "Invalid input\n";
            printStream(stream);
            getSingleInput(option);
            option = (char)toupper(option);
        }
    } while (find(options.begin(), options.end(), option) == options.end());
    return option;
}

void printInfo(entity &player, vector<unique_ptr<entity>> &enemies, int turn)
{
    ostringstream stream;
    string line(20, '-');
    printLine(40);
    if (turn % 2 == 0)
    {
        stream << "Player's turn\n\n";
    }
    else
    {
        stream << "Enemy's turn\n\n";
    }
    stream << line << "\nPlayer" << "\nLEVEL: " << player.getLevel() + 1 << "\nHEALTH: " << player.getHealth() << "\nSTAMINA: " << player.getStam() << "\n"
           << line << "\n";
    for (unique_ptr<entity> &en : enemies)
    {
        stream << en->getID() << "\nLEVEL: " << en->getLevel() + 1 << "\nHEALTH: " << en->getHealth() << "\n"
               << line << "\n";
    }
    printStream(stream);
    printLine(40);
}

// COMBAT FUNCTIONS

double calcDamage(int raw, const string &dType, entity &target)
{
    unordered_map<string, float> resistances = target.getResistances();
    return raw * (1 - resistances.at(dType));
}

unique_ptr<entity> *getTarget(vector<unique_ptr<entity>> &enemies)
{
    ostringstream stream;
    int input;
    int count = 1;
    stream << "Select a target:\n";
    for (unique_ptr<entity> &en : enemies)
    {
        stream << "(" << count << ") " << en->getID() << "\n";
        count++;
    }
    printStream(stream);
    getSingleInput(input);
    do
    {
        if (input < 1 || input > (int)enemies.size())
        {
            stream << "Invalid input";
            printStream(stream);
            getSingleInput(input);
        }
    } while (input < 1 || input > (int)enemies.size());
    printLine(40);
    return &enemies.at(input - 1);
}

attack attackTarget(entity &target, entity &attacker)
{
    attack act = attacker.getAttack();
    int raw = attacker.getRaw();
    double damage = calcDamage(act.damageMultiplier * raw, act.dType, target);
    target.subHealth(damage);
    attacker.changeStam(false, act.staminaCost);
    attack info{act.name, act.dType, damage, 0};
    return info;
}

void killTarget(entity &target, vector<unique_ptr<entity>> &enemies, double &totalExp)
{
    ostringstream stream;
    stream << target.getID() << " has died!\n";
    for (auto en = enemies.begin(); en != enemies.end(); en++)
    {
        if (target.getID() == (*en)->getID())
        {
            totalExp += target.getExp();
            enemies.erase(en);
            break;
        }
    }
    printStream(stream);
}

// SETUP FUNCTIONS

void populateEnemies(vector<unique_ptr<entity>> &enemies, int numEnemies, int combatLevel)
{
    vector<int> idNums = {1, 1, 1};
    int eType;
    for (int i = 0; i < numEnemies; i++)
    {
        eType = getRandom(2);
        switch (eType)
        {
        case 0:
            enemies.emplace_back(make_unique<knight>(idNums.at(0), combatLevel));
            idNums.at(0)++;
            break;
        case 1:
            enemies.emplace_back(make_unique<mage>(idNums.at(1), combatLevel));
            idNums.at(1)++;
            break;
        case 2:
            enemies.emplace_back(make_unique<archer>(idNums.at(2), combatLevel));
            idNums.at(2)++;
            break;
        }
    }
}