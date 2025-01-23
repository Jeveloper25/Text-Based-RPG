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
        stream << "\nSelect a reward:\n\n";
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
            stream << "\n";
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

        p.insertInv(selection.at(option - 1));
    }
}

/***
 * Uses RNG to determine what consumables are dropped
 * and immediately inserts them into the player's inventory.
 * roll determines how many times the program will check for drops.
 * FUTUREIMPROVEMENT: tie the possible drops to the enemies that appear.
 */
void consumableLoot(player &p, int rolls)
{
    int index;
    ostringstream stream;
    vector<string> lootTable = {"Health Potion", "Strength Potion", "Defense Potion"};
    stream << "\n";
    for (int i = 0; i < rolls; i++)
    {
        index = getRandom(lootTable.size() - 1);
        switch (getRandom(1))
        {
        case 1:
            p.insertInv(lootTable.at(index));
            stream << "You acquired a " << lootTable.at(index) << "!\n";
            break;
        default:
            break;
        }
    }
    printStream(stream);
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

    if (attacker.getID() == "P1")
    {
        damage *= 20;
        attacker.useBuff(damage);
    }
    else if (target.getID() == "P1")
    {
        damage *= 20;
        target.useBuff(damage);
    }

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

/***
 * Helper for validOption.
 * Checks player's item inventory for emptiness.
 */
bool validInv(player &p)
{
    for (auto [name, item] : p.getItems())
    {
        if (item->getCount() != 0)
        {
            return true;
        }
    }
    return false;
}

/***
 * Helper for startPlayerTurn.
 * Gets player input and checks for validity.
 */
bool validOption(char &option, player &p, ostringstream &stream)
{
    vector<char> valid = {'A', 'G', 'U'};
    getSingleInput(option);
    option = (char)toupper(option);
    do
    {
        if (find(valid.begin(), valid.end(), option) == valid.end())
        {
            stream << "Invalid input\n";
            printStream(stream);
            getSingleInput(option);
            option = (char)toupper(option);
        }
    } while (find(valid.begin(), valid.end(), option) == valid.end());

    switch (option)
    {
    case 'A':
        if (!p.checkStam())
        {
            stream << "Insufficient stamina";
            return false;
        }
        break;
    case 'U':
        if (!validInv(p))
        {
            stream << "No items to use";
            return false;
        }
        break;
    default:
        break;
    }
    return true;
}

/*
 *  Resets player's guard and regens stamina, also gets first option
 */
char startPlayerTurn(player &p, int turn)
{
    ostringstream stream;
    char option;
    bool valid;

    if (p.stateGuard())
    {
        p.changeGuard();
    }

    stream << "What would you like to do?\n(A)ATTACK\n(G)GUARD\n(U)USE ITEM\n";
    printStream(stream);

    do
    {
        valid = validOption(option, p, stream);
        if (!valid)
        {
            printStream(stream);
        }
    } while (!valid);

    printLine(40);
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

/***
 *  Option U of Player's turn
 */
void itemSelect(player &p)
{
    ostringstream stream;
    int option;
    int counter = 0;
    vector<shared_ptr<consumable>> selection;
    shared_ptr<consumable> chosen;

    for (auto [name, item] : p.getItems())
    {
        if (item->getCount() > 0)
        {
            counter++;
            stream << "(" << counter << ") " << name << "\n"
                   << "Effect: " << item->getEffect() << "\n";
            if (item->getCType() == "Buffer")
            {
                stream << "Duration: " << item->getDuration() << "\n";
            }

            stream << "Quantity: " << item->getCount() << "\n\n";
            selection.push_back(item);
        }
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

    chosen = selection.at(option - 1);
    stream << "You used a " << chosen->getName() << "!";
    printStream(stream);
    p.useItem(chosen->getName());
}

//// SETUP FUNCTIONS ////

void populateEnemies(vector<unique_ptr<entity>> &enemies, int maxEnemies, int maxLevel)
{
    int eType;
    int eLevel;
    vector<int> idNums = {1, 1, 1};
    int numEnemies = getRandom(maxEnemies - 1);
    for (int i = 0; i < numEnemies + 1; i++)
    {
        eType = getRandom(idNums.size() - 1);
        eLevel = getRandom(maxLevel);
        switch (eType)
        {
        case 0:
            enemies.emplace_back(make_unique<knight>(idNums.at(eType), eLevel));
            idNums.at(eType)++;
            break;
        case 1:
            enemies.emplace_back(make_unique<mage>(idNums.at(eType), eLevel));
            idNums.at(eType)++;
            break;
        case 2:
            enemies.emplace_back(make_unique<archer>(idNums.at(eType), eLevel));
            idNums.at(eType)++;
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

    for (auto [name, wep] : p.getWeps())
    {
        counter++;
        stream << "(" << counter << ") " << name << "\n"
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
}
