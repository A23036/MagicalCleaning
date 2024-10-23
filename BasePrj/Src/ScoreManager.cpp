#include "ScoreManager.h"

ScoreManager::ScoreManager()
{
	mp = 0;
	weight = 0;
}

ScoreManager::~ScoreManager()
{
}

void ScoreManager::Update()
{
}

void ScoreManager::addMp(int n)
{
	mp += n;
}
