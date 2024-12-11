#pragma once
#include "Object3D.h"

class Sky : public Object3D {
public:
	Sky();
	~Sky();
	void Update();
private:
	float rotate;
};