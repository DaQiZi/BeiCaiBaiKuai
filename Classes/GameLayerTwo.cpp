#include"GameLayerTwo.h"
#include "cocos2d.h"
#include"SimpleAudioEngine.h"
#include<ctime>
#include "Block.h"
#include"BlockLayer.h"
#include"MenuScene.h"
#include"GameSceneTwo.h"
using namespace cocos2d;
using namespace CocosDenshion;



bool GameLayerTwo::init(){
	if (!Layer::init()){
		return false;
	}
	initData();
	srand(unsigned(time(NULL)));
	winSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	for (int i = 0; i < this->blocklayerNum; ++i){
		auto bl = BlockLayer::create();
		bl->setPosition(Vec2(origin.x+winSize.width/2, origin.y+bl->getContentSize().height/2 + bl->getContentSize().height * i));
		this->addChild(bl);
		blockLayerArr.push_back(bl);
	}
	changeListener(true);
	//cout << winSize.width<<"----->"<<<< endl;
	//Size mysize = Director::getInstance()->getWinSize();
	//log("%f      ----     %f",winSize.width,winSize.height);
	//log("%f      ----     %f", mysize.width, mysize.height);

	std::string fontname = "font.fnt";
	auto pauselabel = Label::createWithBMFont(fontname, "Pause");
	pauselabel->setBMFontSize(40);

	auto pauseitem = MenuItemLabel::create(pauselabel, CC_CALLBACK_0(GameLayerTwo::pauseMenu, this));
	pauseitem->setPosition(Vec2(60, winSize.height - 50));

	pauseButton = Menu::create(pauseitem,NULL);
	pauseButton->setPosition(Vec2::ZERO);
	this->addChild(pauseButton,2);

	//this->touchblocklable = Label::createWithBMFont(fontname,"0");
	this->touchblocklable = Label::createWithBMFont(fontname, "0");
	this->touchblocklable->setPosition(Vec2(winSize.width / 2, winSize.height*0.7));
	this->touchblocklable->setOpacity(125);
	this->touchblocklable->setBMFontSize(100);
	this->addChild(this->touchblocklable, 2);

	//scheduleUpdate();
	//scheduleOnce();
	return true;
}
void GameLayerTwo::update(float dt){
	
	int len = blockLayerArr.size();

	Size blockLayerSize = blockLayerArr[location]->getContentSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float targetheight = -blockLayerArr[location]->getContentSize().height / 2-10;
	for (int i = 0; i < len; ++i){
		unscheduleUpdate();
		if (i != location){
			blockLayerArr[i]->setPositionY(blockLayerArr[i]->getPositionY()-dt*moveSpeed);;
		}
		else{
			
			if (blockLayerArr[i]->getPositionY() > targetheight){
				blockLayerArr[i]->setPositionY(blockLayerArr[i]->getPositionY() - dt*moveSpeed);
			}
			else{
				//判断最下面的那个是否被点击过了
				int blacklocation = blockLayerArr[i]->getBlackBlockLocation();
				if (blockLayerArr[i]->blockArr[blacklocation]->getBlockInfo()){
					completeGame();
					return;
				}
				else{
					//如果被点击过了就移动，没有则游戏失败
					int lastlocation = (this->blocklayerNum + i - 1) % this->blocklayerNum;

					float lastheight = blockLayerArr[lastlocation]->getPositionY() + blockLayerSize.height;
					/*if (i + 1 == this->blocklayerNum){
					lastheight -= 2;
					}*/
					//auto placeto = Place::create(Vec2(origin.x + blockLayerSize.width / 2,lastheight ));
					blockLayerArr[i]->setPosition(Vec2(origin.x + blockLayerSize.width / 2, origin.y + lastheight));
					//blockLayerArr[i]->runAction(placeto);
					blockLayerArr[location]->changeBlackBlock();
					if (i == 0){
						blockLayerArr[i]->setPositionY(blockLayerArr[i]->getPositionY() - dt*moveSpeed);
					}

					location = (location + 1) % this->blocklayerNum;
				}
			}
			
		}
		scheduleUpdate();
	}
	
	
	

}
bool GameLayerTwo::onTouchBegan(Touch* touch, Event* unuse_event)
{
	if (!isfirstTouch){
		isfirstTouch = true;
		scheduleUpdate();
	}
	//判断是否点击到了当前行的黑块。
	Vec2 touchPoint = touch->getLocation();
	int blackLocation = blockLayerArr[this->touchlocation]->getBlackBlockLocation();
	int len = blockLayerArr[this->touchlocation]->blockArr.size();
	for (int i = 0; i < len;++i){

		//getNodeToWorldAffineTransform();
		Rect box = blockLayerArr[this->touchlocation]->blockArr[i]->getBoundingBox();
		Vec2 boxP = blockLayerArr[this->touchlocation]->convertToWorldSpace(box.origin);
		box.origin = boxP;
		//log("convert before %f -----------%f",box.origin.x,box.origin.y);
		bool istouch = box.containsPoint(touchPoint);
		if (istouch&&i==blackLocation){
			//log("i am touch");
			string audioname = StringUtils::format("audio\\%c.wav", music[musicLocation]);
			musicLocation = (musicLocation + 1) % music.length();
			SimpleAudioEngine::getInstance()->playEffect(audioname.c_str());

			blockLayerArr[this->touchlocation]->blockArr[i]->setBlockColor(Color3B::GRAY);
			//检测一个黑块，是不是被点击过了
			blockLayerArr[this->touchlocation]->blockArr[i]->setBlockInfo(false);
			
			this->touchlocation = (this->touchlocation + 1) % blocklayerNum;

			this->touchblocknum += 1;
			if (this->touchblocknum%ACCELERATEINTERVAL == 0 && this->touchblocknum != 0){
				if (moveSpeed<maxSpeed){
					moveSpeed += 100;
				}
			}
			string numstr = StringUtils::format("%d",this->touchblocknum);
			this->touchblocklable->setString(numstr);
			break;
			//加载一个声音
			
		}
		else if (istouch&&i!=blackLocation){

			blockLayerArr[this->touchlocation]->blockArr[i]->setBlockColor(Color3B::RED);
			changeListener(false);
			//gameOver();
			completeGame();
			return false;
		}
	}
	

	return true;
}
void GameLayerTwo::initData(){
	location = 0;
	touchlocation = 0;
	 pauseLayer = nullptr;
	 gameoverLayer = nullptr;
	 //compelteLayer = nullptr;
	 lastLayer = nullptr;
	 //timelable = nullptr;
	 /*blockSums = BLOCKNUM;
	 blockCount = 0;*/
	 touchblocknum = 0;
	 touchblocklable = nullptr;

	 isfirstTouch = false;
	 theMaxBlockNum = UserDefault::getInstance()->getIntegerForKey(MAXTOUCHNUMS.c_str());
	 moveSpeed = 200;
	 maxSpeed = 5000;
	 blocklayerNum = 6;
	 music = "12311231345345565431565431151151";
	 musicLocation = 0;
	// loadAudio();
	 //besttime;
}
void GameLayerTwo::changeBlockColor(int location){

	blockLayerArr[location]->changeBlackBlock();
}

void  GameLayerTwo::changeDirectorEnable(bool flag){
	if (flag){
		Director::getInstance()->resume();
	}
	else{
		Director::getInstance()->pause();
	}
}
void GameLayerTwo::pauseMenu(){
	changeDirectorEnable(false);
	changePauseEnable(false);
	changeListener(false);
	//this->getEventDispatcher()->removeEventListener(listener);
	pauseLayer = LayerColor::create(Color4B::GRAY, winSize.width, winSize.height);
	pauseLayer->setOpacity(125);

	std::string fontname = "font.fnt";
	auto resumelabel1 = Label::createWithBMFont(fontname, "resume");
	resumelabel1->setBMFontSize(100);
	
	auto againlabel1 = Label::createWithBMFont(fontname, "again");
	againlabel1->setBMFontSize(100);

	auto returnlabel1 = Label::createWithBMFont(fontname, "return");
	returnlabel1->setBMFontSize(100);

	auto resumeitem = MenuItemLabel::create(resumelabel1, CC_CALLBACK_0(GameLayerTwo::resumeGame, this));
	resumeitem->setPosition(Vec2(winSize.width / 2, winSize.height / 5));
	auto againitem = MenuItemLabel::create(againlabel1, CC_CALLBACK_0(GameLayerTwo::againGame, this));
	againitem->setPosition(Vec2(winSize.width / 2, winSize.height * 2 / 5));
	auto returnitem = MenuItemLabel::create(returnlabel1, CC_CALLBACK_0(GameLayerTwo::returnMenuScene, this));
	returnitem->setPosition(Vec2(winSize.width / 2, winSize.height*3/5));

	auto menu = Menu::create(resumeitem, againitem, returnitem, NULL);
	menu->setPosition(Vec2::ZERO);
	pauseLayer->addChild(menu);
	//pauseLayer->setSwallowsTouches(true);
	this->addChild(pauseLayer);

	//layer->setTag();
}
//void GameScene1::gameOver(){
//	changePauseEnable(false);
//	changeListener(false);
//	changeDirectorEnable(false);
//	removeTimeLabel();
//	//this->getEventDispatcher()->removeEventListener(listener);
//	gameoverLayer = LayerColor::create(Color4B::GRAY, winSize.width, winSize.height);
//	gameoverLayer->setOpacity(125);
//
//	std::string fontname = "font.fnt";
//
//	auto faillabel = Label::createWithBMFont(fontname, "YOU FAILED");
//	faillabel->setBMFontSize(70);
//	faillabel->setPosition(Vec2(winSize.width / 2, winSize.height * 2/ 3));
//	gameoverLayer->addChild(faillabel);
//
//	auto againlabel1 = Label::createWithBMFont(fontname, "again");
//	againlabel1->setBMFontSize(50);
//
//	auto returnlabel1 = Label::createWithBMFont(fontname, "return");
//	returnlabel1->setBMFontSize(50);
//
//
//	auto againitem = MenuItemLabel::create(againlabel1, CC_CALLBACK_0(GameScene1::againGame, this));
//	againitem->setPosition(Vec2(winSize.width / 2, winSize.height * 1 / 5));
//	auto returnitem = MenuItemLabel::create(returnlabel1, CC_CALLBACK_0(GameScene1::returnMenuScene, this));
//	returnitem->setPosition(Vec2(winSize.width / 2, winSize.height * 2 / 5));
//
//	auto menu = Menu::create(againitem, returnitem, NULL);
//	menu->setPosition(Vec2::ZERO);
//	gameoverLayer->addChild(menu);
//	//pauseLayer->setSwallowsTouches(true);
//	this->addChild(gameoverLayer);
//
//}
void GameLayerTwo::resumeGame(){
	if (pauseLayer){
		pauseLayer->removeFromParent();
		pauseLayer = nullptr;
	}
	if (gameoverLayer){
		gameoverLayer->removeFromParent();
		gameoverLayer = nullptr;
	}
	changeListener(true);
	changePauseEnable(true);
	changeDirectorEnable(true);
	//this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
	//Director::getInstance()->replaceScene();
}
void GameLayerTwo::againGame(){
	changeListener(false);
	changeDirectorEnable(true);
	auto scene = GameSceneTwo::create();
	Director::getInstance()->replaceScene(scene);
	
	//changePauseEnable(true);
	
}
void GameLayerTwo::returnMenuScene(){
	changeListener(false);
	changePauseEnable(true);
	changeDirectorEnable(true);
	auto scene = MenuScene::create();
	for (int i = 1; i <= 7; ++i){
		SimpleAudioEngine::getInstance()->unloadEffect(StringUtils::format("audio\\%d.wav", i).c_str());
	}
	SimpleAudioEngine::getInstance()->unloadEffect("audio\\bgm.mp3");
	Director::getInstance()->replaceScene(scene);
	
}
void GameLayerTwo::changeListener(bool flag){
	if (flag){
		listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = CC_CALLBACK_2(GameLayerTwo::onTouchBegan, this);
		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	}
	else{
		this->getEventDispatcher()->removeEventListener(listener);
		listener = nullptr;
	}
}
void GameLayerTwo::changePauseEnable(bool flag){
	pauseButton->setEnabled(flag);
	//pauseButton->setTouchEnabled(flag);

}
void GameLayerTwo::completeGame(){
	changePauseEnable(false);
	changeListener(false);
	changeDirectorEnable(false);
	removeUpdate();
	removeTouchBlockLabel();
	bool win = false;
	if (this->theMaxBlockNum == 0 || this->theMaxBlockNum<this->touchblocknum){
		UserDefault::getInstance()->setFloatForKey(MAXTOUCHNUMS.c_str(),this->touchblocknum);
		this->theMaxBlockNum = this->touchblocknum;
		win = true;
	}

	//this->getEventDispatcher()->removeEventListener(listener);
	gameoverLayer = LayerColor::create(Color4B::GRAY, winSize.width, winSize.height);
	gameoverLayer->setOpacity(125);

	std::string fontname = "font.fnt";

	auto againlabel1 = Label::createWithBMFont(fontname, "again");
	againlabel1->setBMFontSize(50);

	auto returnlabel1 = Label::createWithBMFont(fontname, "return");
	returnlabel1->setBMFontSize(50);
	if (!win){
		string bestStr = StringUtils::format("Best : %d", this->theMaxBlockNum);
		auto bestlabel1 = Label::createWithBMFont(fontname, bestStr);
		bestlabel1->setBMFontSize(70);
		bestlabel1->setPosition(Vec2(winSize.width / 2, winSize.height * 4 / 5));
		gameoverLayer->addChild(bestlabel1);
		string yourStr = StringUtils::format("Your : %d", this->touchblocknum);
		auto youlabel1 = Label::createWithBMFont(fontname, yourStr);
		youlabel1->setBMFontSize(70);
		youlabel1->setPosition(Vec2(winSize.width / 2, winSize.height * 3 / 5));
		gameoverLayer->addChild(youlabel1);
	}
	else{
		string bestStr = "Your Are";
		auto bestlabel1 = Label::createWithBMFont(fontname, bestStr);
		bestlabel1->setBMFontSize(70);
		bestlabel1->setPosition(Vec2(winSize.width / 2, winSize.height * 4 / 5));
		gameoverLayer->addChild(bestlabel1);
		string yourStr = StringUtils::format("Best : %d", this->theMaxBlockNum);
		auto youlabel1 = Label::createWithBMFont(fontname, yourStr);
		youlabel1->setBMFontSize(70);
		youlabel1->setPosition(Vec2(winSize.width / 2, winSize.height * 3 / 5));
		gameoverLayer->addChild(youlabel1);
	}
	
	

	auto againitem = MenuItemLabel::create(againlabel1, CC_CALLBACK_0(GameLayerTwo::againGame, this));
	againitem->setPosition(Vec2(winSize.width / 2, winSize.height * 1 / 5));
	auto returnitem = MenuItemLabel::create(returnlabel1, CC_CALLBACK_0(GameLayerTwo::returnMenuScene, this));
	returnitem->setPosition(Vec2(winSize.width / 2, winSize.height * 2 / 5));

	auto menu = Menu::create(againitem, returnitem, NULL);
	menu->setPosition(Vec2::ZERO);
	gameoverLayer->addChild(menu);
	//pauseLayer->setSwallowsTouches(true);


	//比较
	int usernums = UserDefault::getInstance()->getIntegerForKey(USER_NUMS.c_str());
	if (usernums<USER_NUMS_INT){
		string userid = StringUtils::format("%s%d", USER_ID_.c_str(),usernums);

		std::vector<int> scoreVector;
		for (int i = 0; i < usernums; ++i){
			string score = StringUtils::format("%s%d", USER_ID_.c_str(), i);
			scoreVector.push_back(UserDefault::getInstance()->getIntegerForKey(score.c_str()));
		}
		scoreVector.push_back(this->touchblocknum);
		sort(scoreVector.begin(),scoreVector.end(),greater<int>());
		for (int i = 0; i < usernums+1; ++i){
			string score = StringUtils::format("%s%d", USER_ID_.c_str(), i);
			UserDefault::getInstance()->setIntegerForKey(score.c_str(), scoreVector[i]);
		}
		//UserDefault::getInstance()->setIntegerForKey(userid.c_str(), this->touchblocknum);
		UserDefault::getInstance()->setIntegerForKey(USER_NUMS.c_str(),++usernums);
	}
	else{
		std::vector<int> scoreVector;
		for (int i = 0; i < usernums; ++i){
			string score = StringUtils::format("%s%d", USER_ID_.c_str(), i);
			scoreVector.push_back(UserDefault::getInstance()->getIntegerForKey(score.c_str()));
		}
		if (scoreVector[usernums - 1]<=this->touchblocknum){
			scoreVector.push_back(this->touchblocknum);
			sort(scoreVector.begin(), scoreVector.end(), greater<int>());
			for (int i = 0; i < usernums; ++i){
				string score = StringUtils::format("%s%d", USER_ID_.c_str(), i);
				UserDefault::getInstance()->setIntegerForKey(score.c_str(),scoreVector[i]);
			}
		}
		
		
	}
	UserDefault::getInstance()->flush();
	this->addChild(gameoverLayer);
}

void GameLayerTwo::removeUpdate(){
	unscheduleUpdate();
	/*this->timelable->removeFromParent();
	this->timelable = nullptr;*/
}
void GameLayerTwo::removeTouchBlockLabel(){
	this->touchblocklable->removeFromParent();
}
//void GameScene1::changeRunning(bool flag){
//	blockLayerArr[location]->isRunning = flag;
//}

//void GameLayerTwo::loadAudio(){
//	for (int i = 1; i <= 7; ++i){
//		SimpleAudioEngine::getInstance()->preloadEffect(StringUtils::format("audio\\%d.mp3", i).c_str());
//	}
//}