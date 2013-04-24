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
    this->pointSprites = new std::list<CCSprite *>();
    
    int rowLength = 15;
    
    for(int i = 0; i <= (int)((double)goalCheckpoints/(double)rowLength); i++){
        for(int j = 0; j < rowLength; j++){
            if(numPoints <= goalCheckpoints){
                numPoints++;
                
                CCSprite *sp = CCSprite::spriteWithFile("square.png");
                sp->setColor(c);
                sp->setScale(.5);
                sp->setPosition(CCPoint(this->boundingBox().getMinX()+((sp->getContentSize().width*sp->getScaleX()+3)*j),
                                        this->boundingBox().getMaxY()+((sp->getContentSize().height*sp->getScaleY()+3)*i)));
                this->addChild(sp);
                pointSprites->push_back(sp);
            }
        }
    }
    
    this->setContentSize(CCSize(300, 80));
    
    return true;
}

void ScoreCounter::removePoint(){
    if(numPoints < player->remainingCheckpoints) return;
    
    CCSprite *sp = this->pointSprites->back();
    if(sp != NULL){
        sp->setOpacity(0);
    }
    numPoints--;
    this->pointSprites->pop_back();
}

void ScoreCounter::addPoint(){
    
}