#pragma once
#include "Object3D.h"

// ---------------------------------------------------------------------------
// SkyBox����
// ---------------------------------------------------------------------------
class Sky : public Object3D {
public:
	Sky();
	virtual ~Sky();
	void Update() override;
	void Draw() override;
private:
	float rotate; //��]�p�ϐ�
};