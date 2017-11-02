
#include "cocos2d.h"
#include"GameSceneTwo.h"
#include"GameLayerTwo.h"
using namespace cocos2d;

bool GameSceneTwo::init(){
	if (!Scene::init()){
		return false;
	}
	auto gameLayer = GameLayerTwo::create();
	this->addChild(gameLayer);
	return true;
}