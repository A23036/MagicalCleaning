#pragma once
#include "Object3D.h"
#include "Player.h"
#include "Stage.h"
#include "LeafEffect.h"

class Leaf : public Object3D {
public:
	Leaf(int number,VECTOR3 pos);
	~Leaf();
	void Start() override;
	void Update();
	void Draw() override;

	SphereCollider Collider(int n);

	int GetNum() { return leafNum; };

	void AddDamage(Player* player,int damage);

private:
	Stage* st;

	VECTOR3 posOld;

	int maxHp;
	int hp;
	int leafNum;
	float size;

	float SmallSize;
	float MidiumSize;
	float BigSize;
	float MaxScale;

	enum LeafID {
		Small = 0,
		Midium,
		Big,
		Item,
	};
};