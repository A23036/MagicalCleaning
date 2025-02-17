#pragma once
#include "Object3D.h"

class DataCarrier;
class EasingCalc;

/// <summary>
/// �v���C���[���Ƃ̊e�����\�����鏈��
/// </summary>

using namespace std;

class PlayDisplay : public Object3D
{
public:
	PlayDisplay();
	~PlayDisplay();
	void Start() override;
	void Update() override;
	void Draw() override;

	void SetBlowPlayer(int atkPlayerNum,int blowPlayerNum);
	void DrawBlowPlayer(int atkPlayerNum, int blowPlayerNum);

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
	bool isPlaySound[4]; //�J�E���g�_�E��SE���ŏ������Đ����邽�߂̃t���O
	
	int blowPlayerList[MAXPLAYER];	//������΂��ꂽ�v���C���[
	float blowAnimFrm[MAXPLAYER];	//������΂����v���C���[�\���A�j���[�V�����o�ߎ���

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