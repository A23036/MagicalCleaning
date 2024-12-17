#pragma once
#include "SceneBase.h"
#include "PlayDisplay.h"
#include "SplitScreenLastDraw.h"
#include "DataCarrier.h"
#include "CsvReader.h"
#include "Dust.h"
#include "Stage.h"
#include "Sky.h"
#include "Camera.h"

class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;

	void CsvLoad();
	void DustDestroyed(Dust* dust);
	void CalcRank();

private:
	PlayDisplay* pd;
	SplitScreenLastDraw* ssld;
	DataCarrier* dc;
	CsvReader* csv;
	Stage* st;
	Sky* sky;
	Camera* cm;

	list<Player*> players;

	Player* p1;
	Player* p2;
	Player* p3;
	Player* p4;

	int timer;		//ゲーム経過時間(秒)
	int frm;		//ゲーム経過時間(フレーム)
	int GameTime;	//ゲーム時間
	bool isPlay;	//ゲーム中フラグ
	int rank[MAXPLAYER];	//順位

	static const int MAX_DUST_NUM = 30; //最大生成ゴミ数
	std::vector<Dust*> dustArray;

	void UpdateReady();
	void UpdatePose();
	void UpdateGamePlay();
	void UpdateFinish();
	VECTOR3 GetRandomPosition();
	int GetRandomSize();

	enum GameState {
		sReady = 0,	//開始前カウントダウン
		sPose,		//ポーズ中
		sGamePlay,	//ゲームプレイ中
		sFinish,	//ゲーム終了演出中
	};
	
	GameState state; //ゲーム状態変数

};