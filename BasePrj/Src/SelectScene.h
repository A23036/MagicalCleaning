#pragma once
#include "SceneBase.h"
#include "Animator.h"
#include "CsvReader.h"

class SelectScene : public SceneBase
{
public:
	SelectScene();
	~SelectScene();
	void Update() override;
	void Draw() override;

	void CsvLoad(); //CSV�t�@�C������f�[�^�̐ݒ�
private:
	CsvReader* csv;
	CSprite* sprite;
	CSpriteImage* selectBackImage;	//�Z���N�g��ʔw�i�C���[�W
	float ScrollSpeed;				//�w�i�X�N���[�����x
	float offX;						//�w�i�摜�ʒuX
};
