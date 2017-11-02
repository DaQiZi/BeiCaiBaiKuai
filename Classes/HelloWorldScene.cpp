#include "HelloWorldScene.h"

Scene* HelloWorld::createScene()
{
	auto scene = Scene::create();
	auto layer = HelloWorld::create();
	scene->addChild(layer);
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto bg = Sprite::create("HelloWorld.png");
	bg->setPosition(Vec2(500,100));
	this->addChild(bg);


	auto spr = Sprite::create("HelloWorld.png");
	spr->setPosition(Vec2(100,100));
	this->addChild(spr);
	spr->runAction(TintTo::create(2.0,Color3B(245,245,245)));

	
	//auto mt1 = MoveTo::create(2.0,Vec2(100,500));
	//auto sct1 = ScaleTo::create(2.0,2);
	//auto seq = Sequence::create(mt1,sct1,NULL);
	//sprball->runAction(EaseCircleActionIn::crea(seq));
	auto sprball = Sprite::create("ball.png");
	sprball->setPosition(Vec2(50, 56));
	this->addChild(sprball);
	auto ballSeq = Sequence::create(MoveBy::create(2.0f, Vec2(500, 0)),
		DelayTime::create(2.0f),
		ScaleBy::create(2.0f, 2.0f), NULL);
	//ScaleTo
	//FadeIn::create();
	auto ballSeqRever = ballSeq->reverse();

	sprball->runAction(Sequence::create(ballSeq, ballSeqRever, NULL));
	/*SpriteFrameCache::getInstance()->addSpriteFramesWithFile("baoza.plist");
	Vector<SpriteFrame*> v;
	v.reserve(6);
	char name[20];
	for (int i = 1; i <= 6;++i){
		sprintf_s(name,"bao%d.png",i);
		SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
		v.pushBack(frame);
	}
	Animation* animation = Animation::createWithSpriteFrames(v,0.1f);
	animation->setRestoreOriginalFrame(true);*/

	//AnimationCache::getInstance()->addAnimation(animation, BOOM_ANIMATION_NAME);
	//auto listener = EventListenerTouchOneByOne::create();
	//listener->onTouchBegan = [spr](Touch* touch, Event* event)->bool{
	//	//spr->runAction(MoveTo::create(1.0,touch->getLocation()));
	//	/*auto animate = Animate::create(AnimationCache::getInstance()->getAnimation(BOOM_ANIMATION_NAME));
	//	spr->runAction(animate);*/
	//	spr->runAction(MoveTo::create(1.0f,touch->getLocation()));
	//	return false;
	//};
	//this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	//Size winSize = Director::getInstance()->getWinSize();
	//Size mySize = Director::getInstance()->getVisibleSize();
	////cout << winSize.width<<"----->"<<<< endl;
	//Size mysize = Director::getInstance()->getWinSize();
	//log("%f      ----     %f",winSize.width,winSize.height);
	//log("%f      ----     %f", mysize.width, mysize.height);
	////this->addChild(lay);



	
	/*auto mt1 = MoveBy::create(5.0,Vec2(500,0));
	auto mt2 = MoveBy::create(5.0,Vec2(-500,0));
	auto spawn = Spawn::create(mt1, mt2, NULL);
	sprball->runAction(RepeatForever::create(spawn));*/
	//sprball->
	//auto action_mt = MoveBy::create(5.0,Vec2(900,0));
	//auto mtBack = action_mt->reverse();

	//auto mt_ease_in = EaseBounceIn::create(action_mt);
	//auto mt_ease_out = EaseBounceIn::create(mtBack);
	//auto delay = DelayTime::create(2.0);
	//auto repeat = RepeatForever::create(Sequence::create(mt_ease_in, delay, mt_ease_out, delay->clone(), NULL));
	//sprball->runAction(repeat);

	/*Sprite* node = Sprite::create("ball.png");
	Animation* animation1 = Animation::create();
	char filename[20];
	for (int i = 1; i <= 11;++i){
		sprintf_s(filename,"bomb\\baoji_%d.png",i);
		animation1->addSpriteFrameWithFileName(filename);
	}
	animation1->setDelayPerUnit(0.1f);
	animation1->setRestoreOriginalFrame(true);
	node->setPosition(Vec2(100,100));
	this->addChild(node);
	node->runAction(Animate::create(animation1));*/
	//Sequence::Crea
	//this->runAction(Follow::create(spr));
	return true;
}
