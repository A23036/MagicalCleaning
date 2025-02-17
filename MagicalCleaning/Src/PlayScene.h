#pragma once
#include "SceneBase.h"
#include "Player.h"

class PlayDisplay;
class SplitScreenLastDraw;
class DataCarrier;
class CsvReader;
class Leaf;
class Stage;
class Sky;
class Camera;
class PlayDisplay;

using namespace std;

class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;

	void CsvLoad();
	void LeafDestroyed(Leaf* dust);
	void SetFinalRank();

private:
	PlayDisplay* pd;
	SplitScreenLastDraw* ssld;
	DataCarrier* dc;
	CsvReader* csv;
	Stage* st;
	Sky* sky;
	Camera* cm;

	vector<Player*> players;

	Player* p1;
	Player* p2;
	Player* p3;
	Player* p4;

	int timer;		//ゲーム経過時間(秒)
	int frm;		//ゲーム経過時間(フレーム)
	int GameTime;	//ゲーム時間
	bool isPlay;	//ゲーム中フラグ
	int rank[MAXPLAYER];	//順位

	static const int MAX_DUST_NUM = 50; //最大生成ゴミ数
	std::vector<Leaf*> dustArray;

	void UpdateTransition();
	void UpdateReady();
	void UpdatePose();
	void UpdateGamePlay();
	void UpdateFinish();
	VECTOR3 GetRandomPosition();
	int GetRandomSize();

	enum GameState {
		sTransition = 0, //画面トランジション
		sReady,		//開始前カウントダウン
		sPose,		//ポーズ中
		sGamePlay,	//ゲームプレイ中
		sFinish,	//ゲーム終了演出中
	};
	
	GameState state; //ゲーム状態変数

};