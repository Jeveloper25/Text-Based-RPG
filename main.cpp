#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "entities.h"
#include "game.h"

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
    // enemies.emplace_back(make_unique<mage>(1));
    // enemies.emplace_back(make_unique<archer>(1));
    // enemies.emplace_back(make_unique<knight>(1));
    unique_ptr<entity> *target;
    attack attackInfo;

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
            populateEnemies(enemies, 1);
            sort(enemies.begin(), enemies.end(), cmp_by_id);
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
                               << "You dealt " << attackInfo.damage << " damage to " << (*target)->getID() << "!\n";
                        printStream(stream);
                        if (!(*target)->isAlive())
                        {
                            killTarget(**target, enemies);
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
                               << en->getID() << " dealt " << attackInfo.damage << "damage!\n";
                        printStream(stream);
                    }
                }
                turn++;
            }
            if (p1->isAlive())
            {
                stream << "All enemies have died!\np1 wins!\n";
                printStream(stream);
            }
            else
            {
                stream << "You have died!\nGame Over!\n";
                printStream(stream);
            }
            // ADD BREAK
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
                p1->reset();
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