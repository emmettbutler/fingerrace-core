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
#include "cocos2d.h"
using namespace cocos2d;

class Player;
class SquareTarget;

class GameManager
{
public:
    std::list<Player *> *players;
    static GameManager *sharedManager();
    void init();
    void startGame();
    void endGame();
    void setTitleState();
    bool gameIsActive();
    bool titleScreenIsActive();
    bool endgameScreenIsActive();
    long double getCurrentTimeSeconds();
    long double timeSinceLastStateChange();
    long double getElapsed();
    int numPlayers, maxPlayers, goalCheckpoints, currentState;
    long double ttime, startTime, lastStateChangeTime;
    
    void resetGameState();
    
    std::list<ccColor3B> *allowedColors;
    std::list<ccColor3B> *usedColors;
    
    enum kScreenState{
        kTitleState, kInGameState, kGameOverState
    };
    
    ccColor3B getNextColor();
    void resetColors();
};




#endif
