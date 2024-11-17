#include "DataCarrier.h"

DataCarrier::DataCarrier()
{
	ObjectManager::DontDestroy(this);		// DataCarrier‚ÍÁ‚³‚ê‚È‚¢
	ObjectManager::SetVisible(this, false);		// DataCarrier‚Í•\Ž¦‚µ‚È‚¢
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
