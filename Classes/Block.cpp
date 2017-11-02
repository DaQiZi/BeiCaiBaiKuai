#include"Block.h"
Block* Block::create(Color4B color, float w, float h){

	Block* block = new Block();
	
	if (block&&block->initWithBlack(color, w, h)){
		block->autorelease();
		//black->initWithBlack(color,w,h);
		return block;
	}
	CC_SAFE_DELETE(block);
	return nullptr;
}
//bool Black::init(){
//	if (!LayerColor::init()){
//		return false;
//	}
//	return true;
//}

bool Block::initWithBlack(Color4B color, float w, float h){
	
	if (!LayerColor::initWithColor(color,w,h)){
		return false;
	}
	_isBlack = false;
	/*this->setContentSize(Size(w,h));
	this->setColor(color);*/
	return true;
}

void Block::setBlockBlack(bool isBlack){
	//_isBlack = isBlack;
	//±ä°×ºÍ±äÂÌ
	setBlockInfo(isBlack);
	if (_isBlack){
		//_isBlack = isBlack;
		this->setColor(Color3B::GREEN);
	}
	else{
		//_isBlack = isBlack;
		this->setColor(Color3B::WHITE);
	}
	
}
void Block::setBlockColor(Color3B color){

	this->setColor(color);
}
bool Block::getBlockInfo(){
	return _isBlack;
}
void Block::setBlockInfo(bool flag){
	_isBlack = flag;
}