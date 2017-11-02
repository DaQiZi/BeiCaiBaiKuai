#include"BlockLayer.h"
BlockLayer* BlockLayer::create(){
	BlockLayer* blockL = new BlockLayer();
	if (blockL&&blockL->initBlackLayer()){
		blockL->autorelease();
		return blockL;
	}
	CC_SAFE_DELETE(blockL);
	return nullptr;
}
bool BlockLayer::initBlackLayer(){
	if (!Node::init()){
		return false;
	}
	initData();
	Size winsize = Director::getInstance()->getVisibleSize();
	Size blockLayerSize(winsize.width,winsize.height/4.0);
	this->setContentSize(blockLayerSize);
	this->setAnchorPoint(Vec2(0.5,0.5));

	Size blockSize(blockLayerSize.width/4.0 -2,blockLayerSize.height -2);
	for (int i = 0; i < 4;++i){
		auto block = Block::create(Color4B::WHITE, blockSize.width, blockSize.height);
		//black->setAnchorPoint(Vec2(0.5, 0.5));
		block->setPosition(Vec2(i * blockLayerSize.width/4, 0));
		this->addChild(block);
		blockArr.push_back(block);
	}
	changeBlackBlock();
	return true;
}

void BlockLayer::changeBlackBlock(){
	//把原来的修改为false，现在的改成true
	blockArr[blackBlock]->setBlockBlack(false);
	int location = rand() % 4;
	blockArr[location]->setBlockBlack(true);
	blackBlock = location;
}
void BlockLayer::initData(){
	blackBlock = 0;
	isRunning = false;
}
int BlockLayer::getBlackBlockLocation(){
	return blackBlock;
}