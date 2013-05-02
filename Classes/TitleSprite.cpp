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
    CCSprite *as = CCSprite::spriteWithFile("square.png");
    as->setScaleX(this->button->getScaleX());
    as->setScaleY(.00001);
    as->setAnchorPoint(CCPoint(.5, .5));
    as->setPosition(this->button->getPosition());
    as->setColor(ccc3(255, 0, 0));
    as->setOpacity(100);
    parent->addChild(as);
    this->buttonQueueOverlay = as;
    
    as->runAction(CCScaleTo::actionWithDuration(GameManager::sharedManager()->queueingTime, this->button->getScaleX(), this->button->getScaleY()));
}

void TitleSprite::stopQueueingAnimation(){
    this->buttonQueueOverlay->removeFromParentAndCleanup(true);
}