#include "SelectDisplay.h"

SelectDisplay::SelectDisplay()
{
	SetDrawOrder(-10000);	// ��ԍŌ�ɕ`�悷��
	
	sprite = new CSprite();
	selectUiImage = new CSpriteImage(_T("data/Image/Select/UI.png"));

	ColorPosY = WINDOW_HEIGHT * 2/5;		//�L�����I���J���[�ʒuY
	ColorIconSize = 64;						//�L�����I���J���[�T�C�Y
	ColorIconDispSize = 100;				//�L�����I���J���[�\���T�C�Y
	UiSpace = 50;							//�L�����I���J���[��
	GuideUiPosY = WINDOW_HEIGHT - 100;		//������@�K�C�hUI�ʒuY
	BackUiPos = VECTOR2(20,20);	//�߂�UI�ʒu

	for (int i = 0; i < 4; i++) {
		playerEntry[i] = false;
	}
}

SelectDisplay::~SelectDisplay()
{
	SAFE_DELETE(sprite);
	SAFE_DELETE(selectUiImage);
}

void SelectDisplay::Update()
{
}

void SelectDisplay::Draw()
{
	//�O��UI�̕`��
	sprite->SetImage(selectUiImage);

	//�߂�A�C�R��
	sprite->SetSrc(0, 320, 64, 64);
	sprite->Draw(BackUiPos.x, BackUiPos.y);

	//�߂�{�^��
	sprite->SetSrc(64 * DOWN, 64, 64, 64);
	sprite->Draw(BackUiPos.x + 64 + 16, BackUiPos.y);

	//�L�����N�^�[�J���[
	//��
	sprite->SetSrc(96, 128, ColorIconSize, ColorIconSize, ColorIconDispSize, ColorIconDispSize);
	sprite->Draw(WINDOW_WIDTH / 2 - ColorIconDispSize /2  - UiSpace - ColorIconDispSize, ColorPosY);
	
	//��
	sprite->SetSrc(96 + ColorIconSize, 128, ColorIconSize, ColorIconSize, ColorIconDispSize, ColorIconDispSize);
	sprite->Draw(WINDOW_WIDTH / 2 - ColorIconDispSize /2, ColorPosY);

	//��
	sprite->SetSrc(96 + ColorIconSize * 2, 128, ColorIconSize, ColorIconSize, ColorIconDispSize, ColorIconDispSize);
	sprite->Draw(WINDOW_WIDTH / 2 - ColorIconDispSize /2 + UiSpace + ColorIconDispSize, ColorPosY);

	//��
	sprite->SetSrc(96, 128+ ColorIconSize, ColorIconSize, ColorIconSize, ColorIconDispSize, ColorIconDispSize);
	sprite->Draw(WINDOW_WIDTH / 2 - ColorIconDispSize / 2 - UiSpace - ColorIconDispSize, ColorPosY + UiSpace + ColorIconDispSize);

	//��
	sprite->SetSrc(96 + ColorIconSize, 128 + ColorIconSize, ColorIconSize, ColorIconSize, ColorIconDispSize, ColorIconDispSize);
	sprite->Draw(WINDOW_WIDTH / 2 - ColorIconDispSize / 2, ColorPosY + UiSpace + ColorIconDispSize);

	//��
	sprite->SetSrc(96 + ColorIconSize * 2, 128 + ColorIconSize, ColorIconSize, ColorIconSize, ColorIconDispSize, ColorIconDispSize);
	sprite->Draw(WINDOW_WIDTH / 2 - ColorIconDispSize / 2 + UiSpace + ColorIconDispSize, ColorPosY + UiSpace + ColorIconDispSize);

}
