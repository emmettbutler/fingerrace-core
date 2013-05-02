//
//  TitleSprite.cpp
//  FingerRace
//
//  Created by Emmett Butler on 5/2/13.
//
//

#include "TitleSprite.h"
#include "GameManager.h"

TitleSprite::TitleSprite(){
    CCSprite::CCSprite();
}

void TitleSprite::runQueueingAnimation(CCLayer *parent){
    this->baseScale = this->button->getScaleY();
    this->button->runAction(CCScaleTo::actionWithDuration(GameManager::sharedManager()->queueingTime, this->button->getScaleX(), 0));
}

void TitleSprite::stopQueueingAnimation(){
    this->button->stopAllActions();
    this->button->setScaleY(this->baseScale);
}