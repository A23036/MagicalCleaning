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

	void CsvLoad(); //CSV�t�@�C������f�[�^�̐ݒ�

private:
	CsvReader*	csv;
	EasingCalc*	 ec;
	DataCarrier* dc;
	CSpriteImage*	titleBackImage;		//�^�C�g����ʔw�i�C���[�W
	CSpriteImage*	broomImage;			//�^�C�g����ʁuⴁv�C���[�W
	CSpriteImage*	titleText1Image;	//�u�܂ق������́v�e�L�X�g�C���[�W
	CSpriteImage*	titleText2Image;	//�u������������v�e�L�X�g�C���[�W
	CSpriteImage*	titleText3Image;	//�uPRESS ANYBUTTON�v�e�L�X�g�C���[�W

	CSprite* sprite;

	float timeRate;
	float rate;

	float alpha;	//�����x
	float rot;		//��]
	float scale;	//�g�k
	float trans;	//�ʒu

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
	
	int animFrm;		//�A�j���[�V�����̌o�ߎ���(�t���[��)
	float animTime;		//�A�j���[�V�����̌o�ߎ���(�b)

	enum animState { //�^�C�g����ʂ̃A�j���[�V�������
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
