#pragma once
#include "Object3D.h"

// ---------------------------------------------------------------------------
// SkyBox処理
// ---------------------------------------------------------------------------
class Sky : public Object3D {
public:
	Sky();
	virtual ~Sky();
	void Update() override;
	void Draw() override;
private:
	float rotate; //回転用変数
};