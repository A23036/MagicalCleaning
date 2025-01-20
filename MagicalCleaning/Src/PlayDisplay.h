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
	CSpriteImage* countDownImage;	//カウントダウンイメージ
	CSpriteImage* playUiImage;		//プレイ画面UIイメージ
	CSpriteImage* playUiImage2;		//プレイ画面UIイメージ2

	DataCarrier* dc;
	EasingCalc* ec;

	int gameState;
	int gameTime;

	bool isLastSpurt;
	bool isPlaySound[4];

	enum GameState {
		sTransition = 0,
		sReady,	//開始前カウントダウン
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

	int msIconPosX[MAXPLAYER];
	int jnIconPosX[MAXPLAYER];
	int asIconPosX[MAXPLAYER];
	int arIconPosX[MAXPLAYER];
	int cwIconPosX[MAXPLAYER];

	int msTextPosX[MAXPLAYER];
	int jnTextPosX[MAXPLAYER];
	int asTextPosX[MAXPLAYER];
	int arTextPosX[MAXPLAYER];
	int cwTextPosX[MAXPLAYER];
	
	bool isMaxLv[5][MAXPLAYER];
};