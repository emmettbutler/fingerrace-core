//
//  GameManager.h
//  FingerRace
//
//  Created by Emmett Butler on 4/4/13.
//
//

#ifndef __FingerRace__GameManager__
#define __FingerRace__GameManager__

#include <list>

class Player;

class GameManager
{
public:
    std::list<Player *> *players;
    static GameManager *sharedManager();
    void init();
    bool gameIsActive;
    int numPlayers, goalCheckpoints;
};




#endif
