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
	CSpriteImage* countDownImage;	//�J�E���g�_�E���C���[�W
	CSpriteImage* playUiImage;		//�v���C���UI�C���[�W
	CSpriteImage* playUiImage2;		//�v���C���UI�C���[�W2

	DataCarrier* dc;
	EasingCalc* ec;

	int gameState;
	int gameTime;

	bool isLastSpurt;
	bool isPlaySound[4];

	enum GameState {
		sTransition = 0,
		sReady,	//�J�n�O�J�E���g�_�E��
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

	int msIconPosX[MAXPLAYER];
	int jnIconPosX[MAXPLAYER];
	int asIconPosX[MAXPLAYER];
	int arIconPosX[MAXPLAYER];
	int cwIconPosX[MAXPLAYER];

	int msTextPosX[MAXPLAYER];
	int jnTextPosX[MAXPLAYER];
	int asTextPosX[MAXPLAYER];
	int arTextPosX[MAXPLAYER];
	int cwTextPosX[MAXPLAYER];
	
	bool isMaxLv[5][MAXPLAYER];
};