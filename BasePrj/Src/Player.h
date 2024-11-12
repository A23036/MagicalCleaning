#pragma once
// Player.h
#include "Object3D.h"
#include "CsvReader.h"
#include "Animator.h"

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
	Player(int num);
	~Player();
	void Update() override;
	void Draw() override;

	void CsvLoad(); //CSV�t�@�C������f�[�^�̐ݒ�

	SphereCollider Collider() override;

	int GetMP() { return mp; };
	int GetWeight() { return weight; };
	int GetPlayerNum() { return playerNum; };
	int GetPlayerState() { return state; };
	void AddMP(int n);
	void AddWeight(int n);

private:
	CsvReader* csv;

	float GRAVITY;		//�d�͉����x
	float JUMP_POWER;	//�W�����v��
	float MOVE_SPEED;	//��{�ړ����x

	int playerNum;		//�v���C���[�ԍ�
	int mp;
	int weight;

	bool finishAtkAnim;

	int moveSpeed;	//�ړ����x
	int	jumpNum;	//�W�����v��
	int atkSpeed;	//�U�����x
	int atkRange;	//�U���͈�
	int carWeight;	//�^���\�d��

	/*
	int moveSpeedT	[MAXPLAYER];	//�ړ����x�e�[�u��
	int	jumpNumT	[MAXPLAYER];	//�W�����v��
	int atkSpeedT	[MAXPLAYER];	//�U�����x
	int atkRangeT	[MAXPLAYER];	//�U���͈�
	int carWeightT	[MAXPLAYER];	//�^���\�d��
	*/

	float speedY; // Y�����̑��x

	Animator* animator; // ���i�̃C���X�^���X���|�C���^�[�^�ŏ���
	enum AnimID {
		aIdle = 0,
		aRun = 1,
		aWalk,
		aJump,
		aAttack1,
		aAttack2,
		aAttack3,
	};

	enum State {
		sOnGround = 0,
		sJump,
		sAttack1,
		sAttack2,
		sAttack3,
	};

	State state;

	void UpdateOnGround();	//�n�㏈��
	void UpdateJump();		//�󒆏���
	void UpdateAttack1();	//�U��1������
	void UpdateAttack2();	//�U��2������
	void UpdateAttack3();	//�U��3������

	int anmFrame; // �A�j���[�V�����̃t���[���𐔂���

	Broom* child; //���g�̎����Ă���ⴂ̃|�C���^
};


