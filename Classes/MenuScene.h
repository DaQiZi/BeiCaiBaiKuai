#ifndef  __MENUSCENE__H__
#define  __MENUSCENE__H__
#include "cocos2d.h"
using namespace cocos2d;
class MenuScene: public Scene{
public:
    virtual bool init();  
    CREATE_FUNC(MenuScene);
	void loadAudio();
private:
	Size winSize;
};
#endif

