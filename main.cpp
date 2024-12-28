#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include <vector>
#include "entities.h"
#include "game.h"

using namespace std;

int main()
{
    ostringstream stream;

    player *p1 = new player(100, 60, 0.3);
    mage *m1 = new mage(50, 30, 0.1, 1);
    mage *m2 = new mage(50, 30, 0.1, 2);
    knight *k1 = new knight(75, 20, 0.3, 1);
    vector<entity *> enemies({m1, m2, k1});
    entity *target;
    int damage;

    int turn = 0;
    char option;

    while (p1->isAlive() && !(enemies.empty()))
    {
        printInfo(p1, enemies, turn);
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
                damage = attackTarget(target, p1);
                stream << "You dealt " << damage << "damage!\n";
                printStream(stream);
                if (!target->isAlive())
                {
                    killTarget(target, enemies);
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
            for (entity *en : enemies)
            {
                damage = attackTarget(p1, en);
                stream << en->getID() << " dealt " << damage << "damage!\n";
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