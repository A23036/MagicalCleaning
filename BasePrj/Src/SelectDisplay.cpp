#include "SelectDisplay.h"

SelectDisplay::SelectDisplay()
{
	sprite = new CSprite();
	titleBackImage = new CSpriteImage(_T("data/Image/title/title.png"));
	SetDrawOrder(-10000);	// ��ԍŌ�ɕ`�悷��
}

SelectDisplay::~SelectDisplay()
{
}

void SelectDisplay::Update()
{
}

void SelectDisplay::Draw()
{
	//�O��UI�̕`��
	//sprite->Draw(titleBackImage, 300, 300, 100, 100, 300, 300, 300, 300);
}
