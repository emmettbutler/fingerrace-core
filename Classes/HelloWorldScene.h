#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;

class Player;
class ScoreCounter;

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
    virtual void visit();
    void resolveTargetCollision();
    void setupTitleScreen();
    void setupTitleScreenTextOverlay(ccColor3B p1Color, ccColor3B p2Color);
    void setupTitleScreenFromEndgameScreen();
    void dismissTitleScreen();
    void dismissEndgameScreen();
    void setupGameScreen();
    void setupEndgameScreen(Player *winner);
    void setupEndgameScreenTextOverlay();
    void selectNextRenderTexture();
    CCPoint initialTargetPosition(Player *p);
    CCPoint nextTargetPosition(Player *p);
    void adjustTargetSize(Player *p);
    Player *currentWinner();
    CCSprite *tutButton;
    int scoreTotal();
    std::list<CCSprite *> *titleSprites;
    std::list<CCPoint> *titleTouchPoints;
    CCArray *renderTextures;
    
    int numQueuedPlayers, kRenderTextureCount, currentRenderTextureIndex;
    CCLayer *titleLayer, *endgameLayer, *gameBackgroundLayer;
    long double lastPlayerQueueTime, ttime;
};

#endif // __HELLOWORLD_SCENE_H__
