#pragma once
#include "Object3D.h"
#include "DataCarrier.h"
#include "EasingCalc.h"

/// <summary>
/// �v���C���[���Ƃ̊e�����\�����鏈��
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
	CSpriteImage* countDownImage;		//�J�E���g�_�E���C���[�W
	DataCarrier* dc;
	EasingCalc* ec;

	int gameState;
	int gameTime;

	enum GameState {
		sReady = 0,	//�J�n�O�J�E���g�_�E��
		sPose,		//�|�[�Y��
		sGamePlay,	//�Q�[���v���C��
		sFinish,	//�Q�[���I�����o��
	};

	int animFrm;		//�A�j���[�V�����̌o�ߎ���(�t���[��)
	float animTime;		//�A�j���[�V�����̌o�ߎ���(�b)

	int rotStart;
	int rotGoal;
	int ScaleStart;
	int ScaleGoal;
};