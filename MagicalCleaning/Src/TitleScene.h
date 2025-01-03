#pragma once
#include "SceneBase.h"
#include "EasingCalc.h"
#include "CsvReader.h"
#include "DataCarrier.h"

class TitleScene : public SceneBase
{
public:
	TitleScene();
	~TitleScene();
	void Update() override;
	void Draw() override;

	void CsvLoad(); //CSVファイルからデータの設定

private:
	CsvReader*	csv;
	EasingCalc*	 ec;
	DataCarrier* dc;
	CSpriteImage*	titleBackImage;		//タイトル画面背景イメージ
	CSpriteImage*	broomImage;			//タイトル画面「箒」イメージ
	CSpriteImage*	titleText1Image;	//「まほうつかいの」テキストイメージ
	CSpriteImage*	titleText2Image;	//「おそうじ大作戦」テキストイメージ
	CSpriteImage*	titleText3Image;	//「PRESS ANYBUTTON」テキストイメージ

	CSprite* sprite;

	float timeRate;
	float rate;

	float alpha;	//透明度
	float rot;		//回転
	float scale;	//拡縮
	float trans;	//位置

	float d1;
	float d2;
	float offX;
	float offY;

	float AnimTime[4];
	float ScaleStart;
	float ScaleGoal;
	float RotateStart;
	float RotateGoal;
	float TransStart;
	float TransGoal;
	float AlphaStart;
	float AlphaGoal;
	
	int animFrm;		//アニメーションの経過時間(フレーム)
	float animTime;		//アニメーションの経過時間(秒)

	enum animState { //タイトル画面のアニメーション状態
		aFadeIn = 0,
		aBroom,
		aText1,
		aText2,
		aWait,
		aTransition,
		aFinish,
	};

	animState state;
};
