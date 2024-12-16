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
	int gameTime;	//ƒQ[ƒ€Œo‰ßŠÔ(•b)
	bool isPlay;	//ƒQ[ƒ€’†”»’è
	int gameState;	//ƒQ[ƒ€ó‘Ô•Ï”
	int rank[MAXPLAYER];	//‡ˆÊ
	int winnerId;
};