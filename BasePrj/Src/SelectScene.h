#pragma once
#include "SceneBase.h"
#include "Animator.h"

class SelectScene : public SceneBase
{
public:
	SelectScene();
	~SelectScene();
	void Update() override;
	void Draw() override;
private:
	CSprite* sprite;
	CSpriteImage* selectBackImage;	//セレクト画面背景イメージ
	CSpriteImage* selectBackImageR;	//セレクト画面背景反転イメージ
	int scrollSpeed;				//背景スクロール速度
	int offX;						//背景画像位置X
};
