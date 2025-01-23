#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "items.h"
#include "entities.h"
#include "game.h"

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
    MAIN FUNCTION
*/
int main()
{
    ostringstream stream;

    unique_ptr<player> p1 = make_unique<player>();
    vector<unique_ptr<entity>> enemies;
    unordered_map<string, shared_ptr<weapon>> lootTable;

    double totalExp = 0;
    int combatLevel = 0;
    int turn = 0;

    if (mainMenu() == 1)
    {
        bool playGame = true;
        while (playGame)
        {
            equipmentSelect(*p1);
            populateEnemies(enemies, 3, combatLevel);
            sort(enemies.begin(), enemies.end(), cmp_by_id);
            determineLoot(lootTable, enemies);
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
                    case 'U':
                        itemSelect(*p1);
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
                printLine(40);
                if (p1->gainExp(totalExp))
                {
                    stream << "You have leveled up!";
                    printStream(stream);
                }
                stream << "Current level: " << p1->getLevel() + 1 << "(" << p1->getExp() << " / " << p1->getExpThreshold() << ")";
                printStream(stream);
                selectLoot(lootTable, *p1);
                consumableLoot(*p1, combatLevel + 1);
                p1->reset();
                lootTable.clear();
                enemies.clear();
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