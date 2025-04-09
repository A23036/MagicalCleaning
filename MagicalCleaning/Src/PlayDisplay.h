#pragma once
#include "Object3D.h"

//プロトタイプ宣言
class DataCarrier;
class EasingCalc;
class Player;

using namespace std;

// ---------------------------------------------------------------------------
// プレイ画面UI表示処理
// 
// プレイ中のUI描画、スプライトアニメーションを行う
// ---------------------------------------------------------------------------
class PlayDisplay : public Object3D
{
public:
	PlayDisplay();
	virtual ~PlayDisplay();
	void Start() override;
	void Init();

	void Update() override;
	void Draw() override;

	//UIアニメーション描画
	void DrawAnimationUI();
	void DrawCountDown();
	void DrawLastSpurt();
	void DrawFinishCount();
	void DrawFinish();

	//基本UI描画
	void DrawBaseUI(Player* pl);

	void SetBlowPlayer(int atkPlayerNum,int blowPlayerNum);
	void DrawBlowPlayer(int atkPlayerNum, int blowPlayerNum);
	
private:
	//各種クラスのポインタ
	DataCarrier* dc;
	EasingCalc* ec;
	CSprite* sprite;
	CSpriteImage* countDownImage;	//カウントダウンイメージ
	CSpriteImage* playUiImage;		//プレイ画面UIイメージ
	CSpriteImage* playUiImage2;		//プレイ画面UIイメージ2

	int gameState;	//ゲーム状態
	int gameTime;	//ゲーム時間

	bool isLastSpurt;		//ラストスパートフラグ
	bool isPlaySound[4];	//カウントダウンSEを最初だけ再生するためのフラグ
	
	int	  blowPlayerList[MAXPLAYER];	//吹っ飛ばされたプレイヤー
	float blowAnimFrm[MAXPLAYER];		//吹っ飛ばしたプレイヤー表示アニメーション経過時間

	int   animFrm;		//アニメーションの経過時間(フレーム)
	float animTime;		//アニメーションの経過時間(秒)

	//各能力アイコン位置
	int msIconPosX[MAXPLAYER];
	int jnIconPosX[MAXPLAYER];
	int asIconPosX[MAXPLAYER];
	int arIconPosX[MAXPLAYER];
	int cwIconPosX[MAXPLAYER];

	//各能力文字位置
	int msTextPosX[MAXPLAYER];
	int jnTextPosX[MAXPLAYER];
	int asTextPosX[MAXPLAYER];
	int arTextPosX[MAXPLAYER];
	int cwTextPosX[MAXPLAYER];

	//各能力マックスフラグ
	bool isMaxLv[5][MAXPLAYER];

	//ゲーム状態
	enum GameState {
		sTransition = 0,//トランジション中
		sReady,			//開始前カウントダウン
		sPose,			//ポーズ中
		sGamePlay,		//ゲームプレイ中
		sFinish,		//ゲーム終了演出中
	};
};