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

	bool GetIsFinish() { return isFinish; };
	void SetBonusID(int bonus1, int bonus2);
	void SetBonusPlayer(std::vector<int> players1, std::vector<int> players2);

private:
	DataCarrier* dc;
	EasingCalc* ec;
	CSprite* sprite;
	CSpriteImage* resultUiImage1;	//リザルト画面UIイメージ1
	CSpriteImage* resultUiImage2;	//リザルト画面UIイメージ2

	float timeRate;
	float rate;

	int animFrm;		//アニメーションの経過時間(フレーム)
	float animTime;		//アニメーションの経過時間(秒)

	bool isFinish;	//リザルトの終了判定

	int bonus1;
	int bonus2;

	std::vector<int> bonusPlayer1;
	std::vector<int> bonusPlayer2;

	enum animState {
		aFade = 0,
		aStart,
		aChart1,
		aChart2,
		aBonus1,
		aScore1,
		aBonusFade1,
		aBonus2,
		aScore2,
		aBonusFade2,
		aCount,
		aRank,
		aFinish,
		aBack,
	};

	animState state;
};