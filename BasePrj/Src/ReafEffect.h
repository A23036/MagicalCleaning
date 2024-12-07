#pragma once
#include "Particle.h"

class ReafEffect : public Particle {
public:
	ReafEffect(VECTOR3 pos, VECTOR3 scale);
	~ReafEffect();
	void Update() override;
private:
	struct ChipControl {
		VECTOR3 velocity; //ˆÚ“®‘¬“x
	};
	std::list < ChipControl> controls;
	float lifeTime;
	int frm;
};