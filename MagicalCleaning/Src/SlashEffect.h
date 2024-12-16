#pragma once
#include "Object3D.h"

class SlashEffect : public Object3D {
public:
	SlashEffect(VECTOR3 pos,float scale);
	~SlashEffect();
	void Update();

private:
	float lifeTime;
	float fadeTime;
	float curTime;
	float rotSpeed;
	int frm;

	bool isFading; // フェード中かどうかのフラグ
	float fadeProgress; // フェードの進捗
};