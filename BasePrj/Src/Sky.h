#pragma once
#include "Object3D.h"

class Sky : public Object3D {
public:
	Sky();
	~Sky();
	void Update() override;
	void Draw() override;
private:
	float rotate;
};