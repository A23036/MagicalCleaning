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
		VECTOR3 velocity; //移動速度
	};
	std::list < ChipControl> controls;
	int leafNum;	//生成葉っぱ枚数
	float lifeTime;
	int frm;
	bool isFinish;	//アニメーション終了
};