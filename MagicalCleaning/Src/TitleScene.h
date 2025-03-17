#pragma once
#include "SceneBase.h"

//�v���g�^�C�v�錾
class EasingCalc;
class CsvReader;
class DataCarrier;

// ---------------------------------------------------------------------------
// �^�C�g����ʏ���
// ---------------------------------------------------------------------------
class TitleScene : public SceneBase
{
public:
	TitleScene();
	~TitleScene();
	void Init();
	void Update() override;
	void Draw() override;

	void CsvLoad(); //CSV�t�@�C������f�[�^�̐ݒ���s��

private:
	//�e��f�[�^�Ǘ��N���X�̃|�C���^
	CsvReader*	csv;
	EasingCalc*	 ec;
	DataCarrier* dc;
	CSpriteImage*	titleBackImage;		//�^�C�g����ʔw�i�C���[�W
	CSpriteImage*	broomImage;			//�^�C�g����ʁuⴁv�C���[�W
	CSpriteImage*	titleText1Image;	//�u�܂ق������́v�e�L�X�g�C���[�W
	CSpriteImage*	titleText2Image;	//�u������������v�e�L�X�g�C���[�W
	CSpriteImage*	titleText3Image;	//�uPRESS ANYBUTTON�v�e�L�X�g�C���[�W
	CSprite* sprite;

	//CSV�t�@�C�����烍�[�h�����萔
	float AnimTime[4];	//�e�A�j���[�V�����̎���
	float ScaleStart;	//�g�嗦�̊J�n�l
	float ScaleGoal;	//�g�嗦�̏I���l
	float RotateStart;	//��]�̊J�n�l
	float RotateGoal;	//��]�̏I���l
	float TransStart;	//�ʒu�̊J�n�l
	float TransGoal;	//�ʒu�̏I���l
	float AlphaStart;	//�����x�̊J�n�l
	float AlphaGoal;	//�����x�̏I���l

	//�A�j���[�V�������Ƃ̐i�s�x�v�Z�p�ϐ�
	float timeRate;		//�A�j���[�V�����̐i�s�x�i0.0�`1.0�j
	float rate;			//�i�s�x���ԏ����ɓK�p�������ʁi0.0�`1.0�j
	int	  animFrm;		//�A�j���[�V�����̌o�ߎ���(�t���[��)
	float animTime;		//�A�j���[�V�����̌o�ߎ���(�b)

	//�A�j���[�V�����i�s���̒l�������ϐ�
	float alpha;	//�����x
	float rot;		//��]
	float scale;	//�g�k
	float trans;	//�ʒu

	float moveY;		//���͑ҋ@���̃X�v���C�g�̏㉺�^���Ɏg�p
	float moveScale;	//���͑ҋ@���̃X�v���C�g�̊g�k�Ɏg�p
	float offX;
	float offY;

	//�^�C�g����ʂ̃A�j���[�V�������
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
