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
	int gameTime;	//ゲーム経過時間(秒)
	bool isPlay;	//ゲーム中判定
	int selectColor[MAXPLAYER];	//プレイヤーごとの選択カラー情報
	int gameState;	//ゲーム状態変数
	int rank[MAXPLAYER];	//順位
	int winnerId;
};