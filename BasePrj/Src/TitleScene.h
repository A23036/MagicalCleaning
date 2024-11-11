#pragma once
#include "SceneBase.h"

class TitleScene : public SceneBase
{
public:
	TitleScene();
	~TitleScene();
	void Update() override;
	void Draw() override;
	float easeOutBack(float x);
	float easeOutBounce(float x);

private:
	CSpriteImage* titleBackImage;
	CSpriteImage* broomImage;
	CSpriteImage* titleText1Image;
	CSpriteImage* titleText2Image;
	CSpriteImage* titleText3Image;

	CSprite* sprite;

	float timeRate;		//アニメーション用
	float rate;

	float alpha;
	float rot;
	float scale;
	float trans;
	
	int animNum;		//アニメーション位置
	int animFrm;		//アニメーションの経過時間(フレーム)
	float animTime;		//アニメーションの経過時間(秒)

	enum animState {
		aWait = 0,
		aBroom,
		aText1,
		aText2,
		aFinish,
	};
};
