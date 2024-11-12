#pragma once
#pragma once
#include "SceneBase.h"

class ViewMapScene : public SceneBase
{
public:
	ViewMapScene();
	~ViewMapScene();
	void Update() override;
	void Draw() override;
};
