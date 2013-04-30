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

#ifdef TARGET_OS_IPHONE
#define ROBOTO_FONT "Roboto-Condensed"
#else
#define ROBOTO_FONT "Roboto-Condensed.ttf"
#endif

class Player;
class SquareTarget;
class ScoreCounter;

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
    void setTitleState();
    bool gameIsActive();
    bool titleScreenIsActive();
    bool endgameScreenIsActive();
    long double getCurrentTimeSeconds();
    long double timeSinceLastStateChange();
    long double getElapsed();
    CCLayer *baseLayer;
    int numPlayers, maxPlayers, goalCheckpoints, currentState;
    long double ttime, startTime, lastStateChangeTime;
    bool tutorialActive;
    
    void resetGameState();
    
    std::list<ccColor3B> *allowedColors;
    std::list<ccColor3B> *usedColors;
    std::list<CCPoint> *counterPositions;
    std::list<CCPoint> *usedCounterPositions;

    enum kScreenState{
        kTitleState, kPreGameState, kInGameState, kGameOverState
    };
    
    ccColor3B getNextColor();
    void resetColors();
    
    CCPoint getNextScoreCounterPosition();
    void resetCounterPositions();
    void setupCounterPositions(CCLayer *b);

    void initStats();
    std::vector<int> *winCounts;
};




#endif
