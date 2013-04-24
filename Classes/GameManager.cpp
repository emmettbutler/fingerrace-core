//
//  GameManager.cpp
//  FingerRace
//
//  Created by Emmett Butler on 4/4/13.
//
//

#include "GameManager.h"

#include "Player.h"
#include "SquareTarget.h"
#include "ScoreCounter.h"

#include <sys/timeb.h>

static GameManager *sharedInstance = NULL;

long double GameManager::getCurrentTimeSeconds(){
    timeb tb;
    ftime( &tb );
    int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
    return nCount / 1000.0;
}

GameManager *GameManager::sharedManager(){
    if(sharedInstance == NULL){
        sharedInstance = new GameManager();
        sharedInstance->init();
    }
    return sharedInstance;
}

void GameManager::init(){
    players = new std::list<Player *>();
    currentState = kTitleState;
    goalCheckpoints = 20;
    
    startTime = getCurrentTimeSeconds();
    
    allowedColors = new std::list<ccColor3B>();
    allowedColors->push_back(ccc3(255, 0, 0));
    allowedColors->push_back(ccc3(0, 0, 255));
    allowedColors->push_back(ccc3(0, 255, 0));
    allowedColors->push_back(ccc3(255, 255, 0));
    allowedColors->push_back(ccc3(255, 0, 255));
    usedColors = new std::list<ccColor3B>();
}

void GameManager::setupCounterPositions(CCLayer *b){
    baseLayer = b;
    
    counterPositions = new std::list<CCPoint>();
    counterPositions->push_back(CCPoint(b->boundingBox().getMinX()+20, b->boundingBox().getMinY()-50));
    counterPositions->push_back(CCPoint(b->boundingBox().getMinX()+620, b->boundingBox().getMinY()-620));
    usedCounterPositions = new std::list<CCPoint>();
}

void GameManager::resetGameState(){
    std::list<Player *> *players = this->players;
    for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
        Player *p1 = *iter;
        p1->currentTarget->removeFromParentAndCleanup(true);
        p1->scoreLabel->removeFromParentAndCleanup(true);
        p1->scoreCounter->removeFromParentAndCleanup(true);
        p1->removeFromParentAndCleanup(true);
    }
    this->players->clear();
}

ccColor3B GameManager::getNextColor(){
    ccColor3B ret;
    for(std::list<ccColor3B>::iterator iter = allowedColors->begin(); iter != allowedColors->end(); ++iter){
        ret = *iter;
        bool found = false;
        for(std::list<ccColor3B>::iterator iter2 = usedColors->begin(); iter2 != usedColors->end(); ++iter2){
            ccColor3B test = *iter2;
            if(ret.r == test.r && ret.b == test.b && ret.g == test.g){
                found = true;
            }
        }
        if(!found){
            usedColors->push_back(ret);
            return ret;
        }
    }
    return ccc3(255, 255, 255);
}

void GameManager::resetColors(){
    usedColors->clear();
}

CCPoint GameManager::getNextScoreCounterPosition(){
    CCPoint ret;
    for(std::list<CCPoint>::iterator iter = counterPositions->begin(); iter != counterPositions->end(); ++iter){
        ret = *iter;
        bool found = false;
        for(std::list<CCPoint>::iterator iter2 = usedCounterPositions->begin(); iter2 != usedCounterPositions->end(); ++iter2){
            CCPoint test = *iter2;
            if(ret.x == test.x && ret.y == test.y){
                found = true;
            }
        }
        if(!found){
            usedCounterPositions->push_back(ret);
            return ret;
        }
    }
    return CCPoint(0, 0);
}

void GameManager::resetCounterPositions(){
    usedCounterPositions->clear();
}

long double GameManager::getElapsed(){
    return getCurrentTimeSeconds() - startTime;
}

bool GameManager::pregameIsActive(){
    return currentState == kPreGameState;
}

bool GameManager::gameIsActive(){
    return currentState == kInGameState;
}

bool GameManager::titleScreenIsActive(){
    return currentState == kTitleState;
}

void GameManager::setupGame(){
    currentState = kPreGameState;
    lastStateChangeTime = getCurrentTimeSeconds();
}

bool GameManager::endgameScreenIsActive(){
    return currentState == kGameOverState;
}

void GameManager::startGame(){
    currentState = kInGameState;
    lastStateChangeTime = getCurrentTimeSeconds();
}

void GameManager::endGame(){
    currentState = kGameOverState;
    lastStateChangeTime = getCurrentTimeSeconds();
}

void GameManager::setTitleState(){
    currentState = kTitleState;
    lastStateChangeTime = getCurrentTimeSeconds();
}

long double GameManager::timeSinceLastStateChange(){
    return getCurrentTimeSeconds() - lastStateChangeTime;
}