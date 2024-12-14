#pragma once
#include "Object3D.h"

enum GameState;

class DataCarrier : public Object3D
{
public:
	DataCarrier();
	~DataCarrier();
	void Update() override;

	void SetGameTime(int time);
	void SetIsPlay(bool isPlay);
	void SetGameState(int state);
	void SetWinnerId(int id);
	void SetRank(int rank[MAXPLAYER]);

	int GetGameTime() { return gameTime; };
	bool GetIsPlay() { return isPlay; };
	int GetGameState() { return gameState; };
	int GetWinnerId() { return winnerId; };
	int GetRank(int num) { return rank[num]; };

private:
	int gameTime;	//�Q�[���o�ߎ���(�b)
	bool isPlay;	//�Q�[��������
	int gameState;	//�Q�[����ԕϐ�
	int rank[MAXPLAYER];	//����
	int winnerId;
};