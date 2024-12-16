#pragma once
#include "Object3D.h"
#include "StageEdit.h"

class ControlPanel : public Object3D {
public:
	ControlPanel();
	~ControlPanel();
	void Update() override;
	void Draw() override;
private:
	char filename[256];
	IVECTOR3 size;
};