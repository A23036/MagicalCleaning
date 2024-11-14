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
	CSpriteImage* titleBackImage;	//�^�C�g����ʔw�i�C���[�W
};