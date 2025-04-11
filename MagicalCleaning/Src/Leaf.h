#pragma once
#include "Object3D.h"

//�v���g�^�C�v�錾
class Player;
class Stage;
class LeafEffect;

// ---------------------------------------------------------------------------
// ���[�t����
// 
// �v���C��ʂł̃��[�t�������s���N���X
// ---------------------------------------------------------------------------
class Leaf : public Object3D {
public:
	Leaf(int number,VECTOR3 pos);
	virtual ~Leaf();
	void Start() override;
	void Update();
	void Draw() override;

	SphereCollider Collider(int n);

	//�A�N�Z�X�֐�
	int GetNum() { return leafNum; };

	void AddDamage(Player* player,int damage);

private:
	Stage* st;

	VECTOR3 prevPos; //�O�t���[���̈ʒu

	int maxHp;		//�ő�hp
	int hp;			//����hp
	int leafNum;	//���[�t�̔ԍ�
	float size;		//���[�t�̃T�C�Y

	float maxScale; //�ő�T�C�Y(���������T�C�Y)
	
	//���[�tID
	enum LeafID {
		Small = 0,
		Midium,
		Big,
		Item,
	};
};