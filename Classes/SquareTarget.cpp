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

/*!
 * SquareTarget constructor. Clearly does nothing.
 */
SquareTarget::SquareTarget() {
    
}

/*!
 * Initializes sprite as a child of a given player.
 *
 * @param p Player for which a target is being generated.
 */
void SquareTarget::initWithPlayer(Player *p) {
    this->initWithFile("square_round.png");
    this->setScale(p->baseScale);
    this->setColor(p->color);
}

void SquareTarget::wasTouched() {

}
