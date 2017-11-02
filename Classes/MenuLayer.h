#ifndef  __MENULAYER__H__
#define  __MENULAYER__H__
#include "cocos2d.h"
#include"ui\CocosGUI.h"
using namespace cocos2d;
class MenuLayer: public Layer{
public:
    virtual bool init();  
    CREATE_FUNC(MenuLayer);
    void startGame();
	void startGame1();
	void quitGame();
	void initBG();
	void rankToGame1(Layer* layer);
	void rankMenu();
private:
	Size winSize;
	Menu* menu;
};
#endif

