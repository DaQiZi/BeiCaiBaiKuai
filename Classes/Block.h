#ifndef __BLOCK_H_
#define __BLOCK_H_
#include"cocos2d.h"
USING_NS_CC;
class Block : public LayerColor{
public:
	//virtual bool init();
	//CREATE_FUNC(Black);
	static Block* create(Color4B color, float w, float h);
	bool initWithBlack(Color4B color, float w, float h);
	void setBlockBlack(bool isBlack);
	void setBlockColor(Color3B color);
	bool getBlockInfo();
	void setBlockInfo(bool flag);
private:
	float  _width;
	float _height;
	Color3B _color;
	bool _isBlack;
};
#endif