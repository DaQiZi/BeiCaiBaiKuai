#include "cocos2d.h"
#include"SimpleAudioEngine.h"
#include<ctime>
#include "Block.h"
#include"BlockLayer.h"
#include"MenuScene.h"
#include"GameLayer.h"
#include"GameScene.h"
using namespace cocos2d;
using namespace CocosDenshion;


bool GameLayer::init(){
	if (!Layer::init()){
		return false;
	}
	initData();
	srand(unsigned(time(NULL)));
	winSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	for (int i = 0; i < 5;++i){
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

	auto pauseitem = MenuItemLabel::create(pauselabel, CC_CALLBACK_0(GameLayer::pauseMenu, this));
	pauseitem->setPosition(Vec2(60, winSize.height - 50));

	pauseButton = Menu::create(pauseitem,NULL);
	pauseButton->setPosition(Vec2::ZERO);
	this->addChild(pauseButton,2);


	timelable = Label::createWithBMFont(fontname,"0.00");
	timelable->setPosition(Vec2(winSize.width/2,winSize.height*0.7));
	timelable->setOpacity(125);
	timelable->setBMFontSize(100);
	this->addChild(timelable,2);

	return true;
}
void GameLayer::update(float dt){
	this->recordtime += dt;
	
	std::string timestr = StringUtils::format("%0.2f", this->recordtime);
	//sprintf_s(timestr,"%0.2f",this->recordtime);
	this->timelable->setString(timestr);
}
bool GameLayer::onTouchBegan(Touch* touch, Event* unuse_event)
{
	if (!isfirstTouch){
		isfirstTouch = true;
		scheduleUpdate();
	}
	//判断是否点击到了当前行的黑块。
	Vec2 touchPoint = touch->getLocation();
	int blackLocation = blockLayerArr[location]->getBlackBlockLocation();
	int len = blockLayerArr[location]->blockArr.size();
	for (int i = 0; i < len;++i){

		//getNodeToWorldAffineTransform();
		Rect box = blockLayerArr[location]->blockArr[i]->getBoundingBox();
		Vec2 boxP = blockLayerArr[location]->convertToWorldSpace(box.origin);
		box.origin = boxP;
		//log("convert before %f -----------%f",box.origin.x,box.origin.y);
		bool istouch = box.containsPoint(touchPoint);
		if (istouch&&i==blackLocation){
			//log("i am touch");
			blockLayerArr[location]->blockArr[i]->setBlockColor(Color3B::GRAY);
			//传入一个临时的变量，对上移的块做记录。

			this->blockCount += 1;
			moveAllBlock(location);
			location = (location + 1) % 5;
			if ((this->blockCount+5)==this->blockSums){
				//添加完成的方块
				lastLayer = LayerColor::create(Color4B::YELLOW, winSize.width, winSize.height);
				float lastLayerH = winSize.height + blockLayerArr[0]->getContentSize().height;
				lastLayer->setPosition(Vec2(0,lastLayerH));
				this->addChild(lastLayer);
				
			}else if (this->blockCount==this->blockSums){
				//游戏完成，记录时间等。
				//log("game complete!");
				log("last layer access-------------");
				completeGame();
			}
			
		}
		else if (istouch&&i!=blackLocation){

			blockLayerArr[location]->blockArr[i]->setBlockColor(Color3B::RED);
			changeListener(false);
			gameOver();

			return false;
		}
	}
	

	return true;
}
void GameLayer::initData(){
	location = 0;
	 pauseLayer = nullptr;
	 gameoverLayer = nullptr;
	 compelteLayer = nullptr;
	 lastLayer = nullptr;
	 timelable = nullptr;
	 blockSums = BLOCKNUM;
	 blockCount = 0;
	 recordtime = 0.00f;
	 isfirstTouch = false;
	 besttime = UserDefault::getInstance()->getFloatForKey(BESTTIME.c_str());
	 //besttime;
}
void GameLayer::moveAllBlock(int location){
	int len = blockLayerArr.size();

	//如果达到了最后的总快，就不要把第一块移动到最上面来了。
	if ((this->blockCount+5)>blockSums){
		Size blockLayerSize = blockLayerArr[location]->getContentSize();
		
		for (int i = 0; i < len; ++i){
			auto action = MoveBy::create(0.05, Vec2(0, -blockLayerSize.height));
			blockLayerArr[i]->runAction(action);
		}
		auto action = MoveBy::create(0.05, Vec2(0, -blockLayerSize.height));
		lastLayer->runAction(action);
	}
	else{
		Size blockLayerSize = blockLayerArr[location]->getContentSize();
		for (int i = 0; i < len; ++i){
			auto action = MoveBy::create(0.05, Vec2(0, -blockLayerSize.height));
			if (i != location){
				blockLayerArr[i]->runAction(action);
			}
			else{
				Vec2 origin = Director::getInstance()->getVisibleOrigin();
				auto placeto = Place::create(Vec2(origin.x + blockLayerSize.width / 2, origin.y + blockLayerSize.height*4.5));
				auto sequence = Sequence::create(action, placeto, CallFunc::create(CC_CALLBACK_0(GameLayer::changeBlockColor, this, location)), NULL);
				blockLayerArr[location]->runAction(sequence);
				//再给这一层，重新设置一次颜色。
				//blockLayerArr[locatino]
			}
		}
	}
	
	
}
void GameLayer::changeBlockColor(int location){
	blockLayerArr[location]->changeBlackBlock();
}

void  GameLayer::changeDirectorEnable(bool flag){
	if (flag){
		Director::getInstance()->resume();
	}
	else{
		Director::getInstance()->pause();
	}
}
void GameLayer::pauseMenu(){
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

	auto resumeitem = MenuItemLabel::create(resumelabel1, CC_CALLBACK_0(GameLayer::resumeGame, this));
	resumeitem->setPosition(Vec2(winSize.width / 2, winSize.height / 5));
	auto againitem = MenuItemLabel::create(againlabel1, CC_CALLBACK_0(GameLayer::againGame, this));
	againitem->setPosition(Vec2(winSize.width / 2, winSize.height * 2 / 5));
	auto returnitem = MenuItemLabel::create(returnlabel1, CC_CALLBACK_0(GameLayer::returnMenuScene, this));
	returnitem->setPosition(Vec2(winSize.width / 2, winSize.height*3/5));

	auto menu = Menu::create(resumeitem, againitem, returnitem, NULL);
	menu->setPosition(Vec2::ZERO);
	pauseLayer->addChild(menu);
	//pauseLayer->setSwallowsTouches(true);
	this->addChild(pauseLayer);

	//layer->setTag();
}
void GameLayer::gameOver(){
	changePauseEnable(false);
	changeListener(false);
	changeDirectorEnable(false);
	removeTimeLabel();
	//this->getEventDispatcher()->removeEventListener(listener);
	gameoverLayer = LayerColor::create(Color4B::GRAY, winSize.width, winSize.height);
	gameoverLayer->setOpacity(125);

	std::string fontname = "font.fnt";

	auto faillabel = Label::createWithBMFont(fontname, "YOU FAILED");
	faillabel->setBMFontSize(70);
	faillabel->setPosition(Vec2(winSize.width / 2, winSize.height * 2/ 3));
	gameoverLayer->addChild(faillabel);

	auto againlabel1 = Label::createWithBMFont(fontname, "again");
	againlabel1->setBMFontSize(50);

	auto returnlabel1 = Label::createWithBMFont(fontname, "return");
	returnlabel1->setBMFontSize(50);


	auto againitem = MenuItemLabel::create(againlabel1, CC_CALLBACK_0(GameLayer::againGame, this));
	againitem->setPosition(Vec2(winSize.width / 2, winSize.height * 1 / 5));
	auto returnitem = MenuItemLabel::create(returnlabel1, CC_CALLBACK_0(GameLayer::returnMenuScene, this));
	returnitem->setPosition(Vec2(winSize.width / 2, winSize.height * 2 / 5));

	auto menu = Menu::create(againitem, returnitem, NULL);
	menu->setPosition(Vec2::ZERO);
	gameoverLayer->addChild(menu);
	//pauseLayer->setSwallowsTouches(true);
	this->addChild(gameoverLayer);

}
void GameLayer::resumeGame(){
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
void GameLayer::againGame(){
	changeListener(false);
	changeDirectorEnable(true);
	auto scene = GameScene::create();
	Director::getInstance()->replaceScene(scene);
	
	//changePauseEnable(true);
	
}
void GameLayer::returnMenuScene(){
	auto scene = MenuScene::create();
	Director::getInstance()->replaceScene(scene);
	changeListener(false);
	changePauseEnable(true);
	changeDirectorEnable(true);
}
void GameLayer::changeListener(bool flag){
	if (flag){
		listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	}
	else{
		this->getEventDispatcher()->removeEventListener(listener);
		listener = nullptr;
	}
}
void GameLayer::changePauseEnable(bool flag){
	pauseButton->setEnabled(flag);
	//pauseButton->setTouchEnabled(flag);

}
void GameLayer::completeGame(){
	changePauseEnable(false);
	changeListener(false);
	changeDirectorEnable(false);
	removeTimeLabel();
	bool win = false;
	if (besttime==0.0||besttime>recordtime){
		UserDefault::getInstance()->setFloatForKey(BESTTIME.c_str(),this->recordtime);
		this->besttime = recordtime;
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
		string bestStr = StringUtils::format("Best : %0.2fs",this->besttime);
		auto bestlabel1 = Label::createWithBMFont(fontname, bestStr);
		bestlabel1->setBMFontSize(70);
		bestlabel1->setPosition(Vec2(winSize.width / 2, winSize.height * 4 / 5));
		gameoverLayer->addChild(bestlabel1);
		string yourStr = StringUtils::format("Your : %0.2fs", this->recordtime);
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
		string yourStr = StringUtils::format("Best : %0.2fs", this->recordtime);
		auto youlabel1 = Label::createWithBMFont(fontname, yourStr);
		youlabel1->setBMFontSize(70);
		youlabel1->setPosition(Vec2(winSize.width / 2, winSize.height * 3 / 5));
		gameoverLayer->addChild(youlabel1);
	}
	


	auto againitem = MenuItemLabel::create(againlabel1, CC_CALLBACK_0(GameLayer::againGame, this));
	againitem->setPosition(Vec2(winSize.width / 2, winSize.height * 1 / 5));
	auto returnitem = MenuItemLabel::create(returnlabel1, CC_CALLBACK_0(GameLayer::returnMenuScene, this));
	returnitem->setPosition(Vec2(winSize.width / 2, winSize.height * 2 / 5));

	auto menu = Menu::create(againitem, returnitem, NULL);
	menu->setPosition(Vec2::ZERO);
	gameoverLayer->addChild(menu);
	//pauseLayer->setSwallowsTouches(true);
	this->addChild(gameoverLayer);
}

void GameLayer::removeTimeLabel(){
	unscheduleUpdate();
	this->timelable->removeFromParent();
	this->timelable = nullptr;
}