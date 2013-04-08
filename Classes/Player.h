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

class Player {
    
public:
    ccColor3B color;
    SquareTarget *currentTarget;
    bool touchLock;
    int checkpointCount;
    CCTouch *touch;
    
    Player();
    void init();
    void spawnNewTargetWithLayer(CCLayer * layer);
    void killOldTarget();
    int getID();
    
private:
    int _identifier;
    
    void unlockTouch();
};

#endif /* defined(__CocosTest__Player__) */
