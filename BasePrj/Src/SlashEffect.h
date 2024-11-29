#pragma once
#include "Object3D.h"

class SlashEffect : public Object3D {
public:
	SlashEffect(VECTOR3 pos,float scale);
	~SlashEffect();
	void Update();

private:
	float lifeTime;
	float curTime;
	float rotSpeed;
	int frm;
};