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
	Player* players[6]; //�v���C���[�̃��X�g
	SelectDisplay* sd;
	CsvReader* csv;
	CSprite* sprite;
	CSpriteImage* selectBackImage;	//�Z���N�g��ʔw�i�C���[�W

	VECTOR3 initPos[6];		//�L�����N�^�[�����ʒu
	float ScrollSpeed;		//�w�i�X�N���[�����x
	float scrollX;			//�w�i�摜�ʒuX
};
