#ifndef  __GAMESCENE__H__
#define  __GAMESCENE__H__
#include "cocos2d.h"

using namespace cocos2d;
class GameScene: public Scene{
public:
    virtual bool init();  
    CREATE_FUNC(GameScene);
private:
	Size winSize;
};
#endif

