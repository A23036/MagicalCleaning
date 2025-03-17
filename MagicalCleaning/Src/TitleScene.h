#pragma once
#include "SceneBase.h"

//プロトタイプ宣言
class EasingCalc;
class CsvReader;
class DataCarrier;

// ---------------------------------------------------------------------------
// タイトル画面処理
// ---------------------------------------------------------------------------
class TitleScene : public SceneBase
{
public:
	TitleScene();
	~TitleScene();
	void Init();
	void Update() override;
	void Draw() override;

	void CsvLoad(); //CSVファイルからデータの設定を行う

private:
	//各種データ管理クラスのポインタ
	CsvReader*	csv;
	EasingCalc*	 ec;
	DataCarrier* dc;
	CSpriteImage*	titleBackImage;		//タイトル画面背景イメージ
	CSpriteImage*	broomImage;			//タイトル画面「箒」イメージ
	CSpriteImage*	titleText1Image;	//「まほうつかいの」テキストイメージ
	CSpriteImage*	titleText2Image;	//「おそうじ大作戦」テキストイメージ
	CSpriteImage*	titleText3Image;	//「PRESS ANYBUTTON」テキストイメージ
	CSprite* sprite;

	//CSVファイルからロードされる定数
	float AnimTime[4];	//各アニメーションの時間
	float ScaleStart;	//拡大率の開始値
	float ScaleGoal;	//拡大率の終了値
	float RotateStart;	//回転の開始値
	float RotateGoal;	//回転の終了値
	float TransStart;	//位置の開始値
	float TransGoal;	//位置の終了値
	float AlphaStart;	//透明度の開始値
	float AlphaGoal;	//透明度の終了値

	//アニメーションごとの進行度計算用変数
	float timeRate;		//アニメーションの進行度（0.0～1.0）
	float rate;			//進行度を補間処理に適用した結果（0.0～1.0）
	int	  animFrm;		//アニメーションの経過時間(フレーム)
	float animTime;		//アニメーションの経過時間(秒)

	//アニメーション進行中の値を扱う変数
	float alpha;	//透明度
	float rot;		//回転
	float scale;	//拡縮
	float trans;	//位置

	float moveY;		//入力待機中のスプライトの上下運動に使用
	float moveScale;	//入力待機中のスプライトの拡縮に使用
	float offX;
	float offY;

	//タイトル画面のアニメーション状態
	enum AnimState {
		aFadeIn = 0,
		aBroom,
		aText1,
		aText2,
		aWait,
		aTransition,
		aFinish,
	};

	AnimState state;
};
