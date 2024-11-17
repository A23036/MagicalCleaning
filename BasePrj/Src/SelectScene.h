#pragma once
#include "SceneBase.h"
#include "Animator.h"
#include "CsvReader.h"
#include "Player.h"
#include "SelectDisplay.h"

class SelectScene : public SceneBase
{
public:
	SelectScene();
	~SelectScene();
	void Update() override;
	void Draw() override;

	void CsvLoad(); //CSV�t�@�C������f�[�^�̐ݒ�
private:
	Player* p0;
	Player* p1;
	Player* p2;
	Player* p3;
	Player* p4;
	Player* p5;

	SelectDisplay* sd;
	
	CsvReader* csv;
	
	CSprite* sprite;
	CSpriteImage* selectBackImage;	//�Z���N�g��ʔw�i�C���[�W
	float ScrollSpeed;				//�w�i�X�N���[�����x
	float offX;						//�w�i�摜�ʒuX
};
