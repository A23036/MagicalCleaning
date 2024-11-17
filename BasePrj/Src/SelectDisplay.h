#pragma once
#include "Object3D.h"

/// <summary>
/// ��ʂɊe�����\�����鏈��
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
	CSpriteImage* selectUiImage;	//�Z���N�g���UI�C���[�W
	
	int ColorPosY;			//�L�����I���J���[�ʒuY
	int ColorIconSize;		//�L�����I���J���[�T�C�Y
	int ColorIconDispSize;	//�L�����I���J���[�\���T�C�Y
	int UiSpace;			//UI���m�̊Ԋu
	int GuideUiPosY;		//������@�K�C�hUI�ʒuY
	VECTOR2 BackUiPos;		//�߂�UI�ʒu

	bool playerEntry[4];	//�v���C���[�̃G���g���[���
	
	enum Dir{
		UP = 0,
		RIGHT,
		DOWN,
		LEFT,
	};
};