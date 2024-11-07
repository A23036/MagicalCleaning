#pragma once
#include "SceneBase.h"

class SelectScene : public SceneBase
{
public:
	SelectScene();
	~SelectScene();
	void Update() override;
	void Draw() override;
private:
	CSprite* sprite;
};
