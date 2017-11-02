#ifndef  __GAMELAYER__H__
#define  __GAMELAYER__H__
#include "cocos2d.h"
#include"BlockLayer.h"
#include"define.h"
using namespace cocos2d;



class GameLayer: public Layer{
public:
    //static Scene* createScene();
    virtual bool init();  
	CREATE_FUNC(GameLayer);
	virtual bool onTouchBegan(Touch* touch,Event* unuse_event);
	void initData();
	void moveAllBlock(int location);
	void changeBlockColor(int location);
	void pauseMenu();
	void gameOver();
	void resumeGame();
	void againGame();
	void returnMenuScene();
	void changeListener(bool flag);
	void changePauseEnable(bool flag);
	void changeDirectorEnable(bool flag);
	void completeGame();
	void update(float dt);
	void removeTimeLabel();
	//void addTime(float );
private:
	Size winSize;
	std::vector<BlockLayer*> blockLayerArr;
	int location;
	EventListenerTouchOneByOne * listener;
	LayerColor* pauseLayer;
	LayerColor* gameoverLayer;
	LayerColor* compelteLayer;
	Menu* pauseButton;
	int blockSums;
	int blockCount;
	LayerColor* lastLayer;
	Label* timelable;
	float recordtime;
	bool isfirstTouch;
	float besttime;
};
#endif

