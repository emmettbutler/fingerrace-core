//
//  Player.h
//  CocosTest
//
//  Created by Evan Moore on 4/2/13.
//
//

#ifndef __CocosTest__Player__
#define __CocosTest__Player__

#include <iostream>

#import "cocos2d.h"
using namespace cocos2d;

class SquareTarget;
class ScoreCounter;

class Player : public CCSprite {
    
public:
    ccColor3B color;
    SquareTarget *currentTarget;
    CCSprite *shineSprite;
    CCLayer *parent;
    CCLabelTTF *scoreLabel, *tutMessage, *tutColorMessage, *grabNotify;
    bool touchLock, touchActive;
    ScoreCounter *scoreCounter;
    int checkpointCount, remainingCheckpoints;
    bool tut_touchHasEnded, tut_touchHasRestarted;
    float baseScale, velocity, opacityDelta, lastCheckpointTime;
    CCTouch *touch;
    CCRect territory;
    CCPoint startingPoint, homePoint;
    
    Player();
    bool init(CCPoint p, ccColor3B c, CCLayer *parent);
    void spawnNewTarget(CCPoint position);
    void killOldTarget();
    float timeSinceLastCheckpoint();
    void shrinkTarget();
    void growTarget();
    void gainPoint();
    void losePoint();
    void activateTouch(CCTouch *touch);
    void deactivateTouch();
    int getID();
    void initScoreLabel();
    void unlockTouch(CCNode *sender);
    void updateScoreText();
    void updatePosition(CCPoint glPosition);
    void initTerritory(CCRect screenBox);
    void initScoreCounter();
    
private:
    int _identifier;
    float activeColorScaleFactor;
};

#endif /* defined(__CocosTest__Player__) */
