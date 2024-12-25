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
	CSpriteImage* resultUiImage1;	//���U���g���UI�C���[�W1
	CSpriteImage* resultUiImage2;	//���U���g���UI�C���[�W2

	float timeRate;
	float rate;

	int animFrm;		//�A�j���[�V�����̌o�ߎ���(�t���[��)
	float animTime;		//�A�j���[�V�����̌o�ߎ���(�b)

	bool isFinish;	//���U���g�̏I������

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