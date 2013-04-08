//
//  SquareTarget.h
//  CocosTest
//
//  Created by Evan Moore on 4/2/13.
//
//

#ifndef __CocosTest__SquareTarget__
#define __CocosTest__SquareTarget__

#include <iostream>

#import "cocos2d.h"

class Player;

class SquareTarget : public cocos2d::CCSprite {
    cocos2d::ccColor3B color;
    
public:
    SquareTarget();
    void initWithPlayer(Player *p);
    void wasTouched();
};

#endif /* defined(__CocosTest__SquareTarget__) */
