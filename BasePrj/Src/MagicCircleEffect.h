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

	bool isSpawn;	// �o�����t���O
	bool isFading;	// �t�F�[�h�����ǂ����̃t���O
	bool isFinish;	// �I���t���O

	float SpawnTime;	// �o������
	float FadeTime;		// ���Ŏ���
	int RotStart;
	int RotGoal;
	float ScaleStart;
	float ScaleGoal;
};