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
#include "ScoreCounter.h"
#include "GameManager.h"

Player::Player() {
    this->currentTarget = NULL;
}

bool Player::init(CCPoint p, ccColor3B c, CCLayer *parent){
    this->parent = parent;
    this->color = c;
    this->touchLock = false;
    this->touchActive = true;
    this->checkpointCount = 0;
    this->velocity = 4;
    this->startingPoint = p;
    this->baseScale = 1.0;
    this->opacityDelta = .05;
    this->activeColorScaleFactor = 1.8;  // must be greater than 1
    this->remainingCheckpoints = GameManager::sharedManager()->goalCheckpoints - this->checkpointCount;
    
    this->scoreLabel = CCLabelTTF::labelWithString("10", ROBOTO_FONT, 70);
    this->scoreLabel->setColor(this->color);
    this->scoreLabel->setOpacity(0);
    
    this->shineSprite = CCSprite::spriteWithFile("circle_blur.png");
    this->shineSprite->setColor(ccc3(255, 255, 255));
    this->shineSprite->setScale(1);
    this->shineSprite->setOpacity(255*.05);
    this->shineSprite->retain();
    parent->addChild(this->shineSprite);
    
    this->grabNotify = CCLabelTTF::labelWithString("Grab", ROBOTO_FONT, 40);
    this->grabNotify->setTag(GameManager::kMotionBlurTag);

    this->grabNotify->setVisible(false);
    parent->addChild(this->grabNotify);

    this->initWithFile("circle_blur.png");
    this->setScale(1.1);
    this->setOpacity(255*.2);
    this->setTag(GameManager::kMotionBlurTag);
    this->setColor(this->color);

    return true;
}

void Player::initScoreLabel(){
    CCPoint pos = this->currentTarget->getPosition();
    this->scoreLabel->setPosition(CCPoint(pos.x, this->currentTarget->boundingBox().getMaxY()+30));
    parent->addChild(scoreLabel);
}

void Player::initTerritory(CCRect screenBox) {

    this->territory = CCRectMake(screenBox.origin.x, screenBox.origin.y, screenBox.size.width, screenBox.size.height);
    territory.size.width /= 2;

    if (startingPoint.x < screenBox.getMidX()) {
        territory.origin.x = screenBox.origin.x / 2;

        if (startingPoint.y > screenBox.getMidY()) {
            this->_identifier = GameManager::kPlayer1;
        } else {
            this->_identifier = GameManager::kPlayer3;
        }
    } else {
        territory.origin.x = screenBox.origin.x / 2 + screenBox.size.width / 2;

        if (startingPoint.y > screenBox.getMidY()) {
            this->_identifier = GameManager::kPlayer2;
        } else {
            this->_identifier = GameManager::kPlayer4;
        }
    }
}

void Player::initScoreCounter() {
    ScoreCounter *sc = new ScoreCounter();
    sc->init(GameManager::sharedManager()->goalCheckpoints, this->color, this);
    sc->setPosition(GameManager::sharedManager()->getNextScoreCounterPosition());

    if (this->_identifier == GameManager::kPlayer1) {
        sc->setScaleY(-1.0);
    } else if (this->_identifier == GameManager::kPlayer2) {
        sc->setScaleX(-1.0);
    } else if (this->_identifier == GameManager::kPlayer3) {
        // correct rotation
    } else if (this->_identifier == GameManager::kPlayer4) {
        sc->setScaleY(-1.0);
        sc->setScaleX(-1.0);
    }

    parent->addChild(sc);
    this->scoreCounter = sc;
}

void Player::spawnNewTarget(CCPoint position) {
    if(this->touchLock) return;

    if(this->currentTarget == NULL){
        this->currentTarget = new SquareTarget();
        this->currentTarget->initWithPlayer(this);
        this->currentTarget->setPosition(position);
        this->currentTarget->setTag(10);
        parent->addChild(this->currentTarget);
        return;
    }
    
    float dX = abs(this->currentTarget->getPosition().x - position.x);
    float dY = abs(this->currentTarget->getPosition().y - position.y);
    double distance = sqrt(dX*dX + dY*dY);
    double timedelta = distance / (this->velocity*1000);
    
    this->currentTarget->runAction(
        CCSequence::actions(
            CCMoveTo::actionWithDuration(timedelta, position),
            NULL
        )
    );
    
    this->scoreCounter->removePoint();
    
    float halfHeight = (this->currentTarget->getContentSize().height*this->currentTarget->getScaleY())/2;
    
    this->scoreLabel->runAction(
        CCSequence::actions(
            CCMoveTo::actionWithDuration(timedelta, CCPoint(position.x, position.y+halfHeight+10)),
            NULL
        )
    );
    
    this->grabNotify->setPosition(CCPoint(position.x, position.y+halfHeight+30));
}

void Player::updateScoreText(){
    char buff[3];
    sprintf(buff, "%d", this->remainingCheckpoints);
    this->scoreLabel->setString(buff);
}

void Player::shrinkTarget(){
    if(this->currentTarget->getScale() > this->baseScale*.5){
        this->currentTarget->runAction(
            CCSequence::actions(
                CCScaleBy::actionWithDuration(.5, .8),
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

void Player::gainPoint(){
    this->checkpointCount++;
    float op = this->getOpacity()+255*this->opacityDelta;
    if(op <= 255){
        this->setOpacity(op);
    }
    op = this->shineSprite->getOpacity()+255*.3*this->opacityDelta;
    if(op <= 255){
        this->shineSprite->setOpacity(op);
    }
}

void Player::losePoint(){
    this->checkpointCount--;
    float op = this->getOpacity()-255*this->opacityDelta;
    if(op >= .2){
        this->setOpacity(op);
    }
    op = this->shineSprite->getOpacity()-255*.3*this->opacityDelta;
    if(op >= .2){
        this->shineSprite->setOpacity(op);
    }
}

void Player::activateTouch(CCTouch *touch){
    this->touch = touch;
    ccColor3B newColor = this->currentTarget->getColor();
    newColor.r *= activeColorScaleFactor;
    newColor.g *= activeColorScaleFactor;
    newColor.b *= activeColorScaleFactor;
    this->currentTarget->setColor(newColor);
    
    this->grabNotify->setVisible(false);
}

void Player::deactivateTouch(){
    this->touch = NULL;
    ccColor3B newColor = this->currentTarget->getColor();
    newColor.r /= activeColorScaleFactor;
    newColor.g /= activeColorScaleFactor;
    newColor.b /= activeColorScaleFactor;
    this->currentTarget->setColor(newColor);
    
    this->scoreCounter->addPoint();
    
    this->lastCheckpointTime = GameManager::sharedManager()->getCurrentTimeSeconds();
    
    this->grabNotify->setVisible(true);
}

void Player::updatePosition(CCPoint glPosition) {
    if (this->touch != NULL) {
        this->setPosition(glPosition);
        this->shineSprite->setPosition(glPosition);
    }
}

float Player::timeSinceLastCheckpoint(){
    return GameManager::sharedManager()->getCurrentTimeSeconds() - this->lastCheckpointTime;
}

int Player::getID(){
    return this->_identifier;
}
