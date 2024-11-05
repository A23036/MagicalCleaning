#pragma once
// Player.h
#include "Object3D.h"
#include "Animator.h"

class Broom : public Object3D {
public:
	Broom(Object3D* parentModel);
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
	~Player();
	void Update() override;
	void Draw() override;

	SphereCollider Collider() override;

	int getMP() { return mp; };
	int getWeight() { return weight; };
	int getPlayerNum() { return playerNum; };
	int getPlayerState() { return state; };
	void addMP(int n);
	void addWeight(int n);
	void setPlayerNum(int n) { playerNum = n; };

private:
	int playerNum;
	int mp;
	int weight;


	bool doneAtkAnim;

	int moveSpeed;	//�ړ����x
	int	jumpNum;	//�W�����v��
	int atkSpeed;	//�U�����x
	int atkRange;	//�U���͈�
	int carWeight;	//�^���\�d��

	float speedY; // Y�̑��x

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

	void UpdateOnGround();
	void UpdateJump();
	void UpdateAttack1();
	void UpdateAttack2();
	void UpdateAttack3();

	int anmFrame; // �A�j���[�V�����̃t���[���𐔂���

	Broom* child;
};


