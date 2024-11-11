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

	float timeRate;		//�A�j���[�V�����p
	float rate;

	float alpha;
	float rot;
	float scale;
	float trans;
	
	int animNum;		//�A�j���[�V�����ʒu
	int animFrm;		//�A�j���[�V�����̌o�ߎ���(�t���[��)
	float animTime;		//�A�j���[�V�����̌o�ߎ���(�b)

	enum animState {
		aWait = 0,
		aBroom,
		aText1,
		aText2,
		aFinish,
	};
};
