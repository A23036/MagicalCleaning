#pragma once
#include "Object3D.h"

// ---------------------------------------------------------------------------
// SkyBoxˆ—
// ---------------------------------------------------------------------------
class Sky : public Object3D {
public:
	Sky();
	virtual ~Sky();
	void Update() override;
	void Draw() override;
private:
	float rotate; //‰ñ“]—p•Ï”
};