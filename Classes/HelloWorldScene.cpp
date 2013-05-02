#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

#import "Player.h"
#import "SquareTarget.h"
#import "GameManager.h"
#import "ScoreCounter.h"
#import "TitleSprite.h"

#include <sys/timeb.h>

using namespace cocos2d;


CCScene* HelloWorld::scene(){
    CCScene *scene = CCScene::create();
    HelloWorld *layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

void HelloWorld::setupTitleScreenTextOverlay(ccColor3B p1Color, ccColor3B p2Color){
    titleLayer = CCLayer::node();
    CCLabelTTF *label = CCLabelTTF::labelWithString("BUMP", ROBOTO_FONT, 100);
    label->setRotation(-90);
    label->setPosition(CCPoint(this->boundingBox().getMidX() - 63, this->boundingBox().getMinY() + 135));
    label->setColor(p1Color);
    titleLayer->addChild(label);

    CCLabelTTF *subLabel = CCLabelTTF::labelWithString("MAP", ROBOTO_FONT, 100);
    subLabel->setRotation(-90);
    subLabel->setPosition(CCPoint(this->boundingBox().getMidX() + 55, this->boundingBox().getMinY() + 115));
    subLabel->setColor(p2Color);
    titleLayer->addChild(subLabel);
    
    insBox1 = CCSprite::spriteWithFile("square.png");
    insBox1->setScaleX(2);
    insBox1->setScaleY(9);
    insBox1->setPosition(CCPoint(this->boundingBox().getMaxX() - 60, this->boundingBox().getMidY()));
    insBox1->setColor(p2Color);
    titleLayer->addChild(insBox1);
    
    CCLabelTTF *instructionLabel1 = CCLabelTTF::labelWithString("p1 touch & hold", ROBOTO_FONT, 50);
    instructionLabel1->setPosition(insBox1->getPosition());
    instructionLabel1->setColor(p1Color);
    instructionLabel1->setRotation(-90);
    titleLayer->addChild(instructionLabel1);
    
    insBox2 = CCSprite::spriteWithFile("square.png");
    insBox2->setScaleX(2);
    insBox2->setScaleY(9);
    insBox2->setPosition(CCPoint(this->boundingBox().getMinX() + 60, this->boundingBox().getMidY()));
    insBox2->setColor(p1Color);
    titleLayer->addChild(insBox2);
    
    CCLabelTTF *instructionLabel2 = CCLabelTTF::labelWithString("p2 touch & hold", ROBOTO_FONT, 50);
    instructionLabel2->setPosition(insBox2->getPosition());
    instructionLabel2->setColor(p2Color);
    instructionLabel2->setRotation(90);
    titleLayer->addChild(instructionLabel2);

    if (!GameManager::sharedManager()->firstRun()) {
        tutButton = CCSprite::spriteWithFile("square.png");
        tutButton->setScale(2);
        tutButton->setColor(p2Color);
        tutButton->setPosition(CCPoint(this->boundingBox().getMidX() + 55, this->boundingBox().getMaxY() - 100));
        titleLayer->addChild(tutButton);

        tutQLabel = CCLabelTTF::labelWithString("?", ROBOTO_FONT, 80);
        tutQLabel->setRotation(90);
        tutQLabel->setColor(p1Color);
        tutQLabel->setPosition(CCPoint(this->boundingBox().getMidX() + 55, this->boundingBox().getMaxY() - 100));
        titleLayer->addChild(tutQLabel);

        tutELabel = CCLabelTTF::labelWithString("!", ROBOTO_FONT, 80);
        tutELabel->setRotation(90);
        tutELabel->setColor(p1Color);
        tutELabel->setOpacity(0);
        tutELabel->setPosition(CCPoint(this->boundingBox().getMidX() + 55, this->boundingBox().getMaxY() - 100));
        titleLayer->addChild(tutELabel);

        tutNotify = CCLabelTTF::labelWithString("Tutorial ON", ROBOTO_FONT, 40);
        tutNotify->setPosition(CCPoint(tutButton->getPosition().x, tutButton->getPosition().y - 150));
        tutNotify->setRotation(90);
        tutNotify->setColor(p2Color);
        tutNotify->setOpacity(0);
        titleLayer->addChild(tutNotify);
    }

    this->addChild(titleLayer, 11);
}

void HelloWorld::animateTutButtonActivation(){
    tutQLabel->runAction(CCSequence::actions(CCRotateTo::actionWithDuration(.3, -90), NULL));
    tutQLabel->runAction(CCSequence::actions(CCFadeTo::actionWithDuration(.3, 0), NULL));
    
    tutELabel->runAction(CCSequence::actions(CCRotateTo::actionWithDuration(.3, -90), NULL));
    tutELabel->runAction(CCSequence::actions(CCFadeTo::actionWithDuration(.3, 255), NULL));
    
    tutNotify->setString("Tutorial ON");
    tutNotify->runAction(
                 CCSequence::actions(
                                     CCFadeTo::actionWithDuration(.2, 255),
                                     CCFadeTo::actionWithDuration(1, 0),
                 NULL)
    );
}

void HelloWorld::animateTutButtonDeactivation(){
    tutQLabel->runAction(CCSequence::actions(CCRotateTo::actionWithDuration(.3, 90), NULL));
    tutQLabel->runAction(CCSequence::actions(CCFadeTo::actionWithDuration(.3, 255), NULL));
    
    tutELabel->runAction(CCSequence::actions(CCRotateTo::actionWithDuration(.3, 90), NULL));
    tutELabel->runAction(CCSequence::actions(CCFadeTo::actionWithDuration(.3, 0), NULL));
    
    tutNotify->setString("Tutorial OFF");
    tutNotify->runAction(
                         CCSequence::actions(
                                             CCFadeTo::actionWithDuration(.2, 255),
                                             CCFadeTo::actionWithDuration(1, 0),
                                             NULL)
                         );
}

void HelloWorld::setupEndgameScreenTextOverlay(){

    ccColor3B statColor = GameManager::sharedManager()->getNextColor();
    endgameLayer = CCLayer::node();

    char p1Score [1];
    sprintf(p1Score, "%d", GameManager::sharedManager()->winCounts->at(0));
    CCLabelTTF *p1ScoreLabel = CCLabelTTF::labelWithString(p1Score, ROBOTO_FONT, 200);
    p1ScoreLabel->setPosition(CCPoint(this->boundingBox().getMidX() - 300, this->boundingBox().getMidY()));
    p1ScoreLabel->setRotation(90);
    p1ScoreLabel->setColor(statColor);
    p1ScoreLabel->setOpacity(0);
    endgameLayer->addChild(p1ScoreLabel);

    char p2Score [1];
    sprintf(p2Score, "%d", GameManager::sharedManager()->winCounts->at(1));
    CCLabelTTF *p2ScoreLabel = CCLabelTTF::labelWithString(p2Score, ROBOTO_FONT, 200);
    p2ScoreLabel->setPosition(CCPoint(this->boundingBox().getMidX() + 300, this->boundingBox().getMidY()));
    p2ScoreLabel->setRotation(-90);
    p2ScoreLabel->setColor(statColor);
    p2ScoreLabel->setOpacity(0);
    endgameLayer->addChild(p2ScoreLabel);

    p1ScoreLabel->runAction(CCFadeIn::actionWithDuration(1.0));
    p2ScoreLabel->runAction(CCFadeIn::actionWithDuration(1.0));

    this->addChild(endgameLayer, 11);
}

void HelloWorld::setupTitleScreen(){
    titleSprites = new std::list<TitleSprite *>();
    titleTouchPoints = new std::list<CCPoint>();
    
    CCSize screenDimensions = CCEGLView::sharedOpenGLView()->getFrameSize();
    printf("Screen: %0.2f x %0.2f\n", screenDimensions.width, screenDimensions.height);
    if(screenDimensions.width <= 960 && screenDimensions.height <= 640){
        printf("Detected small screen\n");
        GameManager::sharedManager()->maxPlayers = 2;
        GameManager::sharedManager()->numPlayers = 2;

        TitleSprite *p1 = new TitleSprite();
        p1->initWithFile("square.png");
        p1->setPosition(CCPoint(this->boundingBox().getMidX()+this->getContentSize().width/4, this->boundingBox().getMidY()));
        p1->setScaleX(this->getContentSize().width/p1->getContentSize().width/2);
        p1->setScaleY(this->getContentSize().height/p1->getContentSize().height);
        p1->setColor(GameManager::sharedManager()->getNextColor());
        this->addChild(p1, 10);
        titleSprites->push_back(p1);

        TitleSprite *p2 = new TitleSprite();
        p2->initWithFile("square.png");
        p2->setPosition(CCPoint(this->boundingBox().getMidX()-this->getContentSize().width/4, this->boundingBox().getMidY()));
        p2->setScaleX(this->getContentSize().width/p2->getContentSize().width/2);
        p2->setScaleY(this->getContentSize().height/p2->getContentSize().height);
        p2->setColor(GameManager::sharedManager()->getNextColor());
        this->addChild(p2, 10);
        titleSprites->push_back(p2);
        
        setupTitleScreenTextOverlay(p1->getColor(), p2->getColor());
        
        p1->button = insBox1;
        p2->button = insBox2;
    } else if(screenDimensions.width > 960 && screenDimensions.height > 640){
        printf("Detected large screen\n");
        GameManager::sharedManager()->maxPlayers = 2;
        GameManager::sharedManager()->numPlayers = 2;

        TitleSprite *p1 = new TitleSprite();
        p1->initWithFile("square.png");
        p1->setPosition(CCPoint(this->boundingBox().getMidX()+this->getContentSize().width/4, this->boundingBox().getMidY()));
        p1->setScaleX(this->getContentSize().width/p1->getContentSize().width/2);
        p1->setScaleY(this->getContentSize().height/p1->getContentSize().height);
        p1->setColor(GameManager::sharedManager()->getNextColor());
        this->addChild(p1, 10);
        titleSprites->push_back(p1);

        TitleSprite *p2 = new TitleSprite();
        p2->initWithFile("square.png");
        p2->setPosition(CCPoint(this->boundingBox().getMidX()-this->getContentSize().width/4, this->boundingBox().getMidY()));
        p2->setScaleX(this->getContentSize().width/p2->getContentSize().width/2);
        p2->setScaleY(this->getContentSize().height/p2->getContentSize().height);
        p2->setColor(GameManager::sharedManager()->getNextColor());
        this->addChild(p2, 10);
        titleSprites->push_back(p2);

        setupTitleScreenTextOverlay(p1->getColor(), p2->getColor());
        
        p1->button = insBox1;
        p2->button = insBox2;
    }

    GameManager::sharedManager()->initStats();
}

void HelloWorld::setupTitleScreenFromEndgameScreen(){
    float initTime = .5;
    TitleSprite *p1 = titleSprites->front();
    p1->runAction(CCSequence::actions(
                                      CCScaleTo::actionWithDuration(initTime, this->getContentSize().width/p1->getContentSize().width/2, this->getContentSize().height/p1->getContentSize().height),
                                      NULL));
    p1->runAction(CCSequence::actions(
                                      CCMoveTo::actionWithDuration(initTime, CCPoint(this->boundingBox().getMidX()+this->getContentSize().width/4, this->boundingBox().getMidY())),
                                      NULL));
    
    TitleSprite *p2 = new TitleSprite();
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
    setupTitleScreenTextOverlay(p1->getColor(), p2->getColor());
    
    p1->button = insBox1;
    p1->touch = NULL;
    
    p2->button = insBox2;
    p2->touch = NULL;
}

void HelloWorld::dismissTitleScreen(){
    float animationDuration = .4;
    if(titleLayer != NULL){
        titleLayer->removeFromParentAndCleanup(true);
    }
    for(std::list<TitleSprite *>::iterator iter = titleSprites->begin(); iter != titleSprites->end(); ++iter){
        TitleSprite *sp = *iter;
        sp->runAction(CCScaleTo::actionWithDuration(animationDuration, 0));
        
        CCPoint p = CCDirector::sharedDirector()->convertToGL(sp->touch->getLocationInView());
        sp->runAction(CCMoveTo::actionWithDuration(animationDuration, p));

        titleTouchPoints->push_back(p);
    }
}

void HelloWorld::dismissEndgameScreen(){
    if(endgameLayer != NULL){
        endgameLayer->removeFromParentAndCleanup(true);
    }
}

void HelloWorld::setupEndgameScreen(Player *winner){
    printf("Game over screen\n");
    float initTime = 0.5;
    
    TitleSprite *p1 = new TitleSprite();
    p1->initWithFile("square.png");
    p1->setPosition(CCPoint(this->boundingBox().getMidX(), this->boundingBox().getMidY()));
    p1->setOpacity(0);
    p1->setScaleX(this->getContentSize().width/p1->getContentSize().width);
    p1->setScaleY(this->getContentSize().height/p1->getContentSize().height);
    p1->setColor(winner->color);
    this->addChild(p1, 10);
    titleSprites->push_back(p1);
    
    GameManager::sharedManager()->usedColors->push_back(winner->color);

    // increment win count stat for the winner
    if(GameManager::sharedManager()->winCounts->at(winner->getID()) == GameManager::sharedManager()->matchPoints){
        GameManager::sharedManager()->initStats();
    }
    GameManager::sharedManager()->winCounts->at(winner->getID())++;
    
    p1->runAction(CCFadeIn::actionWithDuration(initTime));
    setupEndgameScreenTextOverlay();
}

void HelloWorld::RemoveChildSeq(CCNode* pObj){
    this->removeChild(pObj, true);
}

void HelloWorld::iterateBackground(bool lots){
    int limit = 1 + currentWinner()->checkpointCount;
    if(limit == 0){
        limit = 1;
    }
    if(lots){
        limit = 20;
    }
    for(int i = 0; i < limit; i++){
        CCSprite *p = CCSprite::spriteWithFile("square.png");
        p->setScale(.04*(arc4random() % 10));
        p->setColor(currentWinner()->getColor());
        p->setOpacity(arc4random() % 255);
        CCFiniteTimeAction *moveAct;
        float speedFactor = (.1*(GameManager::sharedManager()->goalCheckpoints - currentWinner()->checkpointCount+1));
        CCAction* removeChild = CCCallFuncO::create(this, callfuncO_selector(HelloWorld::RemoveChildSeq), p);
        if(i % 2 == 0){
            p->setPosition(CCPoint(this->boundingBox().getMaxX(), arc4random() % (int)this->boundingBox().getMaxY()));
            moveAct = CCMoveTo::actionWithDuration(.02*speedFactor*(arc4random() % 100), CCPoint(this->boundingBox().getMinX(), p->getPosition().y));
        } else {
            p->setPosition(CCPoint(this->boundingBox().getMinX(), arc4random() % (int)this->boundingBox().getMaxY()));
            moveAct = CCMoveTo::actionWithDuration(.02*speedFactor*(arc4random() % 100), CCPoint(this->boundingBox().getMaxX(), p->getPosition().y));
        }
        this->addChild(p, 0);
        
        p->runAction(CCSequence::actions(moveAct, removeChild, NULL));
    }
}

bool HelloWorld::init(){
    if(!CCLayer::init()){ return false; }
    
    GameManager::sharedManager();
    
    this->setContentSize(CCEGLView::sharedOpenGLView()->getFrameSize());
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    this->setTouchEnabled(true);
    this->kRenderTextureCount = 6;
    this->currentRenderTextureIndex = 0;
    
    CCSize winSize = this->getContentSize();
    renderTextures = CCArray::arrayWithCapacity(kRenderTextureCount);
    renderTextures->retain();
    
    for (int i = 0; i < kRenderTextureCount; i++){
        CCRenderTexture *rtx = CCRenderTexture::renderTextureWithWidthAndHeight((int)winSize.width, (int)winSize.height);

        rtx->setPosition(CCPointMake(winSize.width / 2, winSize.height / 2));
        
        CCSprite* renderSprite = CCSprite::spriteWithTexture(rtx->getSprite()->getTexture());
        renderSprite->setPosition(rtx->getPosition());
        
        this->addChild(renderSprite, 100 + i);
        rtx->setUserData(renderSprite);
        renderTextures->addObject(rtx);
    }
    
    setupTitleScreen();
    
    printf("Finished init\n");
    
    this->schedule(schedule_selector(HelloWorld::tick), .0001);
    
    return true;
}

void HelloWorld::visit(){
    CCRenderTexture* rtx = (CCRenderTexture *)renderTextures->objectAtIndex(currentRenderTextureIndex);
    rtx->beginWithClear(0, 0, 0, 0);
    
    CCObject* node;
    CCARRAY_FOREACH(this->getChildren(), node){
        if (((CCNode *)node)->getTag() == 10){
            ((CCNode *)node)->visit();
        }
    }
    
    rtx->end();
    
    // reorder the render textures so that the
    // most recently rendered texture is drawn last
    this->selectNextRenderTexture();
    int index = currentRenderTextureIndex;
    for (int i = 0; i < kRenderTextureCount; i++){
        CCRenderTexture* rtx = (CCRenderTexture*)renderTextures->objectAtIndex(currentRenderTextureIndex);
        CCSprite* renderSprite = (CCSprite*)rtx->getUserData();
        renderSprite->setOpacity((255.0f / kRenderTextureCount) * (i + 1));
        renderSprite->setScaleY(-1);
        this->reorderChild(renderSprite, 100+i);
        this->selectNextRenderTexture();
        
        index++;
        if (index >= kRenderTextureCount) {
            index = 0;
        }
    }
    
    // draw any remaining nodes
    CCARRAY_FOREACH(this->getChildren(), node){
        if (((CCNode *)node)->getTag() != 10){
            ((CCNode *)node)->visit();
        }
    }
}

void HelloWorld::selectNextRenderTexture(){
	currentRenderTextureIndex++;
	if (currentRenderTextureIndex >= kRenderTextureCount){
		currentRenderTextureIndex = 0;
	}
}

void HelloWorld::tick(float dt){
    ttime = GameManager::sharedManager()->getElapsed();
    
    if(GameManager::sharedManager()->gameIsActive()){
        iterateBackground(false);
        std::list<Player *> *players = GameManager::sharedManager()->players;
        for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
            Player *p1 = *iter;
            p1->remainingCheckpoints = GameManager::sharedManager()->goalCheckpoints - p1->checkpointCount;
            p1->updateScoreText();
            if(p1->checkpointCount >= GameManager::sharedManager()->goalCheckpoints){
                GameManager::sharedManager()->endGame();
                setupEndgameScreen(p1);
            }
            if(GameManager::sharedManager()->tutorialActive && currentWinner()->checkpointCount > GameManager::sharedManager()->goalCheckpoints - 5){
                p1->tutMessage->setString("Faster!!");
                p1->tutColorMessage->setVisible(false);
            }
        }
    } else if(GameManager::sharedManager()->titleScreenIsActive()){
        if(GameManager::sharedManager()->getCurrentTimeSeconds() - lastPlayerQueueTime > GameManager::sharedManager()->queueingTime &&
           numQueuedPlayers <= GameManager::sharedManager()->maxPlayers && numQueuedPlayers > 1){
            printf("Starting pregame\n");
            GameManager::sharedManager()->setupCounterPositions(this);
            dismissTitleScreen();
            GameManager::sharedManager()->numPlayers = numQueuedPlayers;
            setupGameScreen();
            GameManager::sharedManager()->setupGame();
            numQueuedPlayers = 0;
        }
    } else if(GameManager::sharedManager()->pregameIsActive()){
        if (GameManager::sharedManager()->timeSinceLastStateChange() > .5){
            printf("Starting game\n");
            std::list<Player *> *players = GameManager::sharedManager()->players;
            for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
                Player *p = *iter;
                p->spawnNewTarget(nextTargetPosition(p));
                if(GameManager::sharedManager()->tutorialActive){
                    p->tutMessage->runAction(CCSequence::actions(
                                                              CCDelayTime::actionWithDuration(2),
                                                              CCFadeTo::actionWithDuration(.3, 255),
                                                              NULL));
                    
                    p->tutColorMessage->runAction(CCSequence::actions(
                                                                 CCDelayTime::actionWithDuration(2),
                                                                 CCFadeTo::actionWithDuration(.3, 255),
                                                                 NULL));
                }
            }
            GameManager::sharedManager()->startGame();
        }
    } else if(GameManager::sharedManager()->endgameScreenIsActive()){
        iterateBackground(true);
        if(GameManager::sharedManager()->timeSinceLastStateChange() > 4){
            GameManager::sharedManager()->resetGameState();
            dismissEndgameScreen();
            setupTitleScreenFromEndgameScreen();
            GameManager::sharedManager()->setTitleState();
            if(GameManager::sharedManager()->usedColors->size() >= GameManager::sharedManager()->allowedColors->size()){
                GameManager::sharedManager()->resetColors();
            }
            GameManager::sharedManager()->resetCounterPositions();
        }
        if(GameManager::sharedManager()->tutorialActive){
            GameManager::sharedManager()->tutorialActive = false;
            std::list<Player *> *players = GameManager::sharedManager()->players;
            for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
                Player *p1 = *iter;
                p1->tutMessage->setVisible(false);
                p1->tutColorMessage->setVisible(false);
            }
        }
    }
}

void HelloWorld::setupGameScreen(){
    for(int i = 0; i < GameManager::sharedManager()->numPlayers; i++){
        CCTouch *t = NULL;
        CCPoint tp;
        TitleSprite *ts = NULL;
        if(titleSprites->size() > 0){
            ts = titleSprites->front();
            titleSprites->pop_front();
            t = ts->touch;
            tp = titleTouchPoints->front();
            titleTouchPoints->pop_front();
        }
        Player *p = new Player();
        p->init(tp, ts->getColor(), this);
        p->initTerritory(this->boundingBox());
        p->initScoreCounter();
        p->touch = t;
        p->spawnNewTarget(p->startingPoint);
        p->initScoreLabel();
        
        if(GameManager::sharedManager()->tutorialActive){
            int sep = 75;
            p->tutMessage = CCLabelTTF::labelWithString("Slide to", ROBOTO_FONT, 40);
            p->tutColorMessage = CCLabelTTF::labelWithString("your color", ROBOTO_FONT, 40);
            p->tutColorMessage->setColor(p->color);
            if(tp.x > this->boundingBox().getMidX()){
                p->tutMessage->setPosition(CCPoint(this->boundingBox().getMaxX() - 40, this->boundingBox().getMidY() - sep));
                p->tutMessage->setRotation(-90);
                p->tutColorMessage->setPosition(CCPoint(this->boundingBox().getMaxX() - 40, this->boundingBox().getMidY() + sep));
                p->tutColorMessage->setRotation(-90);
            } else {
                p->tutMessage->setPosition(CCPoint(this->boundingBox().getMinX() + 40, this->boundingBox().getMidY() + sep));
                p->tutMessage->setRotation(90);
                p->tutColorMessage->setPosition(CCPoint(this->boundingBox().getMinX() + 40, this->boundingBox().getMidY() - sep));
                p->tutColorMessage->setRotation(90);
            }
            p->tutMessage->setOpacity(0);
            p->tutColorMessage->setOpacity(0);
            this->addChild(p->tutMessage);
            this->addChild(p->tutColorMessage);
        }
        
        this->addChild(p);
        GameManager::sharedManager()->players->push_back(p);
    }
}

void HelloWorld::resolveTargetCollision(){
    std::list<Player *> *players = GameManager::sharedManager()->players;
    for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
        Player *p1 = *iter;
        printf("Resolving collision\n");
        p1->spawnNewTarget(nextTargetPosition(p1));
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
                
                if(CCRect::CCRectContainsPoint(p1->currentTarget->boundingBox(), touchLocation)){
                    if(p1->touch == NULL){
                        p1->activateTouch((CCTouch *)*it);
                        p1->updatePosition(touchLocation);
                        this->addChild(p1);
                        this->addChild(p1->shineSprite);
                        p1->spawnNewTarget(nextTargetPosition(p1));
                        
                        if(GameManager::sharedManager()->tutorialActive && p1->tut_touchHasEnded){
                            p1->tut_touchHasEnded = false;
                            p1->tutMessage->setString("Keep your finger on the screen");
                            p1->tutColorMessage->setVisible(false);
                        }
                        
                        break;
                    }
                }
            }
        } else if(GameManager::sharedManager()->titleScreenIsActive()){
            if(!GameManager::sharedManager()->firstRun()) {
                if(CCRect::CCRectContainsPoint(tutButton->boundingBox(), touchLocation)){
                    if(GameManager::sharedManager()->tutorialActive == false){
                        GameManager::sharedManager()->tutorialActive = true;
                        printf("tutorial activated\n");
                        animateTutButtonActivation();
                    } else {
                        GameManager::sharedManager()->tutorialActive = false;
                        printf("tutorial deactivated\n");
                        animateTutButtonDeactivation();
                    }
                }
            }

            for(std::list<TitleSprite *>::iterator iter = titleSprites->begin(); iter != titleSprites->end(); ++iter){
                TitleSprite *sp = *iter;
                if(sp->touch == NULL && CCRect::CCRectContainsPoint(sp->boundingBox(), touchLocation)){
                    if(numQueuedPlayers < GameManager::sharedManager()->maxPlayers){
                        printf("gained queued player\n");
                        sp->touch = touch;
                        sp->runQueueingAnimation(titleLayer);
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
                            p1->spawnNewTarget(nextTargetPosition(p1));
                            p1->touchLock = true;
                            adjustTargetSize(p1);
                            p1->gainPoint();
                        }
                    } else {
                        p1->touchLock = false;
                    }
                }
            }
        } else if(GameManager::sharedManager()->titleScreenIsActive()){
            for(std::list<TitleSprite *>::iterator iter = titleSprites->begin(); iter != titleSprites->end(); ++iter){
                TitleSprite *sp = *iter;
                if(CCRect::CCRectContainsPoint(sp->boundingBox(), touchLocation) && sp->touch != touch){
                    sp->touch = NULL;
                    if(!CCRect::CCRectContainsPoint(tutButton->boundingBox(), touchLocation)){
                        if(numQueuedPlayers > 0){
                            printf("lost queued player\n");
                            numQueuedPlayers--;
                        }
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
                    this->removeChild(p1, false);
                    this->removeChild(p1->shineSprite, false);
                    p1->deactivateTouch();
                    p1->losePoint();
                    
                    if(GameManager::sharedManager()->tutorialActive && !p1->tut_touchHasEnded){
                        p1->tut_touchHasEnded = true;
                        p1->tutMessage->setString("Hold                    to continue");
                        p1->tutColorMessage->setString("your color");
                        float sep = 52;
                        if(p1->tutMessage->getPosition().x > this->boundingBox().getMidX()){
                            p1->tutMessage->setPosition(CCPoint(this->boundingBox().getMaxX() - 40, this->boundingBox().getMidY()));
                            p1->tutColorMessage->setPosition(CCPoint(this->boundingBox().getMaxX() - 40, this->boundingBox().getMidY() - sep));
                        } else {
                            p1->tutMessage->setPosition(CCPoint(this->boundingBox().getMinX() + 40, this->boundingBox().getMidY()));
                            p1->tutColorMessage->setPosition(CCPoint(this->boundingBox().getMinX() + 40, this->boundingBox().getMidY() + sep));
                        }
                        p1->tutColorMessage->setVisible(true);
                    }
                }
            }
        } else if(GameManager::sharedManager()->titleScreenIsActive()){
            for(std::list<TitleSprite *>::iterator iter = titleSprites->begin(); iter != titleSprites->end(); ++iter){
                TitleSprite *sp = *iter;
                if(sp->touch == touch){
                    sp->touch = NULL;
                    sp->stopQueueingAnimation();
                    if(numQueuedPlayers > 0){
                        printf("lost queued player\n");
                        numQueuedPlayers--;
                    }
                }
            }
        } else if(GameManager::sharedManager()->pregameIsActive()){
            std::list<Player *> *players = GameManager::sharedManager()->players;
            for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
                Player *p1 = *iter;
                if((CCTouch *)*it == p1->touch){
                    this->removeChild(p1, false);
                    this->removeChild(p1->shineSprite, false);
                    p1->deactivateTouch();
                    p1->losePoint();
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
        p->shrinkTarget();
    } else {
        std::list<Player *> *players = GameManager::sharedManager()->players;
        for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
            Player *p1 = *iter;
            if(p1 != p){
                // grow this player
                p1->growTarget();
            }
        }
    }
}

CCPoint HelloWorld::nextTargetPosition(Player *p){
    float x, y;
    int targetSize = p->currentTarget->boundingBox().size.width;
    CCPoint *point = new CCPoint(p->getPosition().x, p->getPosition().y);
    CCRect playerBounds = p->currentTarget->boundingBox();
    
    while((point->x == 0 && point->y == 0) || CCRect::CCRectContainsPoint(playerBounds, *point)){
        //printf("Retrying position after overlap\n");
        if (p->checkpointCount < GameManager::sharedManager()->goalCheckpoints * 0.4) {
            int territoryAddition = this->boundingBox().size.width / GameManager::sharedManager()->goalCheckpoints * 0.4 / 2;
            p->territory.size.width += territoryAddition;
            
            if (p->getID() == 1) {
                p->territory.origin.x -= territoryAddition;
            }
            
            x = arc4random() % ((int)p->territory.size.width - targetSize) + p->territory.getMinX() + targetSize / 2;
            y = arc4random() % ((int)p->territory.size.height - targetSize) + targetSize / 2;
        } else {
            x = arc4random() % ((int)this->boundingBox().size.width - targetSize) + targetSize / 2;
            y = arc4random() % ((int)this->boundingBox().size.height - targetSize) + targetSize / 2;
        }
        point->setPoint(x, y);
    }
    return *point;
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