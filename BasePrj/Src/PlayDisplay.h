#pragma once
#include "Object3D.h"

/// <summary>
/// �v���C���[���Ƃ̊e�����\�����鏈��
/// </summary>
class PlayDisplay : public Object3D
{
public:
	PlayDisplay();
	~PlayDisplay();

	void Update() override;
	void Draw() override;

private:

	int gameState;
	int gameTime;
};