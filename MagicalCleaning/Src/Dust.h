#pragma once
#include "Object3D.h"
#include "Player.h"
#include "Stage.h"
#include "LeafEffect.h"

class Dust : public Object3D {
public:
	Dust(int number,VECTOR3 pos);
	~Dust();
	void Start() override;
	void Update();
	void Draw() override;

	SphereCollider Collider(int n);

	int GetNum() { return dustNum; };

	void AddDamage(Player* player,int damage);

private:
	Stage* st;

	VECTOR3 posOld;

	int maxHp;
	int hp;
	int dustNum;
	float size;

	float SmallSize;
	float MidiumSize;
	float BigSize;
	float MaxScale;

	enum DustID {
		Small = 0,
		Midium,
		Big,
		Item,
	};
};