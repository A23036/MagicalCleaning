#pragma once
// Player.h
#include "Object3D.h"
#include "CsvReader.h"
#include "Animator.h"
#include "DataCarrier.h"
#include "Stage.h"
#include "Camera.h"
#include "MagicCircleEffect.h"

using namespace std;

enum State { //�v���C���[���
	sStandby = 0,
	sOnGround,
	sJump,
	sAttack1,
	sAttack2,
	sAttack3,
	sCharge,
	sBlow,
	sStop,
};

enum SelectPowerID { //�I�𒆋����\��
	pMS = 0,
	pJN,
	pAS,
	pAR,
	pCW,
};

class Broom : public Object3D {
public:
	Broom(Object3D* parentModel, int num);
	~Broom();
	void Update() override;
	void Draw() override;
	void SetPos(MATRIX4X4 m);
private:
	Object3D* parent;
	MATRIX4X4 mat;
};

class Player : public Object3D {
public:
	Player();
	Player(VECTOR3 pos, VECTOR3 rot, int num);
	Player(int num);
	~Player();
	void Start() override;
	void Update() override;
	void Draw() override;

	void CsvLoad(); //CSV�t�@�C������f�[�^�̐ݒ�

	SphereCollider Collider() override;

	int GetLeaf() { return leaf; };
	int GetMP() { return mp; };
	int GetWeight() { return weight; };
	int GetCarWeight() { return carWeight; };
	int GetScore() { return score; };
	int GetPlayerNum() { return playerNum; };
	int GetPlayerState() { return state; };
	int GetSelectPower() { return selectPower; };
	int GetPowerCost(int selectPower);
	int GetPowerLv(int selectPower);
	int GetMaxPowerLv(int selectPower);
	bool GetIsInvisible() { return isInvisible; };
	bool GetIsDash() { return isDash; };
	bool GetIsFly() { return isFly; };
	void SetPlayerState(int state);
	void SetPlayerPrevState(int state);
	void SetBlowVec(VECTOR3 vec);
	void SetSpeedY(float y);
	void SetIsBlow(bool isBlow);
	void ResetLeaf();
	void AddLeaf(int n);
	void AddMP(int n);
	void AddWeight(int n);
	void AddScore(int n);

private:
	CsvReader* csv;
	DataCarrier* dc;
	Stage* st;
	Camera* cm;
	list<Player*> otherPlayers;
	MagicCircleEffect* mcEffect;

	void UpdateOnGround();	//�n�㏈��
	void UpdateJump();		//�󒆏���
	void UpdateAttack1();	//�U��1������
	void UpdateAttack2();	//�U��2������
	void UpdateAttack3();	//�U��3������
	void UpdateCharge();	//MP�ϊ�������
	void UpdateBlow();		//������΂��ꒆ����

	float deltaTime;
	float GRAVITY;		//�d�͉����x
	float JUMP_POWER;	//�W�����v��
	float MOVE_SPEED;	//��{�ړ����x

	int playerNum;		//�v���C���[�ԍ�
	int score;			//�X�R�A
	int leaf;			//�t���Ϗ�����
	int mp;				//MP������
	int weight;			//�^���d��
	int jumpCount;		//���݃W�����v��
	bool isDash;		//�_�b�V��������
	bool isFly;			//��s������
	bool isBlow;		//������ђ�����
	bool canFly;		//��s�\����
	bool atkComboFlg;	//�A���U������
	int	 comboWaitFrm;	//�R���{��s���̓t���[��
	bool isMagicReady;	//MP�ϊ��\�����슮���t���O
	float chargeSpeed;	//MP�ϊ��X�s�[�h
	int chargeFrm;		//MP�ϊ��o�߃t���[��
	VECTOR3 blowVec;	//������΂��x�N�g��
	bool isInvisible;	//�����t���O
	int InvisibleTime;	//����������
	int invisibleTime;	//�������o�ߎ���

	bool finishAtkAnim;

	float	moveSpeed;	//�ړ����x
	int		jumpNum;	//�W�����v��
	float	atkSpeed;	//�U�����x
	float	atkRange;	//�U���͈�
	int		carWeight;	//�^���\�d��

	int msNum;
	int jnNum;
	int asNum;
	int arNum;
	int cwNum;

	static const int MsTableNum = 10;	//�ړ����x�e�[�u����
	static const int JnTableNum = 10;	//�W�����v�񐔃e�[�u����
	static const int AsTableNum = 10;	//�U�����x�e�[�u����
	static const int ArTableNum = 10;	//�U���͈̓e�[�u����
	static const int CwTableNum = 10;	//�^���\�d�ʃe�[�u����

	float MoveSpeedT	[MsTableNum];	//�ړ����x�e�[�u��
	int	  JumpNumT		[JnTableNum];	//�W�����v�񐔃e�[�u��
	float AtkSpeedT		[AsTableNum];	//�U�����x�e�[�u��
	float AtkRangeT		[ArTableNum];	//�U���͈̓e�[�u��
	int   CarWeightT	[CwTableNum];	//�^���\�d�ʃe�[�u��
	
	int MoveSpeedC[MsTableNum];	//�ړ����x�R�X�g�e�[�u��
	int	JumpNumC[JnTableNum];	//�W�����v�񐔃R�X�g�e�[�u��
	int AtkSpeedC[AsTableNum];	//�U�����x�R�X�g�e�[�u��
	int AtkRangeC[ArTableNum];	//�U���͈̓R�X�g�e�[�u��
	int CarWeightC[CwTableNum];	//�^���\�d�ʃR�X�g�e�[�u��

	float speedY; // Y�����̑��x

	Animator* animator; // ���i�̃C���X�^���X���|�C���^�[�^�ŏ���
	enum AnimID {
		aIdle = 0,
		aRun = 1,
		aJump,
		aJump2,
		aFly,
		aFall,
		aAttack1,
		aAttack2,
		aAttack3,
		aChargeReady,
		aCharge,
		aBlow,
	};

	int state;		//�v���C���[���
	int prevState;	//���O�̃v���C���[���

	VECTOR3 posOld;	//���O�̃v���C���[�ʒu

	int selectPower; //�I�𒆔\��

	int anmFrame; // �A�j���[�V�����̃t���[���𐔂���

	Broom* child; //���g�̎����Ă���ⴂ̃|�C���^
};


