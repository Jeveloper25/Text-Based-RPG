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

    populateEnemies(enemies, 3);
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
        stream << "All enemies have died!\np1 wins!";
        printStream(stream);
    }
    else
    {
        stream << "You have died!\nGame Over!";
        printStream(stream);
    }

    return 1;
}