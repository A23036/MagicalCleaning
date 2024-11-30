#pragma once
#include "Object3D.h"

class PowerUpEffect : public Object3D {
public:
	PowerUpEffect(Object3D* parentModel,VECTOR3 pos);
	~PowerUpEffect();
	void Update();

private:
	float lifeTime;
	float fadeTime;
	float curTime;
	float rotSpeed;
	float upSpeed;
	int frm;

	bool isFading; // フェード中かどうかのフラグ
	float fadeProgress; // フェードの進捗

	Object3D* parent;
};