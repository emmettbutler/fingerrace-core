//
//  GameManager.cpp
//  FingerRace
//
//  Created by Emmett Butler on 4/4/13.
//
//

#include "GameManager.h"

#include "Player.h"

static GameManager *sharedInstance = NULL;

GameManager *GameManager::sharedManager(){
    if(sharedInstance == NULL){
        sharedInstance = new GameManager();
        sharedInstance->init();
    }
    return sharedInstance;
}

void GameManager::init(){
    players = new std::list<Player *>();
    gameIsActive = true;
    numPlayers = 2;
    goalCheckpoints = 10;
}