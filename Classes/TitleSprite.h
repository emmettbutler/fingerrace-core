//
//  TitleSprite.h
//  FingerRace
//
//  Created by Emmett Butler on 5/2/13.
//
//

#ifndef __FingerRace__TitleSprite__
#define __FingerRace__TitleSprite__

#include "cocos2d.h"
using namespace cocos2d;

class TitleSprite : public cocos2d::CCSprite {
    cocos2d::ccColor3B color;
    
public:
    TitleSprite();
    
    CCTouch *touch;
    CCSprite *button;
};

#endif /* defined(__FingerRace__TitleSprite__) */
