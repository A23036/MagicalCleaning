#pragma once
#include "Object3D.h"
#include "EasingCalc.h"

class TeleportCircleEffect : public Object3D {
public:
	TeleportCircleEffect(VECTOR3 pos, int num);
	~TeleportCircleEffect();
	void Update() override;

	void UpdateSpawn();
	void UpdateRotate();
	void SetIsFinish();

private:
	EasingCalc* ec;

	VECTOR4 color;

	float curTime;
	float rotSpeed;
	float rotY;
	int frm;

	bool isSpawn;	// 出現中フラグ

	float SpawnTime;	// 出現時間
	int RotStart;
	int RotGoal;
	float ScaleStart;
	float ScaleGoal;
};