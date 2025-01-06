#pragma once
#include "Object3D.h"
#include "Direct3D.h"
#include "Player.h"
#include "Stage.h"
#include "EasingCalc.h"

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

	void Transition();
	
	bool GetIsTransFinish() { return isTransFinish; };
	void SetState(int state);
	void SetGameTime(int time);
	void SetIsTransFinish(bool isTrans);

	void SetPlayers(vector<Player*> players);

	void LoadMap();

private:
	D3D11_VIEWPORT vpSingle;

	EasingCalc* ec;
	Stage* st;

	CSprite* sprite;
	CSpriteImage* frameUiImage;	//画面枠UI
	CSpriteImage* playUiImage;	//プレイ画面UI

	vector<CSpriteImage*> mapImages;	//マップイメージデータ配列

	vector<Player*> players;

	int gameState;
	int gameTime;

	int fadeFrm;
	int transFrm;

	bool isTransFinish;

	enum GameState {
		sTransition = 0, //画面トランジション
		sReady,		//開始前カウントダウン
		sPose,		//ポーズ中
		sGamePlay,	//ゲームプレイ中
		sFinish,	//ゲーム終了演出中
	};
};