#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "entities.h"
#include "game.h"
#include "items.h"

#define PLAYER_TURN 0

using namespace std;

// BELOW ARE HELPER FUNCTIONS FOR MAIN

/*
    function for sort algorithm, compares ID's (strings)
*/
bool cmp_by_id(const unique_ptr<entity> &a, const unique_ptr<entity> &b)
{
    return a->getID() < b->getID();
}

/*
    Prints Main menu and returns user's choice
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
    Resets player's guard and regens stamina, also gets first option
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

/*
    Option A of Player's turn
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
    Processes enemy turn
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

/*
    Post combat menus
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

int main()
{
    ostringstream stream;

    unique_ptr<player> p1 = make_unique<player>();
    vector<unique_ptr<entity>> enemies;

    double totalExp = 0;
    int combatLevel = 0;
    int turn = 0;

    if (mainMenu() == 1)
    {
        bool playGame = true;
        while (playGame)
        {
            populateEnemies(enemies, 3, combatLevel);
            sort(enemies.begin(), enemies.end(), cmp_by_id);
            // COMBAT LOOP
            while (p1->isAlive() && !(enemies.empty()))
            {
                if (turn != 0 && turn % 2 == PLAYER_TURN)
                {
                    p1->changeStam(true);
                }
                printInfo(*p1, enemies, turn);

                // PLAYER'S TURN
                if (turn % 2 == PLAYER_TURN)
                {
                    switch (startPlayerTurn(*p1, turn))
                    {
                    case 'A':
                        playerAttack(*p1, enemies, totalExp);
                        break;
                    case 'G':
                        p1->changeGuard();
                        stream << "You ready yourself for the next attack!\n";
                        printStream(stream);
                        break;
                    }
                }

                // ENEMY TURN
                else
                {
                    enemyAttack(*p1, enemies, turn);
                }
                turn++;
            }

            switch (postCombat(*p1, totalExp))
            {
            // PLAY AGAIN
            case 1:
                turn = 0;
                combatLevel++;
                if (p1->gainExp(totalExp))
                {
                    stream << "You have leveled up!";
                    printStream(stream);
                }
                stream << "Current level: " << p1->getLevel() + 1 << "(" << p1->getExp() << " / " << p1->getExpThreshold() << ")";
                printStream(stream);
                p1->reset();
                totalExp = 0;
                break;
            // EXIT GAME
            case 2:
                stream << "\nExiting game";
                printStream(stream);
                printLine(50);
                playGame = false;
                break;
            }
        }
    }

    return 1;
}