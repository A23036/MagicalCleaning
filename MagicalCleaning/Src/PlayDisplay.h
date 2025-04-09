#pragma once
#include "Object3D.h"

//�v���g�^�C�v�錾
class DataCarrier;
class EasingCalc;
class Player;

using namespace std;

// ---------------------------------------------------------------------------
// �v���C���UI�\������
// 
// �v���C����UI�`��A�X�v���C�g�A�j���[�V�������s��
// ---------------------------------------------------------------------------
class PlayDisplay : public Object3D
{
public:
	PlayDisplay();
	virtual ~PlayDisplay();
	void Start() override;
	void Init();

	void Update() override;
	void Draw() override;

	//UI�A�j���[�V�����`��
	void DrawAnimationUI();
	void DrawCountDown();
	void DrawLastSpurt();
	void DrawFinishCount();
	void DrawFinish();

	//��{UI�`��
	void DrawBaseUI(Player* pl);

	void SetBlowPlayer(int atkPlayerNum,int blowPlayerNum);
	void DrawBlowPlayer(int atkPlayerNum, int blowPlayerNum);
	
private:
	//�e��N���X�̃|�C���^
	DataCarrier* dc;
	EasingCalc* ec;
	CSprite* sprite;
	CSpriteImage* countDownImage;	//�J�E���g�_�E���C���[�W
	CSpriteImage* playUiImage;		//�v���C���UI�C���[�W
	CSpriteImage* playUiImage2;		//�v���C���UI�C���[�W2

	int gameState;	//�Q�[�����
	int gameTime;	//�Q�[������

	bool isLastSpurt;		//���X�g�X�p�[�g�t���O
	bool isPlaySound[4];	//�J�E���g�_�E��SE���ŏ������Đ����邽�߂̃t���O
	
	int	  blowPlayerList[MAXPLAYER];	//������΂��ꂽ�v���C���[
	float blowAnimFrm[MAXPLAYER];		//������΂����v���C���[�\���A�j���[�V�����o�ߎ���

	int   animFrm;		//�A�j���[�V�����̌o�ߎ���(�t���[��)
	float animTime;		//�A�j���[�V�����̌o�ߎ���(�b)

	//�e�\�̓A�C�R���ʒu
	int msIconPosX[MAXPLAYER];
	int jnIconPosX[MAXPLAYER];
	int asIconPosX[MAXPLAYER];
	int arIconPosX[MAXPLAYER];
	int cwIconPosX[MAXPLAYER];

	//�e�\�͕����ʒu
	int msTextPosX[MAXPLAYER];
	int jnTextPosX[MAXPLAYER];
	int asTextPosX[MAXPLAYER];
	int arTextPosX[MAXPLAYER];
	int cwTextPosX[MAXPLAYER];

	//�e�\�̓}�b�N�X�t���O
	bool isMaxLv[5][MAXPLAYER];

	//�Q�[�����
	enum GameState {
		sTransition = 0,//�g�����W�V������
		sReady,			//�J�n�O�J�E���g�_�E��
		sPose,			//�|�[�Y��
		sGamePlay,		//�Q�[���v���C��
		sFinish,		//�Q�[���I�����o��
	};
};