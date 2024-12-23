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
	void SetColor(int color[MAXPLAYER]);
	void SetCameraSetteing(int setting[MAXPLAYER]);
	void SetGameState(int state);
	void SetWinnerId(int id);
	void SetRank(int rank[MAXPLAYER]);
	void SetScore(int score,int playerNum);
	void SetPowerLevel(int power, int level, int playerNum);

	int GetGameTime() { return gameTime; };
	bool GetIsPlay() { return isPlay; };
	int GetColor(int playerNum) { return selectColor[playerNum]; };
	int GetCameraSetting(int playerNum) { return cameraSetting[playerNum]; };
	int GetGameState() { return gameState; };
	int GetWinnerId() { return winnerId; };
	int GetRank(int num) { return rank[num]; };
	int GetScore(int playerNum){ return score[playerNum]; };
	int GetPowerLevel(int power, int playerNum) { return powerLevel[power][playerNum]; };

private:
	int gameTime;	//ゲーム経過時間(秒)
	bool isPlay;	//ゲーム中判定
	int selectColor[MAXPLAYER];		//プレイヤーごとの選択カラー
	int cameraSetting[MAXPLAYER];	//プレイヤーごとのカメラ設定
	int gameState;	//ゲーム状態変数
	int rank[MAXPLAYER];	//順位
	int winnerId;
	int score[MAXPLAYER];			//最終スコア
	int powerLevel[5][MAXPLAYER];	//各能力最終レベル
};