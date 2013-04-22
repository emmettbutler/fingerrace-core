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

bool Player::init(CCTouch *t, ccColor3B c){
    this->color = c;
    this->touchLock = false;
    this->checkpointCount = 0;
    this->touch = t;
    this->baseScale = 4;
    this->remainingCheckpoints = GameManager::sharedManager()->goalCheckpoints - this->checkpointCount;
    
    this->scoreLabel = CCLabelTTF::labelWithString("10", "Courier New", 70);
    this->scoreLabel->setColor(this->color);

    this->initWithFile("circle.png");
    this->setScale(3);
    this->setColor(this->color);

    return true;
}

void Player::initScoreLabel(CCLayer *parent){
    CCPoint pos = this->currentTarget->getPosition();
    this->scoreLabel->setPosition(CCPoint(pos.x, this->currentTarget->boundingBox().getMaxY()+30));
    parent->addChild(scoreLabel);
}

void Player::initTerritory(CCRect screenBox) {

    this->territory = CCRectMake(screenBox.origin.x, screenBox.origin.y, screenBox.size.width, screenBox.size.height);
    territory.size.width /= 2;

    CCPoint touchLocation = this->touch->getLocationInView();
    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);

    if (touchLocation.x < screenBox.getMidX()) {
        territory.origin.x = screenBox.origin.x / 2 ;
        this->_identifier = 0;
    } else {
        territory.origin.x = screenBox.origin.x / 2 + screenBox.size.width / 2;
        this->_identifier = 1;
    }
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
