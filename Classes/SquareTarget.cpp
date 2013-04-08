//
//  SquareTarget.cpp
//  CocosTest
//
//  Created by Evan Moore on 4/2/13.
//
//

#import "cocos2d.h"
using namespace cocos2d;

#include "SquareTarget.h"
#include "Player.h"

SquareTarget::SquareTarget() {
    
}

void SquareTarget::initWithPlayer(Player *p) {
    this->initWithFile("square.png");
    this->setScale(4);
    this->setColor(p->color);
}

void SquareTarget::wasTouched() {

}
