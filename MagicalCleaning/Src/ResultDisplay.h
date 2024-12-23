#pragma once
#include "Object3D.h"
#include "DataCarrier.h"
#include "EasingCalc.h"

class ResultDisplay : public Object3D
{
public:
	ResultDisplay();
	~ResultDisplay();

	void Update() override;
	void Draw() override;

	void DrawFinish(int nowAnim);
private:
	DataCarrier* dc;
	EasingCalc* ec;
	CSprite* sprite;
	CSpriteImage* resultUiImage1;	//セレクト画面UIイメージ1

	float timeRate;
	float rate;

	int animFrm;		//アニメーションの経過時間(フレーム)
	float animTime;		//アニメーションの経過時間(秒)


	enum animState {
		aFade = 0,
		aStart,
		aChart1,
		aChart2,
		aBonus,
		aScore,
		aRank,
		aFinish,
	};

	animState state;
};