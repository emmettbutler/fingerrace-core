#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;

class HelloWorld : public CCLayer
{
public:
    virtual bool init();
    static CCScene* scene();
    CREATE_FUNC(HelloWorld);
    virtual void ccTouchesBegan(CCSet *touches, CCEvent *event);
    virtual void ccTouchesMoved(CCSet *touches, CCEvent *event);
    virtual void ccTouchesEnded(CCSet *touches, CCEvent *event);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void tick(float dt);
    void resolveTargetCollision();
};

#endif // __HELLOWORLD_SCENE_H__
