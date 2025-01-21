#include "DataCarrier.h"

DataCarrier::DataCarrier()
{
	ObjectManager::DontDestroy(this);		// DataCarrier‚ÍÁ‚³‚ê‚È‚¢
	ObjectManager::SetVisible(this, false);		// DataCarrier‚Í•\¦‚µ‚È‚¢
	gameTime = 0;
	isPlay = false;
	for (int i = 0; i < MAXPLAYER; i++) {
		selectColor[i] = i;
	}
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

void DataCarrier::SetMoveDistance(int moveDistance, int playerNum)
{
	this->moveDistance[playerNum] = moveDistance;
}

void DataCarrier::SetJumpCount(int jumpCount, int playerNum)
{
	this->jumpCount[playerNum] = jumpCount;
}

void DataCarrier::SetKnockOutCount(int knockOutCount, int playerNum)
{
	this->knockOutCount[playerNum] = knockOutCount;
}

void DataCarrier::SetItemCount(int itemCount, int playerNum)
{
	this->itemCount[playerNum] = itemCount;
}

void DataCarrier::SetCleanReafCount(int cleanReafCount, int playerNum)
{
	this->cleanReafCount[playerNum] = cleanReafCount;
}

void DataCarrier::SetBlowCount(int blowCount, int playerNum)
{
	this->blowCount[playerNum] = blowCount;
}
