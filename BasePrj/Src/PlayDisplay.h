#pragma once
#include "Object3D.h"

/// <summary>
/// プレイヤーごとの各種情報を表示する処理
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