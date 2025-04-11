#pragma once
#include "Object3D.h"
#include "CsvPlayerDataLoader.h"

//�v���g�^�C�v�錾
class CsvReader;
class Broom;
class Animator;
class DataCarrier;
class Stage;
class Camera;
class MagicCircleEffect;
class TeleportCircleEffect;
class PlayDisplay;

using namespace std;

//�v���C���[���
enum State {
	sStandby = 0,	//�ҋ@(�Z���N�g��ʁA���U���g��ʐ�p���)
	sOnGround,		//�n��
	sJump,			//�W�����v
	sAttack1,		//�U��1
	sAttack2,		//�U��2
	sAttack3,		//�U��3
	sCharge,		//�r��
	sBlow,			//������΂��ꒆ
	sTeleport,		//�e���|�[�g��
	sStop,			//�|�[�Y��
	sWait,			//�J�n�ҋ@��
};

//�I�𒆋����\��
enum SelectPowerID {
	pMS = 0,	//�ړ����x
	pJN,		//�W�����v��
	pAS,		//�U�����x
	pAR,		//�U���͈�
	pCW,		//�e���`���[�t
};

// ---------------------------------------------------------------------------
// �v���C���[����
// 
// �Z���N�g��ʁA�v���C��ʁA���U���g��ʂł̃v���C���[�������s���N���X
// ---------------------------------------------------------------------------
class Player : public Object3D {
public:
	Player(VECTOR3 pos, VECTOR3 rot, int num);
	Player(int num, int color);
	virtual ~Player();
	void Start() override;
	void Init();

	void Update() override;
	void Draw() override;

	void CsvLoad(); //CSV�t�@�C������f�[�^�̐ݒ�

	SphereCollider Collider() override;

	//�A�N�Z�X�֐�
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
	VECTOR2 GetTeleportPos() { return VECTOR2(teleportPos.x, teleportPos.z); };
	bool GetIsInvisible() { return isInvisible; };
	float GetCurInvisibleTime() { return  invisibleTime; };
	float GetInvisibleTime();
	bool GetIsDamage() { return isDamageCool; };
	bool GetIsDash() { return isDash; };
	bool GetIsFly() { return isFly; };
	float GetMoveDistance() { return moveDistance; };
	int GetJumpCount() { return jumpCountAll; };
	int GetKnockOutCount() { return knockOutCount; };
	int GetItemCount() { return itemCount; };
	int GetCleanReafCount() { return cleanReafCount; };
	int GetBlowCount() { return blowCount; };
	int GetItemNum() { return itemNum; };
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
	void UseItem(int num);

private:
	//�e��N���X�̃|�C���^
	Animator* animator;
	CsvReader* csv;
	DataCarrier* dc;
	Stage* st;
	Camera* cm;
	list<Player*> otherPlayers;
	MagicCircleEffect* mcEffect;
	TeleportCircleEffect* tpEffect;
	PlayDisplay* pd;

	//���g�̎����Ă����
	Broom* child;

	//��Ԃ��Ƃ̍X�V����
	void UpdateOnGround();	//�n�㏈��
	void UpdateJump();		//�󒆏���
	void UpdateAttack();	//�U��������
	void UpdateCharge();	//MP�ϊ�������
	void UpdateBlow();		//������΂��ꒆ����
	void UpdateTeleport();	//�e���|�[�g����

	void CheckAtkCoillision(); //�U���q�b�g���菈��

	float deltaTime;

	//�v���C���[��b�ϐ�
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
	VECTOR3 teleportPos;//�e���|�[�g�ꏊ
	bool canFly;		//��s�\����
	bool canSpeedAtk;	//�A���U���\����
	int  fastAtkSpeed;	//�A���U���̍U�����x(��f�Ɉ��)
	bool canRangeAtk;	//�͈͍U���\����
	bool canFastCharge; //�����ϊ��\����

	bool atkComboFlg;	//�A���U������
	int	 comboWaitFrm;	//�R���{��s���̓t���[��
	bool isMagicReady;	//MP�ϊ��\�����슮���t���O
	float chargeSpeed;	//MP�ϊ��X�s�[�h
	float chargeTime;	//MP�ϊ��o�ߎ���
	VECTOR3 blowVec;	//������΂��x�N�g��
	int  itemNum;		//�A�C�e���ԍ�(-1:������,0:�X�e���X)
	bool isInvisible;	//�����t���O

	float invisibleTime;//�������o�ߎ���

	int  damageTime;	//���G�o�ߎ���
	bool isDamageCool;	//���G�t���O

	bool finishAtkAnim;
	int  atkNum;		//���݂̍U���̘A����
	
	bool isTeleporting;	//�e���|�[�g������
	int  teleportFrm;	//�e���|�[�g�o�ߎ���

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

	float speedY; // Y�����̑��x

	//�{�[�i�X�v�Z�p�ϐ�
	float moveDistance;	//�ړ�����
	int jumpCountAll;	//�݌v�W�����v��
	int knockOutCount;	//������΂����G�̐�
	int itemCount;		//�A�C�e�����g�p������
	int cleanReafCount;	//���[�t��|��������
	int blowCount;		//������΂��ꂽ��

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

	VECTOR3 prevPos; //���O�̃v���C���[�ʒu

	int selectPower; //�I�𒆔\��

	int anmFrame; // �A�j���[�V�����̃t���[���𐔂���

	//CSV�t�@�C������ǂݍ��񂾃f�[�^���i�[����\����
	PlayerParams playerParams;

	//CSV�t�@�C������ǂݍ��܂��f�[�^
	float InvisibleTime[MAXPLAYER];	//����������
	float DamageCoolTime;		//���G����
	float TeleportTime;			//�e���|�[�g����

	static const int PowerLvNum = 10;	//�\�̓��x���i�K��

	float MoveSpeedT[PowerLvNum];	//�ړ����x�e�[�u��
	int	  JumpNumT[PowerLvNum];		//�W�����v�񐔃e�[�u��
	float AtkSpeedT[PowerLvNum];	//�U�����x�e�[�u��
	float AtkRangeT[PowerLvNum];	//�U���͈̓e�[�u��
	int   CarWeightT[PowerLvNum];	//�^���\�d�ʃe�[�u��

	int MoveSpeedC[PowerLvNum];	//�ړ����x�R�X�g�e�[�u��
	int	JumpNumC[PowerLvNum];	//�W�����v�񐔃R�X�g�e�[�u��
	int AtkSpeedC[PowerLvNum];	//�U�����x�R�X�g�e�[�u��
	int AtkRangeC[PowerLvNum];	//�U���͈̓R�X�g�e�[�u��
	int CarWeightC[PowerLvNum];	//�^���\�d�ʃR�X�g�e�[�u��
};