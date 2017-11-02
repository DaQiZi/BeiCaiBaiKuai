#ifndef  __GAME_LAYER_TWO__H__
#define  __GAME_LAYER_TWO__H__
#include "cocos2d.h"
#include"BlockLayer.h"
#include"define.h"
using namespace cocos2d;



class GameLayerTwo : public Layer{
public:
    virtual bool init();  
	CREATE_FUNC(GameLayerTwo);
	virtual bool onTouchBegan(Touch* touch,Event* unuse_event);
	void initData();
	void changeBlockColor(int location);
	void pauseMenu();
	void resumeGame();
	void againGame();
	void returnMenuScene();
	void changeListener(bool flag);
	void changePauseEnable(bool flag);
	void changeDirectorEnable(bool flag);
	void completeGame();
	void update(float dt);
	void removeUpdate();
	void removeTouchBlockLabel();
	//void loadAudio();
private:
	Size winSize;
	std::vector<BlockLayer*> blockLayerArr;
	int location;
	int touchlocation;
	EventListenerTouchOneByOne * listener;
	LayerColor* pauseLayer;
	LayerColor* gameoverLayer;
	Menu* pauseButton;
	LayerColor* lastLayer;
	Label* touchblocklable;
	int touchblocknum;
	int theMaxBlockNum;
	bool isfirstTouch;
	//float besttime;
	float moveSpeed;
	float maxSpeed;
	int blocklayerNum;
	std::string music;
	int musicLocation;
};
#endif

