#include "DataCarrier.h"

DataCarrier::DataCarrier()
{
	ObjectManager::DontDestroy(this);		// DataCarrier�͏�����Ȃ�
	ObjectManager::SetVisible(this, false);		// DataCarrier�͕\�����Ȃ�
	gameTime = 0;
	isPlay = false;
}

DataCarrier::~DataCarrier()
{
}

void DataCarrier::Update()
{
}

void DataCarrier::SetGameTime(int time)
{
	gameTime = time;
}

void DataCarrier::SetIsPlay(bool isPlay)
{
	this->isPlay = isPlay;
}

void DataCarrier::SetGameState(int state)
{
	gameState = state;
}
