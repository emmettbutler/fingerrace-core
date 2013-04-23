#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;

class Player;

class HelloWorld : public CCLayer{
public:
    virtual bool init();
    static CCScene* scene();
    CREATE_FUNC(HelloWorld);
    virtual void ccTouchesBegan(CCSet *touches, CCEvent *event);
    virtual void ccTouchesMoved(CCSet *touches, CCEvent *event);
    virtual void ccTouchesEnded(CCSet *touches, CCEvent *event);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void tick(float dt);
    void resolveTargetCollision();
    void setupTitleScreen();
    void setupTitleScreenTextOverlay();
    void setupTitleScreenFromEndgameScreen();
    void dismissTitleScreen();
    void dismissEndgameScreen();
    void setupGameScreen();
    void setupEndgameScreen(Player *winner);
    void setupEndgameScreenTextOverlay();
    CCPoint nextTargetPosition(Player *p);
    void adjustTargetSize(Player *p);
    Player *currentWinner();
    int scoreTotal();
    std::list<CCSprite *> *titleSprites;
    
    int numQueuedPlayers;
    CCLayer *titleLayer, *endgameLayer;
    long double lastPlayerQueueTime, startTime, ttime;
};

#endif // __HELLOWORLD_SCENE_H__
