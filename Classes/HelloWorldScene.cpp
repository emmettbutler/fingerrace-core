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
        Player *p = new Player();
        p->init(i);
        p->spawnNewTarget(nextTargetPosition(p), this);
        p->initScoreLabel(this);
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
        p1->spawnNewTarget(nextTargetPosition(p1), this);
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
                p1->updatePosition(touchLocation);
                this->addChild(p1);
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

            if((CCTouch *)*it == p1->touch){
                p1->updatePosition(touchLocation);

                if(CCRect::CCRectContainsPoint(p1->currentTarget->boundingBox(), touchLocation)){
                    if(!p1->touchLock){
                        p1->spawnNewTarget(nextTargetPosition(p1), this);
                        p1->touchLock = true;
                        p1->checkpointCount += 1;
                        p1->remainingCheckpoints = GameManager::sharedManager()->goalCheckpoints - p1->checkpointCount;
                        adjustTargetSize(p1);
                    }
                } else {
                    p1->touchLock = false;
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
                this->removeChild(p1, false);
            }
        }
    }
}

Player *HelloWorld::currentWinner(){
    Player *top = NULL;
    std::list<Player *> *players = GameManager::sharedManager()->players;
    for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
        Player *p1 = *iter;
        if(top == NULL || p1->checkpointCount > top->checkpointCount){
            top = p1;
        }
    }
    return top;
}

void HelloWorld::adjustTargetSize(Player *p){
    // called when this player's score just increased
    if(p->checkpointCount == currentWinner()->checkpointCount){
        // loop over all other players and shrink them
        std::list<Player *> *players = GameManager::sharedManager()->players;
        for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
            Player *p1 = *iter;
            if(p1 != p){
                p1->shrinkTarget();
            }
        }
    } else {
        // grow this player
        p->growTarget();
    }
}

CCPoint HelloWorld::nextTargetPosition(Player *p) {
    float x, y;

    if (scoreTotal() == 0) {
        x = this->boundingBox().size.width / 4 + p->getID() * this->boundingBox().size.width / 2;
        y = this->boundingBox().getMidY();
    } else {
        x = arc4random() % (int)this->boundingBox().size.width;
        y = arc4random() % (int)this->boundingBox().size.height;
    }

    return *new CCPoint(x, y);
}

int HelloWorld::scoreTotal() {
    int score = 0;

    std::list<Player *> *players = GameManager::sharedManager()->players;
    for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter) {
        Player *p1 = *iter;
        score += p1->checkpointCount;
    }

    return score;
}

bool HelloWorld::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}