#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

#import "Player.h"
#import "SquareTarget.h"
#import "GameManager.h"

using namespace cocos2d;


CCScene* HelloWorld::scene(){
    CCScene *scene = CCScene::create();
    HelloWorld *layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init(){
    
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    this->setTouchEnabled(true);
    
    GameManager::sharedManager()->gameIsActive = true;
    
    for(int i = 0; i < GameManager::sharedManager()->numPlayers; i++){
        Player *p = new Player::Player();
        p->init();
        p->spawnNewTargetWithLayer(this);
        GameManager::sharedManager()->players->push_back(p);
    }
    
    this->schedule(schedule_selector(HelloWorld::tick), .0001);
    
    return true;
}

void HelloWorld::tick(float dt){
    std::list<Player *> *players = GameManager::sharedManager()->players;
    for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
        Player *p1 = *iter;
        
        if(GameManager::sharedManager()->gameIsActive && p1->checkpointCount >= GameManager::sharedManager()->goalCheckpoints){
            GameManager::sharedManager()->gameIsActive = false;
        }
        
        for(std::list<Player *>::iterator iter2 = players->begin(); iter2 != players->end(); ++iter2){
            Player *p2 = *iter2;
            if(p1 == p2) continue;
            
            if(CCRect::CCRectIntersectsRect(p1->currentTarget->boundingBox(), p2->currentTarget->boundingBox())){
                //this->resolveTargetCollision();
            }
        }
    }
}

void HelloWorld::resolveTargetCollision(){
    std::list<Player *> *players = GameManager::sharedManager()->players;
    for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
        Player *p1 = *iter;
        printf("Resolving collision\n");
        p1->spawnNewTargetWithLayer(this);
    }
    
}

void HelloWorld::ccTouchesBegan(CCSet *touches, CCEvent *event) {
    for(auto it = touches->begin(); it != touches->end(); it++){
        CCTouch *touch = (CCTouch *)*it;
        CCPoint touchLocation = touch->getLocationInView();
        touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
        
        std::list<Player *> *players = GameManager::sharedManager()->players;
        for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
            Player *p1 = *iter;

            if(p1->touch == NULL && CCRect::CCRectContainsPoint(p1->currentTarget->boundingBox(), touchLocation)){
                p1->touch = (CCTouch *)*it;
            }
        }
    }
}

void HelloWorld::ccTouchesMoved(CCSet *touches, CCEvent *event) {
    for(auto it = touches->begin(); it != touches->end(); it++){
        CCTouch *touch = (CCTouch *)*it;
        CCPoint touchLocation = touch->getLocationInView();
        touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
        
        std::list<Player *> *players = GameManager::sharedManager()->players;
        for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
            Player *p1 = *iter;
            if((CCTouch *)*it == p1->touch && CCRect::CCRectContainsPoint(p1->currentTarget->boundingBox(), touchLocation)){
                if(!p1->touchLock){
                    p1->spawnNewTargetWithLayer(this);
                    p1->checkpointCount += 1;
                    // uncomment this when cccallfunc gets figured out
                    //p1->touchLock = true;
                }
            }
        }
    }
}

void HelloWorld::ccTouchesEnded(CCSet *touches, CCEvent *event){
    for(auto it = touches->begin(); it != touches->end(); it++){
        CCTouch *touch = (CCTouch *)*it;
        CCPoint touchLocation = touch->getLocationInView();
        touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
        
        std::list<Player *> *players = GameManager::sharedManager()->players;
        for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
            Player *p1 = *iter;
            if((CCTouch *)*it == p1->touch){
                p1->touch = NULL;
            }
        }
    }
}

bool HelloWorld::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}