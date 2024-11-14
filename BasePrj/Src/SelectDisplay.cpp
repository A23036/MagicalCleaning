#include "SelectDisplay.h"

SelectDisplay::SelectDisplay()
{
	sprite = new CSprite();
	titleBackImage = new CSpriteImage(_T("data/Image/title/title.png"));
	SetDrawOrder(-10000);	// ˆê”ÔÅŒã‚É•`‰æ‚·‚é
}

SelectDisplay::~SelectDisplay()
{
}

void SelectDisplay::Update()
{
}

void SelectDisplay::Draw()
{
	//‘O–ÊUI‚Ì•`‰æ
	//sprite->Draw(titleBackImage, 300, 300, 100, 100, 300, 300, 300, 300);
}
