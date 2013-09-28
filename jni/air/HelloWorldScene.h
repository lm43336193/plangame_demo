#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;

class HelloWorld : public cocos2d::CCLayerColor
{
public:
	HelloWorld();
	~HelloWorld();

	virtual bool init();

	static cocos2d::CCScene* scene();

	CREATE_FUNC(HelloWorld);

	void menuCloseCallback(CCObject* pSender);

	void addTarget();

	void spriteMoveFinished(CCNode* sender);

	void gameLogic(float dt);
	 virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	 virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent) ;
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

	void update(float dt);

	void removeTarget();

	void pauseToggle(CCObject* pSender);

	//飞机碰撞
	void collsionTargertWithAir(float dt);
protected:
	cocos2d::CCArray *_targets;
	cocos2d::CCArray *_projectiles;

	//飞机
	CCSprite *pSprite;
	CCPoint location_press ;
};

#endif
