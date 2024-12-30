#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include <vector>
#include <algorithm>
#include "entities.h"
#include "game.h"

#define TEXTDELAY 15

using namespace std;

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
    cin >> input;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
void getSingleInput(char &input)
{
    cin >> input;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int calcDamage(int raw, entity &target)
{
    return (raw * (1 - target.getDefense()));
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
        if (input < 0 || input > (int)enemies.size())
        {
            stream << "Invalid input";
            printStream(stream);
            getSingleInput(input);
        }
    } while (input < 0 || input > (int)enemies.size());
    return &enemies[input - 1];
}

void printInfo(entity &player, vector<unique_ptr<entity>> &enemies, int turn)
{
    ostringstream stream;
    if (turn % 2 == 0)
    {
        stream << "Player's turn\n";
    }
    else
    {
        stream << "Enemy's turn\n";
    }
    stream << "HEALTH Player: " << player.getHealth() << "\n";
    for (unique_ptr<entity> &en : enemies)
    {
        stream << "HEALTH " << en->getID() << ": " << en->getHealth() << "\n";
    }
    printStream(stream);
}

int attackTarget(entity &target, entity &attacker)
{
    int damage = calcDamage(attacker.getAttack(), target);
    target.subHealth(damage);
    return damage;
}

void killTarget(entity &target, vector<unique_ptr<entity>> &enemies)
{
    ostringstream stream;
    for (auto en = enemies.begin(); en != enemies.end(); en++)
    {
        if (target.getID() == (*en)->getID())
        {
            enemies.erase(en);
            break;
        }
    }
    stream << target.getID() << " has died!\n";
    printStream(stream);
}

char optionMenu()
{
    ostringstream stream;
    vector<char> options = {'A', 'G'};
    char option;
    stream << "What would you like to do?\nATTACK(A)\nGUARD(G)\n";
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