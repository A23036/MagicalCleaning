#pragma once
#include "SceneBase.h"
#include "CsvPlayDataLoader.h"
#include "Object3D.h"

//プロトタイプ宣言
class PlayDisplay;
class SplitScreenLastDraw;
class DataCarrier;
class CsvReader;
class Player;
class Leaf;
class Stage;
class Sky;
class Camera;
class PlayDisplay;

using namespace std;

// ---------------------------------------------------------------------------
// プレイ画面処理
// 
// ゲーム状態の管理などを行う
// ---------------------------------------------------------------------------
class PlayScene : public SceneBase
{
public:
	PlayScene();
	virtual ~PlayScene();
	void Init();
	void Update() override;

	void LeafDestroyed(Leaf* leaf);
	void SetFinalRank();

	void UpdateTransition();
	void UpdateReady();
	void UpdatePose();
	void UpdateGamePlay();
	void UpdateFinish();
	VECTOR3 GetRandomPosition();
	int GetRandomSize();

	void CsvLoad();

private:
	//各種クラスのポインタ
	PlayDisplay* pd;
	SplitScreenLastDraw* ssld;
	DataCarrier* dc;
	CsvReader* csv;
	Stage* st;
	Sky* sky;
	Camera* cm;

	//各プレイヤーを格納した配列
	vector<Player*> players;
	
	//ステージ上のリーフを格納した配列
	vector<Leaf*> leafArray;

	//各プレイヤーのポインタ
	Player* p1;
	Player* p2;
	Player* p3;
	Player* p4;

	//CSVファイルから読み込んだデータを格納する構造体
	PlayParams playParams;

	//CSVファイルから読み込まれるデータ
	int GameTime;					//ゲーム時間
	int MaxLeafNum;					//最大生成リーフ数
	int LeafSpawnRangeMin;			//リーフ出現最少範囲
	int LeafSpawnRangeMax;			//リーフ出現最大範囲
	int LeafSpawnHeightMin;			//リーフ出現最少高さ
	int LeafSpawnHeightMax;			//リーフ出現最大高さ
	VECTOR3 InitPos[MAXPLAYER];		//各プレイヤーの初期位置

	//ゲームプレイに使用する変数
	int  timer;				//ゲーム経過時間(秒)
	int  frm;				//ゲーム経過時間(フレーム)
	int  rank[MAXPLAYER];	//順位
	bool isPlay;			//ゲーム中フラグ

	enum GameState {
		sTransition = 0,	//画面トランジション
		sReady,				//開始前カウントダウン
		sPose,				//ポーズ中
		sGamePlay,			//ゲームプレイ中
		sFinish,			//ゲーム終了演出中
	};
	
	GameState state;	//ゲーム状態変数

};