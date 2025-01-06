#pragma once
#include "Object3D.h"
#include "Direct3D.h"
#include "Player.h"
#include "Stage.h"

/// 画面分割クラス
/// </summary>
class SplitScreenLastDraw : public Object3D
{
public:
	SplitScreenLastDraw();
	~SplitScreenLastDraw();

	void Start() override;
	void Update() override;
	void Draw() override;
	
	void SetState(int state);
	void SetGameTime(int time);

	void SetPlayers(vector<Player*> players);

	void LoadMap();

private:
	D3D11_VIEWPORT vpSingle;

	Stage* st;

	CSprite* sprite;
	CSpriteImage* frameUiImage;	//画面枠UI
	CSpriteImage* playUiImage;	//プレイ画面UI

	vector<CSpriteImage*> mapImages;	//マップイメージデータ配列

	vector<Player*> players;

	int gameState;
	int gameTime;

	int fadeFrm;

	enum GameState {
		sReady = 0,	//開始前カウントダウン
		sPose,		//ポーズ中
		sGamePlay,	//ゲームプレイ中
		sFinish,	//ゲーム終了演出中
	};
};