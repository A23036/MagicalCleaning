#pragma once
#include "Particle.h"

class LeafEffect : public Particle {
public:
	LeafEffect(VECTOR3 pos, VECTOR3 scale, int num);
	~LeafEffect();
	void Update() override;

	bool GetIsFinish() { return isFinish; };
private:
	struct ChipControl {
		VECTOR3 velocity; //�ړ����x
	};
	std::list < ChipControl> controls;
	int leafNum;	//�����t���ϖ���
	float lifeTime;
	int frm;
	bool isFinish;	//�A�j���[�V�����I��
};