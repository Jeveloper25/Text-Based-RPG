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

#define TEXTDELAY 5

using namespace std;

/**
 * RANDOM NUMBER GENERATOR
 * *INCLUDES MAX*
 */
unsigned int getRandom(int max)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, max);
    return dist(gen);
}

//// DISPLAY FUNCTIONS ////

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

/**
 * Prints important info about player and enemies at the start of each turn
 */
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

/*
 *   Prints Main menu and returns user's choice
 */
int mainMenu()
{
    ostringstream stream;
    int mainOption;

    printLine(50);
    stream << "\t\t\tWelcome";
    printStream(stream);
    printLine(50);
    stream << "Select an option:\n(1)Start Game\n(2)Exit\n";
    printStream(stream);
    getSingleInput(mainOption);

    do
    {
        if (mainOption < 1 || mainOption > 2)
        {
            stream << "Invalid input";
            printStream(stream);
            getSingleInput(mainOption);
        }
    } while (mainOption < 1 || mainOption > 2);

    return mainOption;
}

/*
 *    Post combat menus
 */
int postCombat(player &p, double &totalExp)
{
    ostringstream stream;
    int postOption;

    // POST_COMBAT INFO
    if (p.isAlive())
    {
        stream << "All enemies have died!\np1 wins!\n";
        printStream(stream);
    }
    else
    {
        stream << "You have died!\nGame Over!\n";
        printStream(stream);
        totalExp = 0;
    }

    // POST COMBAT INPUT
    stream << "Would you like to fight again?\n(1)Yes\n(2)Quit game\n";
    printStream(stream);
    getSingleInput(postOption);

    do
    {
        if (postOption < 1 || postOption > 2)
        {
            stream << "Invalid input";
            printStream(stream);
            getSingleInput(postOption);
        }
    } while (postOption < 1 || postOption > 2);

    return postOption;
}

/**
 * Displays and gets player option for dropped loot
 */
void selectLoot(unordered_map<string, shared_ptr<weapon>> &lootTable, player &p)
{
    ostringstream stream;
    int option;
    int counter = 0;
    vector<shared_ptr<weapon>> selection;

    if (lootTable.size() != 0)
    {
        stream << "Select a reward:\n\n";
        for (auto [name, wep] : lootTable)
        {
            counter++;
            stream << "(" << counter << ") " << name << "\n"
                   << "Raw damage: " << wep->getDamage() << "\n"
                   << "Attacks: ";
            selection.push_back(wep);
            for (attack &at : wep->getAttacks())
            {
                stream << at.name << "   ";
            }
            printStream(stream);
        }

        getSingleInput(option);

        do
        {
            if (option <= 0 || option > counter)
            {
                stream << "Invalid input";
                printStream(stream);
                getSingleInput(option);
            }
        } while (option <= 0 || option > counter);

        p.insertInventory(selection.at(option - 1));
    }
}

//// COMBAT FUNCTIONS ////

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

/*
 *  Resets player's guard and regens stamina, also gets first option
 */
char startPlayerTurn(player &p, int turn)
{
    ostringstream stream;
    char option;

    if (p.stateGuard())
    {
        p.changeGuard();
    }

    option = optionMenu();

    do
    {
        if (!p.checkStam() && option == 'A')
        {
            stream << "Insufficient stamina";
            printStream(stream);
            getSingleInput(option);
        }
    } while (!p.checkStam() && option == 'A');
    printLine(40);
    return option;
}

/**
 * Gets player decision for their turn
 */
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

/*
 *   Option A of Player's turn
 */
void playerAttack(player &p, vector<unique_ptr<entity>> &enemies, double &totalExp)
{
    ostringstream stream;
    unique_ptr<entity> *target;
    attack attackInfo; // dM for attackInfo is the damage dealt, stamCost always 0(not displayed)

    target = getTarget(enemies);
    attackInfo = attackTarget(**target, p);
    stream << "You used " << attackInfo.name << "!\n"
           << "You dealt " << (int)attackInfo.damageMultiplier << " damage to " << (*target)->getID() << "!\n";
    printStream(stream);
    if (!(*target)->isAlive())
    {
        killTarget(**target, enemies, totalExp);
    }
}

/*
 *   Processes enemy turn
 */
void enemyAttack(player &p, vector<unique_ptr<entity>> &enemies, int turn)
{
    ostringstream stream;
    attack attackInfo;

    for (unique_ptr<entity> &en : enemies)
    {
        if (turn != 1)
        {
            en->changeStam(true);
        }

        if (en->checkStam())
        {
            attackInfo = attackTarget(p, *en);
            stream << en->getID() << " used " << attackInfo.name << "!\n"
                   << en->getID() << " dealt " << (int)attackInfo.damageMultiplier << " damage!\n";
            printStream(stream);
        }
        else
        {
            stream << en->getID() << " passes their turn!\n";
            printStream(stream);
        }
    }
}

//// SETUP FUNCTIONS ////

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

void determineLoot(unordered_map<string, shared_ptr<weapon>> &lootTable, vector<unique_ptr<entity>> &enemies)
{
    shared_ptr<weapon> droppedWeapon;
    for (unique_ptr<entity> &en : enemies)
    {
        droppedWeapon = en->getLoot();
        if (droppedWeapon != nullptr)
        {
            lootTable.try_emplace(droppedWeapon->getName(), droppedWeapon);
        }
    }
}

void equipmentSelect(player &p)
{
    ostringstream stream;
    int option;
    int counter = 0;
    vector<shared_ptr<weapon>> selection;

    printLine(40);
    stream << "Select your weapon:\n\n";

    for (auto [name, wep] : p.getInv())
    {
        counter++;
        stream << "(" << counter << ")" << name << "\n"
               << "Damage: " << wep->getDamage() << "\n"
               << "Attacks: ";
        selection.push_back(wep);
        for (attack &at : wep->getAttacks())
        {
            stream << at.name << "   ";
        }
        stream << "\n\n";
    }
    printStream(stream);

    getSingleInput(option);

    do
    {
        if (option <= 0 || option > counter)
        {
            stream << "Invalid input";
            printStream(stream);
            getSingleInput(option);
        }
    } while (option <= 0 || option > counter);

    p.setCurrWeapon(selection.at(option - 1));
    printLine(40);
}