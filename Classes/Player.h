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

class Player : CCNode {
    
public:
    ccColor3B color;
    SquareTarget *currentTarget;
    CCLabelTTF *scoreLabel;
    bool touchLock;
    int checkpointCount, remainingCheckpoints;
    float baseScale;
    CCTouch *touch;
    
    Player();
    void init();
    void spawnNewTarget(CCPoint position, CCLayer * layer);
    void killOldTarget();
    void shrinkTarget();
    void growTarget();
    int getID();
    void initScoreLabel(CCLayer *parent);
    void unlockTouch(CCNode *sender);
    
private:
    int _identifier;
};

#endif /* defined(__CocosTest__Player__) */
