//
//  ScoreCounter.h
//  FingerRace
//
//  Created by Emmett Butler on 4/23/13.
//
//

#ifndef __FingerRace__ScoreCounter__
#define __FingerRace__ScoreCounter__

#include "cocos2d.h"
using namespace cocos2d;

class Player;

class ScoreCounter : public CCLayer
{
public:
    Player *player;
    int numPoints;
    CCSprite *current;
    std::list<CCSprite *> *pointSprites;
    
    bool init(int goalCheckpoints, ccColor3B c, Player *p);
    void removePoint();
    void addPoint();
};

#endif /* defined(__FingerRace__ScoreCounter__) */
