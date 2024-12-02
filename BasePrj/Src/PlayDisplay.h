#pragma once
#include "Object3D.h"
#include "DataCarrier.h"
#include "EasingCalc.h"

/// <summary>
/// プレイヤーごとの各種情報を表示する処理
/// </summary>
class PlayDisplay : public Object3D
{
public:
	PlayDisplay();
	~PlayDisplay();
	void Start() override;
	void Update() override;
	void Draw() override;

private:
	CSprite* sprite;
	CSpriteImage* countDownImage;		//カウントダウンイメージ
	DataCarrier* dc;
	EasingCalc* ec;

	int gameState;
	int gameTime;

	enum GameState {
		sReady = 0,	//開始前カウントダウン
		sPose,		//ポーズ中
		sGamePlay,	//ゲームプレイ中
		sFinish,	//ゲーム終了演出中
	};

	int animFrm;		//アニメーションの経過時間(フレーム)
	float animTime;		//アニメーションの経過時間(秒)

	int rotStart;
	int rotGoal;
	int ScaleStart;
	int ScaleGoal;
};