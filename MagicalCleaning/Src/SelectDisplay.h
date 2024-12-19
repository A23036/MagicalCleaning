#pragma once
#include "Object3D.h"
#include "DataCarrier.h"

/// <summary>
/// ��ʂɊe�����\�����鏈��
/// </summary>

enum Color {
	Red = 0,
	Blue,
	Yellow,
	Green,
	Purple,
	Black,
};

class SelectDisplay : public Object3D
{
public:
	SelectDisplay();
	~SelectDisplay();

	void Update() override;
	void Draw() override;

	void DrawUI();

	bool GetIsSelectColor(int playerNum) { return playerEntry[playerNum]; };
	bool GetIsSelected(int color) { return isSelect[color]; };
	int GetSelectColor(int playerNum) { return selectColor[playerNum]; };
	bool GetIsReady() { return isReadyAll; };
	
private:
	DataCarrier* dc;
	CSprite* sprite;
	CSpriteImage* selectUiImage;	//�Z���N�g���UI�C���[�W
	
	int ColorPosY;			//�L�����I���J���[�ʒuY
	int ColorIconSize;		//�L�����I���J���[�T�C�Y
	int ColorIconDispSize;	//�L�����I���J���[�\���T�C�Y
	int UiSpace;			//UI���m�̊Ԋu
	int GuideUiPosY;		//������@�K�C�hUI�ʒuY
	VECTOR2 BackUiPos;		//�߂�UI�ʒu

	bool isReadyAll;				//�v���C���[�S������������
	bool playerEntry[MAXPLAYER];	//�v���C���[�̃G���g���[���
	bool isReady[MAXPLAYER];		//�v���C���[������������
	int  selectColor[MAXPLAYER];	//�v���C���[���Ƃ̑I�𒆃J���[
	int  selectCamera[MAXPLAYER];	//�v���C���[���Ƃ̑I�𒆃J�����ݒ� 0:�I�[�g/1:�}�j���A��
	bool isSetUpCamera[MAXPLAYER];	//�v���C���[���Ƃ̃J�����ݒ芮������
	bool isSelect[6];				//�J���[���Ƃ̑I���ςݔ���
	VECTOR2 cursorPos[MAXPLAYER];	//�J�[�\���ʒu

	int moveFrm[MAXPLAYER];	//�v���C���[���Ƃ̓��̓t���[���J�E���g�p�ϐ�
	int MoveFrm;			//�J�[�\���������̂ɕK�v�ȓ��̓t���[����
};