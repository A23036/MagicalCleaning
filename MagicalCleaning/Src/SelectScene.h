#pragma once
#include "SceneBase.h"
#include "CsvSelectDataLoader.h"

//�v���g�^�C�v�錾
class Player;
class SelectDisplay;
class CsvReader;

// ---------------------------------------------------------------------------
// �Z���N�g��ʏ���
// ---------------------------------------------------------------------------
class SelectScene : public SceneBase
{
public:
	SelectScene();
	virtual ~SelectScene();
	void Init();
	void Update() override;
	void Draw() override;

	void CsvLoad(); //CSV�t�@�C������ǂݍ��񂾃f�[�^�̐ݒ���s��

private:
	//�e��N���X�̃|�C���^
	Player*			players[6];			//�v���C���[�̃��X�g
	SelectDisplay*	sd;
	CsvReader*		csv;
	CSpriteImage*	selectBackImage;	//�Z���N�g��ʔw�i�C���[�W
	CSprite*		sprite;
	
	//CSV�t�@�C������ǂݍ��񂾃f�[�^���i�[����\����
	SelectParams selectParams;

	//CSV�t�@�C������ǂݍ��܂��f�[�^
	float	ScrollSpeed;	//�w�i�X�N���[�����x
	int		BackFrm;		//�V�[����߂�̂ɕK�v�Ȓ������t���[��

	//�w�i�A�j���[�V�����p�ϐ�
	float	scrollX;		//�w�i�摜�ʒuX
	int		frm;			//�o�߃t���[��

	//�v���C���[�ݒ�
	VECTOR3 initPos[6];		//�L�����N�^�[�����ʒu
};
