#include "SelectDisplay.h"

SelectDisplay::SelectDisplay()
{
	SetDrawOrder(-10000);	// ��ԍŌ�ɕ`�悷��
	
	sprite = new CSprite();
	selectUiImage = new CSpriteImage(_T("data/Image/Select/UI.png"));

	ColorPosY = WINDOW_HEIGHT * 2/5;		//�L�����I���J���[�ʒuY
	ColorIconSize = 64;						//�L�����I���J���[�T�C�Y
	ColorIconDispSize = 80;					//�L�����I���J���[�\���T�C�Y
	UiSpace = 50;							//�L�����I���J���[��
	GuideUiPosY = WINDOW_HEIGHT - 100;		//������@�K�C�hUI�ʒuY
	BackUiPos = VECTOR2(20,20);	//�߂�UI�ʒu

	for (int i = 0; i < 4; i++) {
		playerEntry[i] = false;
	}

	//�J���[�I�������l
	selectColor[0] = 0;	//��
	selectColor[1] = 1; //��
	selectColor[2] = 4; //��
	selectColor[3] = 5; //��

}

SelectDisplay::~SelectDisplay()
{
	SAFE_DELETE(sprite);
	SAFE_DELETE(selectUiImage);
}

void SelectDisplay::Update()
{
	//�J�[�\���̈ړ�
	auto di = GameDevice()->m_pDI;

	if(di->CheckJoy(KD_DAT,5))

	//�I���J���[���Ƃ̈ʒu���̐ݒ�
	for (int i = 0; i < MAXPLAYER; i++) {
		switch (selectColor[i]) {
		case Red:
			cursorPos[i] = VECTOR2(WINDOW_WIDTH / 2 - ColorIconDispSize / 2 - UiSpace - ColorIconDispSize, ColorPosY);
			break;
		case Blue:
			cursorPos[i] = VECTOR2(WINDOW_WIDTH / 2 - ColorIconDispSize / 2, ColorPosY);
			break;
		case Yellow:
			cursorPos[i] = VECTOR2(WINDOW_WIDTH / 2 - ColorIconDispSize / 2 + UiSpace + ColorIconDispSize, ColorPosY);
			break;
		case Green:
			cursorPos[i] = VECTOR2(WINDOW_WIDTH / 2 - ColorIconDispSize / 2 - UiSpace - ColorIconDispSize, ColorPosY + UiSpace + ColorIconDispSize);
			break;
		case Purple:
			cursorPos[i] = VECTOR2(WINDOW_WIDTH / 2 - ColorIconDispSize / 2, ColorPosY + UiSpace + ColorIconDispSize);
			break;
		case Black:
			cursorPos[i] = VECTOR2(WINDOW_WIDTH / 2 - ColorIconDispSize / 2 + UiSpace + ColorIconDispSize, ColorPosY + UiSpace + ColorIconDispSize);
			break;
		}
	}
	
}

void SelectDisplay::Draw()
{
	//UI�C���[�W�̐ݒ�
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


	//�R���g���[���[���Ƃ̃J�[�\���̕`��

}
