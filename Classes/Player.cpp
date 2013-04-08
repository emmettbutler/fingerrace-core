//
//  Player.cpp
//  CocosTest
//
//  Created by Evan Moore on 4/2/13.
//
//

#import "cocos2d.h"
using namespace cocos2d;

#import "SquareTarget.h"
#include "Player.h"

Player::Player() {
    this->currentTarget = NULL;
}

void Player::init(){
    this->color = ccc3(arc4random() % 255, arc4random() % 255, arc4random() % 255);
    this->touchLock = false;
    this->checkpointCount = 0;
    this->_identifier = 11011;
    this->touch = NULL;
}

void Player::spawnNewTargetWithLayer(CCLayer * layer) {
    if(this->touchLock) return;
    
    CCPoint randPos = *new CCPoint(arc4random() % (int)layer->boundingBox().size.width, arc4random() % (int)layer->boundingBox().size.height);
    
    if(this->currentTarget == NULL){
        this->currentTarget = new SquareTarget();
        this->currentTarget->initWithPlayer(this);
        this->currentTarget->setPosition(randPos);
        layer->addChild(this->currentTarget);
        return;
    }
    
    this->currentTarget->runAction(
        CCSequence::actions(
            CCMoveTo::actionWithDuration(.05, randPos),
            // to get this to work, this class needs to conform to the selector_protocol.h?
            //CCCallFunc::actionWithTarget((CCObject *)this, callfunc_selector(Player::unlockTouch)),
            NULL
        )
    );
}

void Player::unlockTouch(){
    this->touchLock = false;
}

int Player::getID(){
    return this->_identifier;
}
