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
	CSpriteImage* resultUiImage1;	//�Z���N�g���UI�C���[�W1

	float timeRate;
	float rate;

	int animFrm;		//�A�j���[�V�����̌o�ߎ���(�t���[��)
	float animTime;		//�A�j���[�V�����̌o�ߎ���(�b)


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