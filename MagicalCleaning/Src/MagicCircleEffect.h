#pragma once
#include "Object3D.h"
#include "EasingCalc.h"

class MagicCircleEffect : public Object3D {
public:
	MagicCircleEffect(VECTOR3 pos,int num);
	~MagicCircleEffect();
	void Update() override;

	void UpdateSpawn();
	void UpdateRotate();
	void UpdateFinish();
	void SetIsFinish();

private:
	EasingCalc* ec;

	VECTOR4 color;

	float curTime;
	float rotSpeed;
	float rotY;
	int frm;

	bool isSpawn;	// 出現中フラグ
	bool isFading;	// フェード中かどうかのフラグ
	bool isFinish;	// 終了フラグ

	float SpawnTime;	// 出現時間
	float FadeTime;		// 消滅時間
	int RotStart;
	int RotGoal;
	float ScaleStart;
	float ScaleGoal;
};