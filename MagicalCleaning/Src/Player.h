#pragma once
// Player.h
#include "Object3D.h"
#include "CsvReader.h"
#include "Animator.h"
#include "DataCarrier.h"
#include "Stage.h"
#include "Camera.h"
#include "MagicCircleEffect.h"
#include "TeleportCircleEffect.h"

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
	sTeleport,
	sStop,
	sWait,
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
	Player(int num, int color);
	~Player();
	void Start() override;
	void Update() override;
	void Draw() override;

	void CsvLoad(); //CSV�t�@�C������f�[�^�̐ݒ�

	SphereCollider Collider() override;

	int GetColor() { return color; };
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
	bool GetIsSetTeleport() { return setTeleport; };
	VECTOR2 GetTeleportPos() { return VECTOR2(tereportPos.x, tereportPos.z); };
	bool GetIsInvisible() { return isInvisible; };
	bool GetIsDash() { return isDash; };
	bool GetIsFly() { return isFly; };
	float GetMoveDistance() { return moveDistance; };
	int GetJumpCount() { return jumpCount; };
	int GetKnockOutCount() { return knockOutCount; };
	int GetItemCount() { return itemCount; };
	int GetCleanReafCount() { return cleanReafCount; };
	int GetBlowCount() { return blowCount; };
	void SetPlayerState(int state);
	void SetPlayerPrevState(int state);
	void SetBlowVec(VECTOR3 vec);
	void SetSpeedY(float y);
	void SetIsBlow();
	void ResetLeaf();
	void AddLeaf(int n);
	void AddMP(int n);
	void AddWeight(int n);
	void AddScore(int n);
	void AddCleanReaf();

private:
	CsvReader* csv;
	DataCarrier* dc;
	Stage* st;
	Camera* cm;
	list<Player*> otherPlayers;
	MagicCircleEffect* mcEffect;
	TeleportCircleEffect* tpEffect;

	void UpdateOnGround();	//�n�㏈��
	void UpdateJump();		//�󒆏���
	void UpdateAttack();	//�U��������
	void UpdateCharge();	//MP�ϊ�������
	void UpdateBlow();		//������΂��ꒆ����
	void UpdateTeleport();	//�e���|�[�g����

	float deltaTime;
	float GRAVITY;		//�d�͉����x
	float JUMP_POWER;	//�W�����v��
	float MOVE_SPEED;	//��{�ړ����x

	int playerNum;		//�v���C���[�ԍ�
	int color;			//�v���C���[�J���[
	int score;			//�X�R�A
	int leaf;			//�t���Ϗ�����
	int mp;				//MP������
	int weight;			//�^���d��
	int jumpCount;		//���݃W�����v��
	bool isDash;		//�_�b�V��������
	bool isFly;			//��s������
	bool isBlow;		//������ђ�����

	//�e�\�͍ŏI�i��
	bool canTeleport;	//�e���|�[�g�\����
	bool setTeleport;	//�e���|�[�g�ꏊ�ݒu�ςݔ���
	VECTOR3 tereportPos;//�e���|�[�g�ꏊ
	bool canFly;		//��s�\����
	bool canSpeedAtk;	//�A���U���\����
	int fastAtkSpeed;	//�A���U���̍U�����x(��f�Ɉ��)
	bool canRangeAtk;	//�������U���\����
	bool canFastCharge; //�����ϊ��\����

	bool atkComboFlg;	//�A���U������
	int	 comboWaitFrm;	//�R���{��s���̓t���[��
	bool isMagicReady;	//MP�ϊ��\�����슮���t���O
	float chargeSpeed;	//MP�ϊ��X�s�[�h
	float chargeTime;	//MP�ϊ��o�ߎ���
	VECTOR3 blowVec;	//������΂��x�N�g��
	bool isInvisible;	//�����t���O
	int InvisibleTime;	//����������
	int invisibleTime;	//�������o�ߎ���

	bool finishAtkAnim;
	int atkNum;			//���݂̍U���̘A����
	
	bool isTeleporting;	//�e���|�[�g������
	float TeleportTime;	//�e���|�[�g����
	int teleportFrm;	//�e���|�[�g�o�ߎ���

	float	moveSpeed;	//�ړ����x
	int		jumpNum;	//�W�����v��
	float	atkSpeed;	//�U�����x
	float	atkRange;	//�U���͈�
	int		carWeight;	//�^���\�d��

	int msNum;	//�\�̓��x��:�ړ����x
	int jnNum;	//�\�̓��x��:�W�����v��
	int asNum;	//�\�̓��x��:�U�����x
	int arNum;	//�\�̓��x��:�U���͈�
	int cwNum;	//�\�̓��x��:�^���\�d��

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

	float moveDistance;	//�ړ�����
	int jumpCountAll;	//�݌v�W�����v��
	int knockOutCount;	//������΂����G�̐�
	int itemCount;		//�A�C�e�����g�p������
	int cleanReafCount;	//���[�t��|��������
	int blowCount;		//������΂��ꂽ��

	Animator* animator; // ���i�̃C���X�^���X���|�C���^�[�^�ŏ���
	enum AnimID {
		aStandBy = 0,
		aIdle,
		aRun,
		aJump,
		aJump2,
		aFly,
		aFall,
		aAttack1,
		aAttack2,
		aAttack3,
		aComboAttack,
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


