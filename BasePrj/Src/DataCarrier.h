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

	int GetGameTime() { return gameTime; };
	bool GetIsPlay() { return isPlay; };
	int GetGameState() { return gameState; };

private:
	int gameTime;	//ƒQ[ƒ€Œo‰ßŠÔ(•b)
	bool isPlay;	//ƒQ[ƒ€’†”»’è
	int gameState;	//ƒQ[ƒ€ó‘Ô•Ï”
};