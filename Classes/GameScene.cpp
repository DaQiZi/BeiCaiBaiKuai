
#include "cocos2d.h"
#include"GameScene.h"
#include"GameLayer.h"
using namespace cocos2d;
bool GameScene::init(){
	if (!Scene::init()){
		return false;
	}
	auto gamelayer = GameLayer::create();
	this->addChild(gamelayer);
	return true;
}