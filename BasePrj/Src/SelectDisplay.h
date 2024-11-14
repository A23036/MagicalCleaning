#pragma once
#include "Object3D.h"

/// <summary>
/// 画面に各種情報を表示する処理
/// </summary>
class SelectDisplay : public Object3D
{
public:
	SelectDisplay();
	~SelectDisplay();

	void Update() override;
	void Draw() override;
private:
	CSprite* sprite;
	CSpriteImage* titleBackImage;	//タイトル画面背景イメージ
};