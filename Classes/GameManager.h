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
    void setupGame();
    void startGame();
    void endGame();
    bool pregameIsActive();
    bool gameIsActive();
    bool titleScreenIsActive();
    long double getCurrentTimeSeconds();
    long double getElapsed();
    int numPlayers, maxPlayers, goalCheckpoints, currentState;
    long double ttime, startTime;
    
    enum kScreenState{
        kTitleState, kPreGameState, kInGameState, kGameOverState
    };
};




#endif
