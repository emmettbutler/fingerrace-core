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
    CCLayer *parent;
    CCLabelTTF *scoreLabel;
    bool touchLock;
    ScoreCounter *scoreCounter;
    int checkpointCount, remainingCheckpoints;
    float baseScale, velocity;
    CCTouch *touch;
    CCRect territory;
    CCPoint startingPoint;
    
    Player();
    bool init(CCPoint p, ccColor3B c, CCLayer *parent);
    void spawnNewTarget(CCPoint position);
    void killOldTarget();
    void shrinkTarget();
    void growTarget();
    int getID();
    void initScoreLabel();
    void unlockTouch(CCNode *sender);
    void updateScoreText();
    void updatePosition(CCPoint glPosition);
    void initTerritory(CCRect screenBox);
    
private:
    int _identifier;
};

#endif /* defined(__CocosTest__Player__) */
