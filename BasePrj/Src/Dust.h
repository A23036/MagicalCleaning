#pragma once
#include "Object3D.h"
#include "Player.h"

class Dust : public Object3D {
public:
	Dust(int number,VECTOR3 pos);
	~Dust();
	void Update();
	void Draw() override;

	SphereCollider Collider(int n);

	int GetNum() { return dustNum; };

	void AddDamage(Player* player,int damage);

private:
	int maxHp;
	int hp;
	int dustNum;

	enum DustID {
		Small = 0,
		Midium,
		Big,
	};
};