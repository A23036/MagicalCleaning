#pragma once
#include "Object3D.h"

class JumpEffect : public Object3D {
public:
	JumpEffect(VECTOR3 pos);
	~JumpEffect();
	void Update();

private:
	float lifeTime;
	float fadeTime;
	float curTime;
	int frm;

	float fadeProgress; // フェードの進捗
};