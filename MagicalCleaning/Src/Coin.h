#pragma once
#include "Object3D.h"

class Coin : public Object3D {
public:
	Coin();
	~Coin();
	void Update();
private:
	bool got;
};