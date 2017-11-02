#include"MenuLayer.h"
#include "cocos2d.h"
#include"ui\CocosGUI.h"
#include"GameScene.h"
#include"GameSceneTwo.h"
#include"define.h"
#include"SimpleAudioEngine.h"

using namespace cocos2d;
using namespace ui;
using namespace CocosDenshion;

bool MenuLayer::init(){
	if (!Layer::init()){
		return false;
	}
	winSize = Director::getInstance()->getVisibleSize();
	initBG();
	std::string fontname = "font.fnt";
	auto title = Label::createWithBMFont(fontname, "Not Touch White block!!");
	title->setBMFontSize(40);

	title->setPosition(Vec2(winSize.width/2,winSize.height*3/4));
	this->addChild(title);

	auto startlabel1 = Label::createWithBMFont(fontname,"start");
	startlabel1->setBMFontSize(100);

	auto startlabel2 = Label::createWithBMFont(fontname, "start2");
	startlabel2->setBMFontSize(100);

	auto quitlabel1 = Label::createWithBMFont(fontname,"quit");
	quitlabel1->setBMFontSize(100);

	auto ranklabel1 = Label::createWithBMFont(fontname, "Rank");
	ranklabel1->setBMFontSize(100);

	auto startitem = MenuItemLabel::create(startlabel1, CC_CALLBACK_0(MenuLayer::startGame, this));
	startitem->setPosition(Vec2(winSize.width / 2, winSize.height*3/ 6));

	auto startitem1 = MenuItemLabel::create(startlabel2, CC_CALLBACK_0(MenuLayer::startGame1, this));
	startitem1->setPosition(Vec2(winSize.width / 2, winSize.height * 2 / 6));
	
	auto quititem = MenuItemLabel::create(quitlabel1, CC_CALLBACK_0(MenuLayer::quitGame, this));
	quititem->setPosition(Vec2(winSize.width/2,winSize.height/6));
	
	auto rankitem = MenuItemLabel::create(ranklabel1, CC_CALLBACK_0(MenuLayer::rankMenu, this));
	rankitem->setPosition(Vec2(winSize.width / 2, winSize.height *4/ 6));

	auto musicSwitch = Label::createWithBMFont(fontname, "Music");
	musicSwitch->setBMFontSize(30);

	auto switchItem = MenuItemLabel::create(musicSwitch, [](Ref* sender){
		if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()){
			SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		}
		else{
			SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		}
	});
	switchItem->setPosition(Vec2(50, 50));

	menu = Menu::create(startitem, startitem1, quititem, rankitem, switchItem, NULL);
	menu->setPosition(Vec2::ZERO);

	this->addChild(menu);
	
	return true;
}


void MenuLayer::startGame1(){
	auto scene = GameSceneTwo::create();
	Director::getInstance()->replaceScene(TransitionCrossFade::create(0.5, scene));
}

void MenuLayer::initBG(){
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	for (int i = 0; i < 5; ++i){
		auto bl = BlockLayer::create();
		bl->setPosition(Vec2(origin.x + winSize.width / 2, origin.y + bl->getContentSize().height / 2 + bl->getContentSize().height * i));
		this->addChild(bl);
	}
}
void MenuLayer::startGame(){
	auto scene = GameScene::create();
	//Director::getInstance()->pushScene(TransitionFadeTR::create(0.5f,scene));
	Director::getInstance()->replaceScene(TransitionRotoZoom::create(0.5,scene));
}
void MenuLayer::quitGame(){
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
void MenuLayer::rankToGame1(Layer* layer){
	int nums = UserDefault::getInstance()->getIntegerForKey(USER_NUMS.c_str());
	std::vector<int> scoreVector;
	for (int i = 0; i < nums;++i){
		string score = StringUtils::format("%s%d",USER_ID_.c_str(),i);
		scoreVector.push_back(UserDefault::getInstance()->getIntegerForKey(score.c_str()));
	}
	if (nums!=0){
		ListView* list = ListView::create();
		list->setContentSize(Size(400,400));
		list->setBackGroundColor(Color3B::GRAY);
		//list->setOpacity(128);
		list->setBackGroundColorOpacity(200);
		list->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
		list->setAnchorPoint(Vec2(0.5,0.5));
		for (int i = 0; i < nums;++i){
			string top = StringUtils::format("%d                ",i+1);
			string score = StringUtils::format("%d score", scoreVector[i]);
			auto textTop = TextBMFont::create(top, "font.fnt");
			auto textScore = TextBMFont::create(score,"font.fnt");
			auto layout = Layout::create();
			layout->setLayoutType(Layout::Type::HORIZONTAL);
			layout->setContentSize(Size(textScore->getContentSize().width*2,textScore->getContentSize().height));
			layout->addChild(textTop);
			layout->addChild(textScore);
			list->pushBackCustomItem(layout);
		}
		list->setPosition(Vec2(winSize.width/2,winSize.height/2-50));
		layer->addChild(list);
	}
	auto labelrank = Label::createWithBMFont("font.fnt","Ranking List");
	labelrank->setPosition(Vec2(winSize.width/2,winSize.height*5/6));
	layer->addChild(labelrank);
}

void MenuLayer::rankMenu(){

	auto rankLayer = LayerColor::create(Color4B::GRAY,winSize.width,winSize.height);
	rankLayer->setOpacity(128);
	menu->setEnabled(false);
	rankToGame1(rankLayer);
	auto startlabel1 = Label::createWithBMFont("font.fnt", "BACK");
	startlabel1->setBMFontSize(50);

	auto startitem = MenuItemLabel::create(startlabel1, [=](Ref* sender){
		this->menu->setEnabled(true);
		
		rankLayer->removeFromParent();

	});
	startitem->setPosition(Vec2(80, winSize.height-50));
	auto menutemp = Menu::create(startitem,NULL);
	menutemp->setPosition(Vec2::ZERO);
	menutemp->setTag(100);
	rankLayer->addChild(menutemp);
	this->addChild(rankLayer);
}