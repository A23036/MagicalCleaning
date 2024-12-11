#pragma once
#include "SceneBase.h"
#include "PlayDisplay.h"
#include "SplitScreenLastDraw.h"
#include "DataCarrier.h"
#include "CsvReader.h"
#include "Dust.h"

class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;

	void CsvLoad();
	void DustDestroyed(Dust* dust);

private:
	PlayDisplay* pd;
	SplitScreenLastDraw* ssld;
	DataCarrier* dc;
	CsvReader* csv;

	int timer;		//ゲーム経過時間(秒)
	int frm;		//ゲーム経過時間(フレーム)
	int GameTime;	//ゲーム時間
	bool isPlay;	//ゲーム中フラグ

	static const int MAX_DUST_NUM = 20; //最大生成ゴミ数
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