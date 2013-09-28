#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "CCLayout.h"
#include "GameOverScene.h"

using namespace cocos2d;
using namespace CocosDenshion;

cocos2d::CCLabelTTF *_totalScore;
cocos2d::CCLabelTTF *_lifeLeft;
int _score;
int _life;
CCSprite *_targetNow;

HelloWorld::~HelloWorld() {
	CC_SAFE_RELEASE_NULL(_targets);
	CC_SAFE_RELEASE_NULL(_projectiles);
}

HelloWorld::HelloWorld() :
		_targets(NULL), _projectiles(NULL) {

}

CCScene *HelloWorld::scene() {
	CCScene *scene = CCScene::create();
	HelloWorld *layer = HelloWorld::create();
	scene->addChild(layer);
	return scene;
}

bool HelloWorld::init() {
	if (!CCLayerColor::initWithColor(ccc4(255, 255, 255, 255))) {
		return false;
	}

	this->setTouchEnabled(true);
	CCMenuItemImage *pCloseItem = CCMenuItemImage::create("icon_exit.png",
			"icon_exit2.png", this,
			menu_selector(HelloWorld::menuCloseCallback));
	pCloseItem->setPosition(
			ccp(CCDirector::sharedDirector()->getWinSize().width - 32, 32));
	CCMenu *pMenu = CCMenu::create(pCloseItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);

	CCLabelTTF *pLabel = CCLabelTTF::create("飞机大战", "FZMWFont.ttf", 30);
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	pLabel->setPosition(ccp(size.width / 2, size.height - 30));
	pLabel->setFontFillColor(ccc3(0, 0, 0));
	this->addChild(pLabel, 1);

	// add game background.
	CCSprite *bgSprite = CCSprite::create("bg_game.png");
	bgSprite->setScale(CCGetScaleX());
	bgSprite->setScale(CCGetScaleY());
	bgSprite->setPosition(CCGetCenterXY());
	this->addChild(bgSprite, 0);

	// add Total Score display.
	_score = 0;
	_totalScore = CCLabelTTF::create("分数：0", "FZMWFont.ttf", 40);
	_totalScore->setPosition(ccp(_totalScore->getContentSize().width / 2 + 10
			, size.height - _totalScore->getContentSize().height / 2 - 10));
	_totalScore->setFontFillColor(ccc3(255, 0, 0));
	this->addChild(_totalScore, 1);

	// add life left display.
	_life = 10;
	_lifeLeft = CCLabelTTF::create("生命：10", "FZMWFont.ttf", 40);
	_lifeLeft->setPosition(ccp(_lifeLeft->getContentSize().width / 2 + 10
			, size.height - _lifeLeft->getContentSize().height - 30));
	_lifeLeft->setFontFillColor(ccc3(255, 0, 0));
	this->addChild(_lifeLeft, 1);

	pSprite = CCSprite::create("my_plane.png");
	pSprite->setPosition(ccp(size.width / 2, 100));
	this->addChild(pSprite, 0);

	this->schedule(schedule_selector(HelloWorld::gameLogic), 1.0);
	this->schedule(schedule_selector(HelloWorld::update));

	_targets = new CCArray();
	_projectiles = new CCArray();

	//play background sound.
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("game_music.mp3",
			true);

	//add pause button
	CCMenuItemSprite *btn_pause_1 = CCMenuItemSprite::create(
			CCSprite::create("audio_btn_on.png"), NULL);
	CCMenuItemSprite *btn_pause_2 = CCMenuItemSprite::create(
			CCSprite::create("audio_btn_off.png"), NULL);
	CCMenuItemToggle *itemToggle = CCMenuItemToggle::createWithTarget(this,
			menu_selector(HelloWorld::pauseToggle), btn_pause_1, btn_pause_2,
			NULL);
	CCMenu *btn_pause = CCMenu::create(NULL);
	btn_pause->addChild(itemToggle);
	btn_pause->setPosition(ccp(size.width - 94
			, size.height - 40 ));
	this->addChild(btn_pause, 1);

	return true;
}

void HelloWorld::addTarget() {
	CCSprite *target = CCSprite::create("enemy_plan.png",
			CCRectMake(0, 0, 100, 153));
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	int maxX = winSize.width;
	int actualX = rand() % maxX;

	target->setPosition(ccp(actualX, winSize.height));
	this->addChild(target);

	int minDuration = (int) 4.0;
	int maxDuration = (int) 8.0;
	int rangeDuration = maxDuration - minDuration;
	int actualDuration = (rand() % rangeDuration) + minDuration;

	CCPoint point = ccp(actualX, 0);

	CCFiniteTimeAction *actionMove = CCMoveTo::create((float) actualDuration,
			point);
	CCFiniteTimeAction *actionMoveDone = CCCallFuncN::create(this,
			callfuncN_selector(HelloWorld::spriteMoveFinished));

	target->setTag(1);
	_targets->addObject(target);

	target->runAction(CCSequence::create(actionMove, actionMoveDone, NULL));
}

void HelloWorld::spriteMoveFinished(CCNode *sender) {
	CCSprite *sprite = (CCSprite *) sender;
	this->removeChild(sprite, true);

	if (sprite->getTag() == 1) {
		_targets->removeObject(sprite, true);

		//reduce life count.
		//_life -= 1;
		char buffer[20];
		sprintf(buffer, "%s%d", "生命：", _life);
		_lifeLeft->setString(buffer);
		_lifeLeft->setPosition(
				ccp(_lifeLeft->getContentSize().width / 2 + 10
						, CCDirector::sharedDirector()->getWinSize().height - _lifeLeft->getContentSize().height - 30));
		if (_life < 0) {
			GameOverScene *gameOverScene = GameOverScene::create();
			gameOverScene->getLayer()->getLabel()->setString("你输了！！！");
			CCDirector::sharedDirector()->replaceScene(gameOverScene);
			// pause bgm
			SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
			SimpleAudioEngine::sharedEngine()->playEffect("game_over.mp3");
		}
	} else if (sprite->getTag() == 2) {
		_projectiles->removeObject(sprite, true);
	}

}

void HelloWorld::gameLogic(float dt) {
	this->addTarget();
	//与敌机碰撞
	this->collsionTargertWithAir(dt);
}
void HelloWorld::ccTouchesBegan(CCSet* touches, CCEvent* pEvent) {

	//得到触摸点
	CCTouch *touch = (CCTouch *) (touches->anyObject());
	location_press = touch->getLocationInView();
	location_press = CCDirector::sharedDirector()->convertToGL(location_press);
	//

}
void HelloWorld::ccTouchesMoved(CCSet* touches, CCEvent* pEvent) {
	CCPoint cp=pSprite->getPosition();
	//超出屏幕
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();






	//移动点
	CCTouch *touch = (CCTouch *) (touches->anyObject());
	CCPoint location_move = touch->getLocationInView();
	location_move = CCDirector::sharedDirector()->convertToGL(location_move);
	//更新飞机位置
	//右边滑动

	if(location_move.x>location_press.x){
		pSprite->setPositionX(cp.x+10);
	}
	else if(location_move.x<location_press.x){
			pSprite->setPositionX(cp.x-10);
	}

	 if(location_move.y>location_press.y){
				pSprite->setPositionY(cp.y+10);
	}
	else if(location_move.y<location_press.y){
				pSprite->setPositionY(cp.y-10);
	}
	 location_press=location_move;

}

void HelloWorld::ccTouchesEnded(CCSet *touches, CCEvent *event) {
	//飞机停止移动
	pSprite->stopAllActions();



	SimpleAudioEngine::sharedEngine()->playEffect("bullet.mp3");

	CCTouch *touch = (CCTouch *) (touches->anyObject());
	CCPoint location = touch->getLocationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite *projectile = CCSprite::create("icon_bullet.png",
			CCRectMake(0, 0, 58, 58));

	projectile->setPosition(ccp(pSprite->getPositionX(), pSprite->getPositionY()));

	int offX = location.x - projectile->getPosition().x;
	int offY = location.y - projectile->getPosition().y;

	this->addChild(projectile);
	int realX = 0;
	if (offX > 0) {
		realX = winSize.width + (projectile->getContentSize().width / 2);
	} else {
		realX = -winSize.width + (projectile->getContentSize().width / 2);
	}
	float ratio = (float) offY / (float) offX;
	int realY = (realX * ratio) + projectile->getPosition().y;

	CCPoint realDest = ccp(realX, realY);

	int offRealX = realX - projectile->getPosition().x;
	int offRealY = realY - projectile->getPosition().y;
	float length = sqrtf((offRealX * offRealX) + (offRealY * offRealY));

	float speed = 800;
	float realMoveDuration = length / speed;

	projectile->setTag(2);
	_projectiles->addObject(projectile);

	projectile->runAction(
			CCSequence::create(CCMoveTo::create(realMoveDuration, realDest),
					CCCallFuncN::create(this,
							callfuncN_selector(HelloWorld::spriteMoveFinished)),
					NULL));
}

void HelloWorld::update(float dt) {
	CCArray *projectilesToDelete = new CCArray();
	CCArray *targetsToDelete = new CCArray();
	CCObject *it = NULL;
	CCObject *jt = NULL;

	CCARRAY_FOREACH(_projectiles, it) {
		CCSprite *projectile = dynamic_cast<CCSprite *>(it);
		CCRect projectileRect =
				CCRectMake(projectile->getPosition().x - (projectile->getContentSize().width / 2)
						, projectile->getPosition().y - (projectile->getContentSize().height / 2)
						, projectile->getContentSize().width
						, projectile->getContentSize().height);
		CCARRAY_FOREACH(_targets, jt) {
			CCSprite *target = dynamic_cast<CCSprite *>(jt);
			CCRect targetRect =
					CCRectMake(target->getPosition().x - (target->getContentSize().width / 2)
							, target->getPosition().y - (target->getContentSize().height / 2)
							, target->getContentSize().width
							, target->getContentSize().height);
			if (projectileRect.intersectsRect(targetRect)) {
				targetsToDelete->addObject(target);
				projectilesToDelete->addObject(projectile);
			}
		}
	}

	CCARRAY_FOREACH(targetsToDelete, jt) {
		CCSprite *target = dynamic_cast<CCSprite *>(jt);
		//change enemy image.
		target->setTexture(
				CCTextureCache::sharedTextureCache()->addImage(
						"enemy_die.png"));

		// play enemy down effect sound.
		SimpleAudioEngine::sharedEngine()->playEffect("enemy_down.mp3");

		_targetNow = target;

		this->runAction(
				CCSequence::create(CCDelayTime::create(0.2),
						CCCallFuncN::create(this,
								callfuncN_selector(HelloWorld::removeTarget)),
						NULL));
	}

	CCARRAY_FOREACH(projectilesToDelete, it) {
		CCSprite *projectile = dynamic_cast<CCSprite *>(it);
		_projectiles->removeObject(projectile, true);
		this->removeChild(projectile, true);
	}

	projectilesToDelete->release();
	targetsToDelete->release();
}

void HelloWorld::removeTarget() {
	_targets->removeObject(_targetNow, true);
	this->removeChild(_targetNow, true);

	_score += 100;
	char buffer[20];
	sprintf(buffer, "%s%d", "分数：", _score);
	_totalScore->setString(buffer);
	_totalScore->setPosition(
			ccp(_totalScore->getContentSize().width / 2 + 10
					, CCDirector::sharedDirector()->getWinSize().height - _totalScore->getContentSize().height / 2 - 10));

	_targetNow = NULL;
}

void HelloWorld::menuCloseCallback(CCObject *pSender) {
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void HelloWorld::pauseToggle(CCObject *pSender) {
	CCMenuItemToggle *temp = (CCMenuItemToggle *) pSender;
	if (temp->getSelectedIndex() == 0) {
		// resume bgm
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
		// resume game
		CCDirector::sharedDirector()->resume();
		this->setTouchEnabled(true);
	}
	if (temp->getSelectedIndex() == 1) {
		// pause bgm
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
		// pause game
		CCDirector::sharedDirector()->pause();
		this->setTouchEnabled(false);
	}
}






void HelloWorld::collsionTargertWithAir(float dt) {

	if(pSprite==NULL){
		return;
	}
	//取得敌机数量
	int num = _targets->count();

	for (int i = 0; i < num; i++) {


		//取得myplan的位置
		CCPoint cp_plan=pSprite->getPosition();
		CCSize 	size_plan=pSprite->getContentSize();
		//敌机位置
		CCSprite *target=((CCSprite*)_targets->objectAtIndex(i));
		CCPoint cp_target=target->getPosition();
		CCSize size_target=target->getContentSize();
		//判断区域
		CCRect rc_plan=CCRectMake(cp_plan.x-size_plan.width/2,cp_plan.y+size_plan.height/2,size_plan.width,size_plan.height);
		CCRect rc_target=CCRectMake(cp_target.x-size_target.width/2,cp_target.y+size_target.height/2,size_target.width,size_target.height);
		//是否有敌机目标点在飞机区域内
		if(rc_plan.intersectsRect(rc_target)){
			CCLog("与敌机碰撞");
			//扣血
			_life=_life-1;
			//记录敌机状态 dead
			((CCSprite*)_targets->objectAtIndex(i))->setUserData((void*)1);
			//移除

			_targets->removeObject(target,true);
			this->removeChild(target);
			break;
		}




	}

/*	//检测完毕  移除碰撞
	for (int j = num-1; j>=0; j--) {
		int deal=(int)((CCSprite*)_targets->objectAtIndex(j))->getUserData();
		if(deal==1){
			CCLog("移除敌机");
			_targets->removeObject((CCSprite*)_targets->objectAtIndex(j),true);
		}
	}*/

}

