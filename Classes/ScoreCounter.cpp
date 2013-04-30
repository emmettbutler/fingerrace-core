//
//  ScoreCounter.cpp
//  FingerRace
//
//  Created by Emmett Butler on 4/23/13.
//
//

#include "ScoreCounter.h"
#include "Player.h"

bool ScoreCounter::init(int goalCheckpoints, ccColor3B c, Player *p){
    if(!CCLayer::init()){ return false; }
    
    this->player = p;
    this->pointSprites = CCArray::array();
    this->pointSprites->retain();
    
    int rowLength = 15;
    
    for(int i = 0; i <= (int)((double)goalCheckpoints/(double)rowLength); i++){
        for(int j = 0; j < rowLength; j++){
            CCSprite *sp = CCSprite::spriteWithFile("square.png");
            sp->setColor(c);
            sp->setScale(.5);
            sp->setPosition(CCPoint(this->boundingBox().getMinX()+((sp->getContentSize().width*sp->getScaleX()+3)*j),
                                    this->boundingBox().getMaxY()+((sp->getContentSize().height*sp->getScaleY()+3)*i)));
            if(numPoints > goalCheckpoints){
                sp->setOpacity(0);
            } else {
                numPoints++;
            }
            this->addChild(sp);
            pointSprites->addObject(sp);
        }
    }
    this->currentPointSpriteIndex = numPoints - 1;
    
    this->setContentSize(CCSize(345, 46));
    
    return true;
}

void ScoreCounter::removePoint(){
    if(numPoints < player->remainingCheckpoints) return;
    
    CCSprite *sp = (CCSprite *)this->pointSprites->objectAtIndex(currentPointSpriteIndex);
    if(sp != NULL){
        sp->setOpacity(0);
    }
    numPoints--;
    currentPointSpriteIndex--;
}

void ScoreCounter::addPoint(){
    if(numPoints > player->remainingCheckpoints || currentPointSpriteIndex+1 > pointSprites->count()-1) return;
    
    CCSprite *sp = (CCSprite *)this->pointSprites->objectAtIndex(currentPointSpriteIndex+1);
    if(sp != NULL){
        sp->setOpacity(255);
    }
    numPoints++;
    currentPointSpriteIndex++;
}