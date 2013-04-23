#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

#import "Player.h"
#import "SquareTarget.h"
#import "GameManager.h"

#include <sys/timeb.h>

using namespace cocos2d;


CCScene* HelloWorld::scene(){
    CCScene *scene = CCScene::create();
    HelloWorld *layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

void HelloWorld::setupTitleScreenTextOverlay(){
    titleLayer = CCLayer::node();
    CCLabelTTF *label = CCLabelTTF::labelWithString("Trundle", "Courier New", 80);
    label->setRotation(-90);
    label->setPosition(CCPoint(this->boundingBox().getMidX(), this->boundingBox().getMidY()));
    label->setColor(ccc3(0, 0, 0));
    titleLayer->addChild(label, 11);
    
    this->addChild(titleLayer, 11);
}

void HelloWorld::setupTitleScreen(){
    titleSprites = new std::list<CCSprite *>();
    
    CCSize screenDimensions = CCEGLView::sharedOpenGLView()->getFrameSize();
    printf("Screen: %0.2f x %0.2f\n", screenDimensions.width, screenDimensions.height);
    if(screenDimensions.width <= 960 && screenDimensions.height <= 640){
        printf("Detected small screen\n");
        GameManager::sharedManager()->maxPlayers = 2;
        GameManager::sharedManager()->numPlayers = 2;

        CCSprite *p1 = new CCSprite();
        p1->initWithFile("square.png");
        p1->setPosition(CCPoint(this->boundingBox().getMidX()+this->getContentSize().width/4, this->boundingBox().getMidY()));
        p1->setScaleX(this->getContentSize().width/p1->getContentSize().width/2);
        p1->setScaleY(this->getContentSize().height/p1->getContentSize().height);
        p1->setColor(GameManager::sharedManager()->getNextColor());
        this->addChild(p1, 10);
        titleSprites->push_back(p1);

        CCSprite *p2 = new CCSprite();
        p2->initWithFile("square.png");
        p2->setPosition(CCPoint(this->boundingBox().getMidX()-this->getContentSize().width/4, this->boundingBox().getMidY()));
        p2->setScaleX(this->getContentSize().width/p2->getContentSize().width/2);
        p2->setScaleY(this->getContentSize().height/p2->getContentSize().height);
        p2->setColor(GameManager::sharedManager()->getNextColor());
        this->addChild(p2, 10);
        titleSprites->push_back(p2);
        
        setupTitleScreenTextOverlay();
    } else if(screenDimensions.width > 960 && screenDimensions.height > 640){
        printf("Detected large screen\n");
        GameManager::sharedManager()->maxPlayers = 4;
    }
}

void HelloWorld::setupTitleScreenFromEndgameScreen(){
    float initTime = .5;
    CCSprite *p1 = titleSprites->front();
    p1->runAction(CCSequence::actions(
                                      CCScaleTo::actionWithDuration(initTime, this->getContentSize().width/p1->getContentSize().width/2, this->getContentSize().height/p1->getContentSize().height),
                                      NULL));
    p1->runAction(CCSequence::actions(
                                      CCMoveTo::actionWithDuration(initTime, CCPoint(this->boundingBox().getMidX()+this->getContentSize().width/4, this->boundingBox().getMidY())),
                                      NULL));
    
    CCSprite *p2 = new CCSprite();
    p2->initWithFile("square.png");
    p2->setScaleX(this->getContentSize().width/p2->getContentSize().width/2);
    p2->setScaleY(this->getContentSize().height/p2->getContentSize().height);
    p2->setPosition(CCPoint(this->boundingBox().getMinX()-p2->getContentSize().width*p2->getScaleX(), this->boundingBox().getMidY()));
    p2->setColor(GameManager::sharedManager()->getNextColor());
    this->addChild(p2, 10);
    titleSprites->push_back(p2);
    
    p2->runAction(CCSequence::actions(
                                      CCMoveTo::actionWithDuration(.5, CCPoint(this->boundingBox().getMidX()-this->getContentSize().width/4, this->boundingBox().getMidY())),
                                      NULL));
    setupTitleScreenTextOverlay();
}

void HelloWorld::dismissTitleScreen(){
    float animationDuration = 1;
    if(titleLayer != NULL){
        titleLayer->removeFromParentAndCleanup(true);
    }
    for(std::list<CCSprite *>::iterator iter = titleSprites->begin(); iter != titleSprites->end(); ++iter){
        CCSprite *sp = *iter;
        sp->runAction(CCScaleTo::actionWithDuration(animationDuration, 0));
        
        CCPoint p = CCDirector::sharedDirector()->convertToGL(((CCTouch *)sp->getUserData())->getLocationInView());
        sp->runAction(CCMoveTo::actionWithDuration(animationDuration, p));
    }
}

void HelloWorld::setupEndgameScreen(Player *winner){
    printf("Game over screen\n");
    float initTime = .5, waitTime = 1;
    
    CCSprite *p1 = new CCSprite();
    p1->initWithFile("square.png");
    p1->setPosition(winner->getPosition());
    p1->setScale(0);
    p1->setColor(winner->color);
    this->addChild(p1, 10);
    titleSprites->push_back(p1);
    
    GameManager::sharedManager()->usedColors->push_back(winner->color);
    
    p1->runAction(CCScaleTo::actionWithDuration(initTime, this->getContentSize().width/p1->getContentSize().width, this->getContentSize().height/p1->getContentSize().height));
    p1->runAction(CCMoveTo::actionWithDuration(initTime, CCPoint(this->boundingBox().getMidX(), this->boundingBox().getMidY())));
}

bool HelloWorld::init(){
    if(!CCLayer::init()){ return false; }
    
    GameManager::sharedManager();
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    this->setTouchEnabled(true);
    
    setupTitleScreen();
    
    this->schedule(schedule_selector(HelloWorld::tick), .0001);
    
    return true;
}

void HelloWorld::tick(float dt){
    ttime = GameManager::sharedManager()->getElapsed();
    
    if(GameManager::sharedManager()->gameIsActive()){
        std::list<Player *> *players = GameManager::sharedManager()->players;
        for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
            Player *p1 = *iter;
            
            if(p1->checkpointCount >= GameManager::sharedManager()->goalCheckpoints){
                GameManager::sharedManager()->endGame();
                setupEndgameScreen(p1);
            }
            
            for(std::list<Player *>::iterator iter2 = players->begin(); iter2 != players->end(); ++iter2){
                Player *p2 = *iter2;
                if(p1 == p2) continue;
                
                if(CCRect::CCRectIntersectsRect(p1->currentTarget->boundingBox(), p2->currentTarget->boundingBox())){
                    //this->resolveTargetCollision();
                }
            }
        }
    } else if(GameManager::sharedManager()->titleScreenIsActive()){
        if(GameManager::sharedManager()->getCurrentTimeSeconds() - lastPlayerQueueTime > 2 &&
           numQueuedPlayers <= GameManager::sharedManager()->maxPlayers && numQueuedPlayers > 1){
            printf("Starting game\n");
            GameManager::sharedManager()->startGame();
            dismissTitleScreen();
            GameManager::sharedManager()->numPlayers = numQueuedPlayers;
            setupGameScreen();
            numQueuedPlayers = 0;
        }
    } else if(GameManager::sharedManager()->endgameScreenIsActive()){
        if(GameManager::sharedManager()->timeSinceLastStateChange() > 4){
            GameManager::sharedManager()->resetGameState();
            setupTitleScreenFromEndgameScreen();
            GameManager::sharedManager()->setTitleState();
            GameManager::sharedManager()->resetColors();
        }
    }
}

void HelloWorld::setupGameScreen(){
    for(int i = 0; i < GameManager::sharedManager()->numPlayers; i++){
        CCTouch *t = NULL;
        CCSprite *ts = NULL;
        if(titleSprites->size() > 0){
            ts = titleSprites->front();
            titleSprites->pop_front();
            t = (CCTouch *)ts->getUserData();
        }
        Player *p = new Player();
        p->init(t, ts->getColor());
        p->initTerritory(this->boundingBox());
        p->spawnNewTarget(nextTargetPosition(p), this);
        p->initScoreLabel(this);
        this->addChild(p);
        GameManager::sharedManager()->players->push_back(p);
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
        
        if(GameManager::sharedManager()->gameIsActive()){
            std::list<Player *> *players = GameManager::sharedManager()->players;
            for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
                Player *p1 = *iter;
                
                if(p1->touch == NULL && CCRect::CCRectContainsPoint(p1->currentTarget->boundingBox(), touchLocation)){
                    p1->touch = (CCTouch *)*it;
                    p1->updatePosition(touchLocation);
                    this->addChild(p1);
                }
            }
        } else if(GameManager::sharedManager()->titleScreenIsActive()){
            for(std::list<CCSprite *>::iterator iter = titleSprites->begin(); iter != titleSprites->end(); ++iter){
                CCSprite *sp = *iter;
                if(sp->getUserData() == NULL && CCRect::CCRectContainsPoint(sp->boundingBox(), touchLocation)){
                    if(numQueuedPlayers < GameManager::sharedManager()->maxPlayers){
                        printf("gained queued player\n");
                        sp->setUserData(touch);  // use userdata as a lightweight "touched" indicator
                        numQueuedPlayers++;
                        lastPlayerQueueTime = GameManager::sharedManager()->getCurrentTimeSeconds();
                    }
                }
            }
        }
    }
}

void HelloWorld::ccTouchesMoved(CCSet *touches, CCEvent *event) {
    for(auto it = touches->begin(); it != touches->end(); it++){
        CCTouch *touch = (CCTouch *)*it;
        CCPoint touchLocation = touch->getLocationInView();
        touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
        
        if(GameManager::sharedManager()->gameIsActive()){
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
        } else if(GameManager::sharedManager()->titleScreenIsActive()){
            for(std::list<CCSprite *>::iterator iter = titleSprites->begin(); iter != titleSprites->end(); ++iter){
                CCSprite *sp = *iter;
                if(CCRect::CCRectContainsPoint(sp->boundingBox(), touchLocation) && (CCTouch *)sp->getUserData() != touch){
                    sp->setUserData(NULL);
                    if(numQueuedPlayers > 0){
                        printf("lost queued player\n");
                        numQueuedPlayers--;
                    }
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

        if(GameManager::sharedManager()->gameIsActive()){
            std::list<Player *> *players = GameManager::sharedManager()->players;
            for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
                Player *p1 = *iter;
                if((CCTouch *)*it == p1->touch){
                    p1->touch = NULL;
                    this->removeChild(p1, false);
                }
            }
        } else if(GameManager::sharedManager()->titleScreenIsActive()){
            for(std::list<CCSprite *>::iterator iter = titleSprites->begin(); iter != titleSprites->end(); ++iter){
                CCSprite *sp = *iter;
                if((CCTouch *)sp->getUserData() == touch){
                    sp->setUserData(NULL);
                    if(numQueuedPlayers > 0){
                        printf("lost queued player\n");
                        numQueuedPlayers--;
                    }
                }
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
        CCTouch *touch = p->touch;
        CCPoint touchLocation = touch->getLocationInView();
        touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);

        x = touchLocation.x;
        y = touchLocation.y;
    } else if (p->checkpointCount < GameManager::sharedManager()->goalCheckpoints * 0.75) {
        int territoryAddition = this->boundingBox().size.width / GameManager::sharedManager()->goalCheckpoints * 0.75 / 2;
        p->territory.size.width += territoryAddition;

        if (p->getID() == 1) {
            p->territory.origin.x -= territoryAddition;
        }

        x = arc4random() % (int)p->territory.size.width + p->territory.getMinX();
        y = arc4random() % (int)p->territory.size.height;
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