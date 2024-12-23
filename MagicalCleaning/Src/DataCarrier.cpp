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

void DataCarrier::SetColor(int color[MAXPLAYER])
{
	for (int i = 0; i < MAXPLAYER; i++) {
		selectColor[i] = color[i];
	}
}

void DataCarrier::SetCameraSetteing(int setting[MAXPLAYER])
{
	for (int i = 0; i < MAXPLAYER; i++) {
		cameraSetting[i] = setting[i];
	}
}

void DataCarrier::SetGameState(int state)
{
	gameState = state;
}

void DataCarrier::SetWinnerId(int id)
{
	winnerId = id;
}

void DataCarrier::SetRank(int rank[MAXPLAYER])
{
	for (int i = 0; i < MAXPLAYER; i++) {
		this->rank[i] = rank[i];
	}
}

void DataCarrier::SetScore(int score, int playerNum)
{
	this->score[playerNum] = score;
}

void DataCarrier::SetPowerLevel(int power, int level, int playerNum)
{
	powerLevel[power][playerNum] = level;
}
