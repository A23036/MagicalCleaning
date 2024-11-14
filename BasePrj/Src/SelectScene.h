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
	CSpriteImage* selectBackImage;	//�Z���N�g��ʔw�i�C���[�W
	CSpriteImage* selectBackImageR;	//�Z���N�g��ʔw�i���]�C���[�W
	int scrollSpeed;				//�w�i�X�N���[�����x
	int offX;						//�w�i�摜�ʒuX
};
