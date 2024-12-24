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
	void SetRank(int rank[MAXPLAYER]);
	void SetScore(int score,int playerNum);
	void SetPowerLevel(int power, int level, int playerNum);
	void SetMoveDistance(int moveDistance,int playerNum);
	void SetJumpCount(int jumpCount, int playerNum);
	void SetKnockOutCount(int knockOutCount, int playerNum);
	void SetItemCount(int itemCount, int playerNum);
	void SetCleanReafCount(int cleanReafCount, int playerNum);
	void SetBlowCount(int blowCount, int playerNum);

	int GetGameTime() { return gameTime; };
	bool GetIsPlay() { return isPlay; };
	int GetColor(int playerNum) { return selectColor[playerNum]; };
	int GetCameraSetting(int playerNum) { return cameraSetting[playerNum]; };
	int GetGameState() { return gameState; };
	int GetRank(int num) { return rank[num]; };
	int GetScore(int playerNum){ return score[playerNum]; };
	int GetPowerLevel(int power, int playerNum) { return powerLevel[power][playerNum]; };
	float GetMoveDistance(int playerNum) { return moveDistance[playerNum]; };
	int GetJumpCount(int playerNum) {return jumpCount[playerNum];};
	int GetKnockOutCount(int playerNum) { return knockOutCount[playerNum]; };
	int GetItemCount(int playerNum) { return itemCount[playerNum]; };
	int GetCleanReaf(int playerNum) { return cleanReafCount[playerNum]; };
	int GetBlowCount(int playerNum) { return blowCount[playerNum]; };

private:
	int gameTime;	//ゲーム経過時間(秒)
	bool isPlay;	//ゲーム中判定
	int selectColor[MAXPLAYER];		//プレイヤーごとの選択カラー
	int cameraSetting[MAXPLAYER];	//プレイヤーごとのカメラ設定
	int gameState;	//ゲーム状態変数
	int rank[MAXPLAYER];	//順位
	int score[MAXPLAYER];			//最終スコア
	int powerLevel[5][MAXPLAYER];	//各能力最終レベル

	float moveDistance[MAXPLAYER];
	int jumpCount[MAXPLAYER];
	int knockOutCount[MAXPLAYER];
	int itemCount[MAXPLAYER];
	int cleanReafCount[MAXPLAYER];
	int blowCount[MAXPLAYER];
};