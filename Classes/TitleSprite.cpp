//
//  TitleSprite.cpp
//  FingerRace
//
//  Created by Emmett Butler on 5/2/13.
//
//

#include "TitleSprite.h"
#include "GameManager.h"

/*!
 * TitleSprite constructor.
 */
TitleSprite::TitleSprite(){
    CCSprite();
    this->queued = false;
    this->touch = NULL;
}

/*!
 * Runs shrinking animation on title sprite label.
 */
void TitleSprite::runQueueingAnimation(CCLayer *parent){
    this->baseScale = this->button->getScaleY();
    this->button->runAction(CCScaleTo::actionWithDuration(GameManager::sharedManager()->queueingTime, this->button->getScaleX(), 0));
}

/*!
 * Stops and reverts shrinking animation on title sprite label.
 */
void TitleSprite::stopQueueingAnimation(){
    this->button->stopAllActions();
    this->button->setScaleY(this->baseScale);
}

/*!
 * Queues title sprite to indicate player is active.
 */
void TitleSprite::queue(){
    this->queued = true;
}

/*!
 * Dequeues title sprite to indicate player is no longer active.
 */
void TitleSprite::dequeue(){
    this->queued = false;
}

/*!
 * @return True if player is active, false if inactive.
 */
bool TitleSprite::isQueued(){
    return this->queued;
}