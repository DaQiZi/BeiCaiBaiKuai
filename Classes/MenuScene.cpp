
#include "cocos2d.h"
#include "MenuScene.h"
#include "MenuLayer.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;
using namespace CocosDenshion;

bool MenuScene::init(){
	if (!Scene::init()){
		return false;
	}
	//SimpleAudioEngine::getInstance()->preloadBackgroundMusic("audio\\bgm.mp3");
	winSize = Director::getInstance()->getVisibleSize();
	loadAudio();
	SimpleAudioEngine::getInstance()->playBackgroundMusic("audio\\bgm.mp3",true);
	auto menulayer = MenuLayer::create();
	this->addChild(menulayer);
	return true;
}

void MenuScene::loadAudio(){
	for (int i = 1; i <= 7;++i){
		SimpleAudioEngine::getInstance()->preloadEffect(StringUtils::format("audio\\%d.wav",i).c_str());
	}
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("audio\\bgm.mp3");
}
