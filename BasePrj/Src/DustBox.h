#pragma once
#include "Object3D.h"

class DustBox : public Object3D {
public:
	DustBox();
	~DustBox();
	void Update();

	SphereCollider Collider();

private:
};