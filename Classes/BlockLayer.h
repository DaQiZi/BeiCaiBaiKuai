#ifndef __BLOCK_LAYER_H
#define __BLOCK_LAYER_H
#include"cocos2d.h"
#include"Block.h"
#include<vector>
USING_NS_CC;
class BlockLayer: public Node{
public:
	static BlockLayer* create();
	bool initBlackLayer();
	std::vector<Block*> blockArr;
	void changeBlackBlock();
	void initData();
	int getBlackBlockLocation();
	bool isRunning;
private:
	//�ڿ����ĸ�λ�á�
	int blackBlock;
	
};
#endif
