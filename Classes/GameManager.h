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
    CCPoint PH_P1TPOS, PH_P2TPOS, TAB_P1TPOS, TAB_P2TPOS, TAB_P3TPOS, TAB_P4TPOS,           // title sprite positions for phone and tablet
            PH_P1SCPOS, PH_P2SCPOS, TAB_P1SCPOS, TAB_P2SCPOS, TAB_P3SCPOS, TAB_P4SCPOS;     // score counter positions
    float PH_TSCY, PH_TSCX, TAB_TSCX, TAB_TSCY;  // x and y title sprite scales for phone and tablet
    int numPlayers, maxPlayers, goalCheckpoints, currentState, matchPoints;
    long double ttime, startTime, lastStateChangeTime;
    float queueingTime;
    bool tutorialActive;
    
    void resetGameState();
    bool tabletDevice();
    bool retinaDevice();
    
    std::list<ccColor3B> *allowedColors;
    std::list<ccColor3B> *usedColors;

    enum kScreenState{
        kTitleState, kPreGameState, kInGameState, kGameOverState
    };
    
    enum kSpriteTag{
        kMotionBlurTag
    };
    
    enum kPlayerId{
        kPlayer1=0, kPlayer2=1, kPlayer3=2, kPlayer4=3
    };
    
    ccColor3B getNextColor();
    void resetColors();
    
    CCPoint getScoreCounterPosition(int i);
    CCPoint getLayoutScale();
    CCPoint getLayoutPosition(int i);
    void setupCounterPositions(CCLayer *b);

    void initStats();
    std::vector<int> *winCounts;

    bool firstRun();
    
    void setScaleFactor(float sf);
    float getScaleFactor();
    
private:
    float scaleFactor;
};




#endif
