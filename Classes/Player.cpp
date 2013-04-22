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
#include "GameManager.h"

Player::Player() {
    this->currentTarget = NULL;
}

bool Player::init(int id){
    this->color = ccc3(arc4random() % 255, arc4random() % 255, arc4random() % 255);
    this->touchLock = false;
    this->checkpointCount = 0;
    this->_identifier = id;
    this->touch = NULL;
    this->baseScale = 4;
    this->remainingCheckpoints = GameManager::sharedManager()->goalCheckpoints - this->checkpointCount;
    
    this->scoreLabel = CCLabelTTF::labelWithString("10", "Courier New", 20);
    this->scoreLabel->setColor(this->color);

    this->initWithFile("circle.png");
    this->setColor(this->color);

    return true;
}

void Player::initScoreLabel(CCLayer *parent){
    CCPoint pos = this->currentTarget->getPosition();
    this->scoreLabel->setPosition(CCPoint(pos.x, this->currentTarget->boundingBox().getMaxY()+10));
    parent->addChild(scoreLabel);
}

void Player::spawnNewTarget(CCPoint position, CCLayer * layer) {
    if(this->touchLock) return;

    if(this->currentTarget == NULL){
        this->currentTarget = new SquareTarget();
        this->currentTarget->initWithPlayer(this);
        this->currentTarget->setPosition(position);
        layer->addChild(this->currentTarget);
        return;
    }
    
    this->currentTarget->runAction(
        CCSequence::actions(
            CCMoveTo::actionWithDuration(.05, position),
            NULL
        )
    );
    
    float halfHeight = this->currentTarget->boundingBox().getMaxY() - this->currentTarget->boundingBox().getMidY();
    
    char buff[3];
    sprintf(buff, "%d", this->remainingCheckpoints);
    this->scoreLabel->setString(buff);
    
    this->scoreLabel->runAction(
        CCSequence::actions(
            CCMoveTo::actionWithDuration(.05, CCPoint(position.x, position.y+halfHeight+10)),
            NULL
        )
    );
}

void Player::shrinkTarget(){
    if(this->currentTarget->getScale() > this->baseScale*.5){
        this->currentTarget->runAction(
            CCSequence::actions(
                CCScaleBy::actionWithDuration(.5, .9),
                NULL
            )
        );
    }
}

void Player::growTarget(){
    if(this->currentTarget->getScale() < this->baseScale){
        this->currentTarget->runAction(
            CCSequence::actions(
                CCScaleBy::actionWithDuration(.5, 1.1),
                NULL
            )
        );
    }
}

void Player::updatePosition(CCPoint glPosition) {
    if (this->touch != NULL) {
        this->setPosition(glPosition);
    }
}

int Player::getID(){
    return this->_identifier;
}
