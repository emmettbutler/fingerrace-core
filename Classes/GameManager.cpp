//
//  GameManager.cpp
//  FingerRace
//
//  Created by Emmett Butler on 4/4/13.
//
//

#include "GameManager.h"

#include "FileManager.h"
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
    matchPoints = 3;
    queueingTime = 1;
    
    startTime = getCurrentTimeSeconds();
    
    allowedColors = new std::list<ccColor3B>();

    allowedColors->push_back(ccc3(196, 77, 88));
    allowedColors->push_back(ccc3(78, 205, 196));
    allowedColors->push_back(ccc3(199, 244, 100));
    allowedColors->push_back(ccc3(252, 182, 83));
    allowedColors->push_back(ccc3(85, 98, 112));
    allowedColors->push_back(ccc3(142, 172, 0));

    usedColors = new std::list<ccColor3B>();

    if (this->firstRun()) {
        this->tutorialActive = true;
    }
}

void GameManager::setupCounterPositions(CCLayer *b){
    baseLayer = b;
    
    counterPositions = new std::list<CCPoint>();
    counterPositions->push_back(CCPoint(b->boundingBox().getMaxX() - 366, b->boundingBox().getMinY() - 30));
    counterPositions->push_back(CCPoint(b->boundingBox().getMinX() + 20, b->boundingBox().getMaxY() - 13));
    usedCounterPositions = new std::list<CCPoint>();
}

void GameManager::resetGameState(){
    std::list<Player *> *players = this->players;
    for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
        Player *p1 = *iter;
        p1->currentTarget->removeFromParentAndCleanup(true);
        p1->scoreLabel->removeFromParentAndCleanup(true);
        p1->shineSprite->removeFromParentAndCleanup(true);
        p1->scoreCounter->removeFromParentAndCleanup(true);
        if(this->tutorialActive){
            if(p1->tutMessage != NULL){
                p1->tutMessage->removeFromParentAndCleanup(true);
            }
            if(p1->tutColorMessage != NULL){
                p1->tutColorMessage->removeFromParentAndCleanup(true);
            }
        }
        p1->removeFromParentAndCleanup(true);
    }
    this->players->clear();
    this->tutorialActive = false;
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

bool GameManager::firstRun() {
    return !FileManager::readFile("tut");
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

    if (this->firstRun()) {
        FileManager::saveFile("tut");
    }

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

void GameManager::initStats() {
    winCounts = new std::vector<int>(maxPlayers, 0);
}