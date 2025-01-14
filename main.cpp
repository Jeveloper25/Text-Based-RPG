#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "entities.h"
#include "game.h"
#include "items.h"

using namespace std;

bool cmp_by_id(const unique_ptr<entity> &a, const unique_ptr<entity> &b)
{
    return a->getID() < b->getID();
}

int main()
{
    ostringstream stream;

    unique_ptr<player> p1 = make_unique<player>();
    vector<unique_ptr<entity>> enemies;
    unique_ptr<entity> *target;
    attack attackInfo; // dM for attackInfo is the damage dealt

    double totalExp = 0;
    int combatLevel = 0;
    int turn = 0;
    char option;
    int main_option;

    printLine(50);
    stream << "\t\t\tWelcome";
    printStream(stream);
    printLine(50);
    stream << "Select an option:\n(1)Start Game\n(2)Exit\n";
    printStream(stream);
    getSingleInput(main_option);
    do
    {
        if (main_option < 1 || main_option > 2)
        {
            stream << "Invalid input";
            printStream(stream);
            getSingleInput(main_option);
        }
    } while (main_option < 1 || main_option > 2);

    if (main_option == 1)
    {
        while (true)
        {
            populateEnemies(enemies, 3, combatLevel);
            sort(enemies.begin(), enemies.end(), cmp_by_id);
            // COMBAT LOOP
            while (p1->isAlive() && !(enemies.empty()))
            {
                printInfo(*p1, enemies, turn);
                if (turn % 2 == 0)
                {
                    if (p1->stateGuard())
                    {
                        p1->changeGuard();
                    }
                    option = optionMenu();

                    if (option == 'A')
                    {
                        target = getTarget(enemies);
                        attackInfo = attackTarget(**target, *p1);
                        stream << "You used " << attackInfo.name << "!\n"
                               << "You dealt " << (int)attackInfo.damageMultiplier << " damage to " << (*target)->getID() << "!\n";
                        printStream(stream);
                        if (!(*target)->isAlive())
                        {
                            killTarget(**target, enemies, totalExp);
                        }
                    }
                    else if (option == 'G')
                    {
                        p1->changeGuard();
                        stream << "You ready yourself for the next attack!\n";
                        printStream(stream);
                    }
                }
                else
                {
                    for (unique_ptr<entity> &en : enemies)
                    {
                        attackInfo = attackTarget(*p1, *en);
                        stream << en->getID() << " used " << attackInfo.name << "!\n"
                               << en->getID() << " dealt " << (int)attackInfo.damageMultiplier << " damage!\n";
                        printStream(stream);
                    }
                }
                turn++;
            }

            // POST_COMBAT
            if (p1->isAlive())
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
            stream << "Would you like to fight again?\n(1)Yes\n(2)Quit game\n";
            printStream(stream);
            getSingleInput(main_option);
            do
            {
                if (main_option < 1 || main_option > 2)
                {
                    stream << "Invalid input";
                    printStream(stream);
                    getSingleInput(main_option);
                }
            } while (main_option < 1 || main_option > 2);
            if (main_option == 1)
            {
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
            }
            else if (main_option == 2)
            {
                stream << "\nExiting game";
                printStream(stream);
                printLine(50);
                break;
            }
        }
    }
    return 1;
}