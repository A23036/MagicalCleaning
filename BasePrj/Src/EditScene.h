#pragma once
// EditScene.h
#include "SceneBase.h"

class EditScene : public SceneBase {
public:
	EditScene();
	~EditScene();
	void Update() override;
	void Draw() override;
};