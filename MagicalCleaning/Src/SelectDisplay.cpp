#include "SelectDisplay.h"
#include "EasingCalc.h"

SelectDisplay::SelectDisplay()
{
	SetDrawOrder(-10000);	// ��ԍŌ�ɕ`�悷��
	
	dc = ObjectManager::FindGameObject<DataCarrier>();
	ec = ObjectManager::FindGameObject<EasingCalc>();

	sprite = new CSprite();
	selectUiImage = new CSpriteImage(_T("data/Image/Select/UI.png"));

	ColorPosY = WINDOW_HEIGHT * 2/5;		//�L�����I���J���[�ʒuY
	ColorIconSize = 64;						//�L�����I���J���[�T�C�Y
	ColorIconDispSize = 100;				//�L�����I���J���[�\���T�C�Y
	UiSpace = 50;							//�L�����I���J���[��
	GuideUiPosY = WINDOW_HEIGHT - 100;		//������@�K�C�hUI�ʒuY
	BackUiPos = VECTOR2(20,20);	//�߂�UI�ʒu

	isReadyAll = false;

	for (int i = 0; i < 4; i++) {
		playerEntry[i] = false;
		isSetUpCamera[i] = false;
		isReady[i] = false;
		selectCamera[i] = 0;
		moveFrm[i] = 15;
		animFrm[i] = 0;
	}

	for (int i = 0; i < 6; i++) {
		isSelect[i] = false;
	}

	MoveFrm = 10;

	//�J���[�I�������l
	selectColor[0] = 0;
	selectColor[1] = 0;
	selectColor[2] = 0;
	selectColor[3] = 0;
}

SelectDisplay::~SelectDisplay()
{
	SAFE_DELETE(sprite);
	SAFE_DELETE(selectUiImage);
}

void SelectDisplay::Update()
{
	auto di = GameDevice()->m_pDI;

	for (int i = 0; i < MAXPLAYER; i++) {
		// �R���g���[���[�X�e�B�b�N�̓��͂��擾�A���K��
		float ix = di->GetJoyState(i).lX / 1000.0f; // ���E�̓��� -1.0~1.0
		float iy = di->GetJoyState(i).lY / 1000.0f; // �㉺�̓��� -1.0~1.0

		//�J�[�\���ړ��p�t���[�����Z
		moveFrm[i]++;

		if (isReady[i]) { //��������
			//���������L�����Z������
			if (di->CheckJoy(KD_TRG, 1, i)) {
				isReady[i] = false;
				animFrm[i] = 0;
			}
			continue;
		}

		if (isSetUpCamera[i]) { //�J�����ݒ��
			//������������
			if (di->CheckJoy(KD_TRG, 2, i)) {
				isReady[i] = true;
			}
			//�J�����ݒ�L�����Z������
			if (di->CheckJoy(KD_TRG, 1, i)) {
				isSetUpCamera[i] = false;
			}
			continue;
		}


		if (playerEntry[i]) { //�J���[�I����
			// �`�F�b�N�}�[�N�������̈ړ�
			if (moveFrm[i] >= MoveFrm) {
				if (ix > 0.8f || ix < -0.8f) { // ���E����
					if (selectCamera[i] == 0) {
						selectCamera[i] = 1;
					}
					else {
						selectCamera[i] = 0;
					}
					moveFrm[i] = 0;
				}
			}
			//�J�����ݒ茈�菈��
			if (di->CheckJoy(KD_TRG, 2, i)) {
				isSetUpCamera[i] = true;
			}
			//�J���[����L�����Z������
			if (di->CheckJoy(KD_TRG, 1, i)) {
				isSelect[selectColor[i]] = false;	//�J���[�𖢑I����
				playerEntry[i] = false;				//�v���C���[��I�𖢊�����
			}
			continue;
		}

		//�J���[�̑I��
		// �������̈ړ�
		if (moveFrm[i] >= MoveFrm) {
			if (ix > 0.8f) { // �E��������
				if (selectColor[i] % 3 == 2) { // �E�[�̏ꍇ
					selectColor[i] -= 2;      // ���[�Ƀ��[�v
				}
				else {
					selectColor[i]++;         // �E�Ɉړ�
				}
				moveFrm[i] = 0;
			}
			else if (ix < -0.8f) { // ����������
				if (selectColor[i] % 3 == 0) { // ���[�̏ꍇ
					selectColor[i] += 2;      // �E�[�Ƀ��[�v
				}
				else {
					selectColor[i]--;         // ���Ɉړ�
				}
				moveFrm[i] = 0;
			}
		}

		// �c�����̈ړ�
		if (moveFrm[i] >= MoveFrm) {
			if (iy > 0.8f) { // ����������
				if (selectColor[i] / 3 == 1) { // ���[�̏ꍇ
					selectColor[i] -= 3;      // ��[�Ƀ��[�v
				}
				else {
					selectColor[i] += 3;      // ���Ɉړ�
				}
				moveFrm[i] = 0;
			}
			else if (iy < -0.8f) { // ���������
				if (selectColor[i] / 3 == 0) { // ��[�̏ꍇ
					selectColor[i] += 3;      // ���[�Ƀ��[�v
				}
				else {
					selectColor[i] -= 3;      // ��Ɉړ�
				}
				moveFrm[i] = 0;
			}
		}

		//�J���[���菈��
		if (di->CheckJoy(KD_TRG, 2, i)) {
			if (isSelect[selectColor[i]]) { //���肵���J���[�����łɑI������Ă�����
				//���񂽂�����Ă����I
			}
			else {
				isSelect[selectColor[i]] = true; //�J���[��I���ς݂�
				playerEntry[i] = true;		//�v���C���[��I��������
			}
		}

		//�I���J���[���Ƃ̈ʒu���̐ݒ�
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
	
	int num = 0; //���������l��
	for (int i = 0; i < MAXPLAYER; i++) {
		if (isReady[i]) {
			animFrm[i]++;
			if (animFrm[i] * (1.0f / 60.0f) > 1.0f) { //���������A�j���[�V�������I�����Ă���
				num++;
			}
		}
	}

	if (num == MAXPLAYER) { //�S������������
		//DataCarrier�ɐݒ����ۑ�
		dc->SetCameraSetteing(selectCamera);
		dc->SetColor(selectColor);

		isReadyAll = true;
	}
	else {
		isReadyAll = false;
	}
}

void SelectDisplay::Draw()
{
	//UI�C���[�W�̐ݒ�
	sprite->SetImage(selectUiImage);

	//�߂�A�C�R��
	sprite->SetSrc(1, 330, 64, 64);
	sprite->Draw(BackUiPos.x, BackUiPos.y);
	//�߂�{�^��
	sprite->SetSrc(133, 67, 64, 64);
	sprite->Draw(BackUiPos.x + 64 + 16, BackUiPos.y);

	//�J�[�\������K�C�hUI
	//L�X�e�B�b�N�A�C�R��
	sprite->SetSrc(272, 48, 132, 80);
	sprite->Draw(500, 100);
	//����{�^��
	sprite->SetSrc(67, 67, 64, 64);
	sprite->Draw(642, 106);
	//�uOK�vUI
	sprite->SetSrc(1, 34, 92, 32);
	sprite->Draw(570, 180);
	//�߂�{�^��
	sprite->SetSrc(133, 67, 64, 64);
	sprite->Draw(750, 106);
	//�uBACK�vUI
	sprite->SetSrc(96, 34, 140, 32);
	sprite->Draw(712, 180);

	//�L�����N�^�[�J���[
	//��
	sprite->SetSrc(1, 132, ColorIconSize, ColorIconSize, ColorIconDispSize, ColorIconDispSize);
	sprite->Draw(WINDOW_WIDTH / 2 - ColorIconDispSize / 2 - UiSpace - ColorIconDispSize, ColorPosY);

	//��
	sprite->SetSrc(67, 132, ColorIconSize, ColorIconSize, ColorIconDispSize, ColorIconDispSize);
	sprite->Draw(WINDOW_WIDTH / 2 - ColorIconDispSize / 2, ColorPosY);

	//��
	sprite->SetSrc(133, 132, ColorIconSize, ColorIconSize, ColorIconDispSize, ColorIconDispSize);
	sprite->Draw(WINDOW_WIDTH / 2 - ColorIconDispSize / 2 + UiSpace + ColorIconDispSize, ColorPosY);

	//��
	sprite->SetSrc(1, 197, ColorIconSize, ColorIconSize, ColorIconDispSize, ColorIconDispSize);
	sprite->Draw(WINDOW_WIDTH / 2 - ColorIconDispSize / 2 - UiSpace - ColorIconDispSize, ColorPosY + UiSpace + ColorIconDispSize);

	//��
	sprite->SetSrc(67, 197, ColorIconSize, ColorIconSize, ColorIconDispSize, ColorIconDispSize);
	sprite->Draw(WINDOW_WIDTH / 2 - ColorIconDispSize / 2, ColorPosY + UiSpace + ColorIconDispSize);

	//��
	sprite->SetSrc(133, 197, ColorIconSize, ColorIconSize, ColorIconDispSize, ColorIconDispSize);
	sprite->Draw(WINDOW_WIDTH / 2 - ColorIconDispSize / 2 + UiSpace + ColorIconDispSize, ColorPosY + UiSpace + ColorIconDispSize);

	//�v���C���[�̏�Ԃɉ�����UI�̕`��
	DrawUI();

	for (int i = 0; i < MAXPLAYER; i++) {
		sprite->SetSrc(selectUiImage, 1, 1, 192, 32);
		// �A�j���[�V�����̐i�s�x�v�Z
		float timeRate = animFrm[i]*(1.0f/60.0f) / 1.0f;
		if (timeRate > 1.0f) {
			timeRate = 1.0f;
		}
		float rate = ec->easeOutExpo(timeRate); // ���炩�Ȋg��

		int width, height;
		width = sprite->GetSrcWidth();
		height = sprite->GetSrcHeight();

		// �g�咆�S���摜�̒��S�ɂ��邽�߂̕␳
		float pivotX = width / 2.0f;
		float pivotY = height / 2.0f;

		float posX;
		float posY;

		posX = 260 + i % 2 * 840;
		posY = 250 + i / 2 * 340;

		float scale = 2.0f * rate;	// �g��

		// ���[���h�s��̌v�Z
		MATRIX4X4 m = XMMatrixTranslation(-pivotX, -pivotY, 0)
			* XMMatrixScaling(scale, scale, 1.0f)
			* XMMatrixTranslation(posX, posY, 0);

		sprite->Draw(m);
	}
}

void SelectDisplay::DrawUI()
{
	for (int i = 0; i < MAXPLAYER; i++) {
		int offX = 0, offY = 0;
		int dispOffX = 0, dispOffY = 0;
		int width, height, dispWidth, dispHeight;

		//�J�[�\���T�C�Y�̕���ݒ�
 		switch (i) {
		case 0:
		case 2:
			//1P��3P���I�𒆂̐F��2P��4P���I�����Ă�����
			if (selectColor[1] == selectColor[i] || selectColor[3] == selectColor[i]) {
				width = ColorIconSize / 2; //�J�[�\�����𔼕���
			}
			else {
				width = ColorIconSize; //�J�[�\�����͂��̂܂�
			}
			break;
		case 1:
		case 3:
			//2P��4P���I�𒆂̐F��1P��3P���I�����Ă�����
			if (selectColor[0] == selectColor[i] || selectColor[2] == selectColor[i]) {
				width = ColorIconSize / 2;  //�J�[�\�����𔼕���
				offX = ColorIconSize / 2;	//�J�[�\��UI�̉E�����̈ʒu��ݒ�
				dispOffX = ColorIconDispSize / 2; //�\���ʒu���J�[�\���A�C�R���̔��������E��
			}
			else {
				width = ColorIconSize; //�J�[�\�����͂��̂܂�
			}
			break;
		}

		//�J�[�\���T�C�Y�̍�����ݒ�
		switch (i) {
		case 0:
			//1P���I�𒆂̐F��3P���I�����Ă�����
			if (selectColor[2] == selectColor[i]) {
				height = ColorIconSize / 2; //�J�[�\���T�C�Y�𔼕���
			}
			else {
				height = ColorIconSize; //�J�[�\���T�C�Y�͂��̂܂�
			}
			break;
		case 1:
			//2P���I�𒆂̐F��4P���I�����Ă�����
			if (selectColor[3] == selectColor[i]) {
				height = ColorIconSize / 2; //�J�[�\���T�C�Y�𔼕���
			}
			else {
				height = ColorIconSize; //�J�[�\���T�C�Y�͂��̂܂�
			}
			break;
		case 2:
			//3P���I�𒆂̐F��1P���I�����Ă�����
			if (selectColor[0] == selectColor[i]) {
				height = ColorIconSize / 2; //�J�[�\���T�C�Y�𔼕���
				offY = ColorIconSize / 2;	//�J�[�\��UI�̉������̈ʒu��ݒ�
				dispOffY = ColorIconDispSize / 2; //�\���ʒu���J�[�\���A�C�R���̔�����������
			}
			else {
				height = ColorIconSize; //�J�[�\���T�C�Y�͂��̂܂�
			}
			break;
		case 3:
			//4P���I�𒆂̐F��2P���I�����Ă�����
			if (selectColor[1] == selectColor[i]) {
				height = ColorIconSize / 2; //�J�[�\���T�C�Y�𔼕���
				offY = ColorIconSize / 2;	//�J�[�\��UI�̉������̈ʒu��ݒ�
				dispOffY = ColorIconDispSize / 2; //�\���ʒu���J�[�\���A�C�R���̔�����������
			}
			else {
				height = ColorIconSize; //�J�[�\���T�C�Y�͂��̂܂�
			}
			break;
		}

		//�J�[�\���X�v���C�g�̐ݒ�
		dispWidth = ColorIconDispSize * (float)width / (float)ColorIconSize;
		dispHeight = ColorIconDispSize * (float)height / (float)ColorIconSize;

		sprite->SetSrc(1 + i * 66 + offX, 264 + offY, width, height, dispWidth, dispHeight);
		sprite->Draw(cursorPos[i].x + dispOffX, cursorPos[i].y + dispOffY);	//�J�[�\���ʒu���v�Z���`��


		//�u��P�v�X�v���C�g�̕`��
		sprite->SetSrc(208, 144 + i * 18, 24, 16, 48, 32);
		
		if (playerEntry[i]) { //�J���[�I����
			//�Z���N�g���̎��A�A�C�R���̐^�񒆂ɕ\��
			sprite->Draw(cursorPos[i].x + 28, cursorPos[i].y + 36);

			sprite->SetSrc(80, 336, 444, 80); //�J�����ݒ�A��������UI�\��
			sprite->Draw(20+i%2 * 880, 300 + i/2 * 350); //�v���C���[���ƂɈʒu�w��

			//�`�F�b�N�{�b�N�XUI�\��
			if (isSetUpCamera[i]) { //�J�����ݒ�I���ς�
				sprite->SetSrc(100, 432, 16, 16);
				sprite->Draw(192 + selectCamera[i] * 112 + i % 2 * 880, 312 + i / 2 * 350);

				if (isReady[i]) { //��������
					sprite->SetSrc(100, 432, 16, 16);
					sprite->Draw(192 + i % 2 * 880, 356 + i / 2 * 350);
				}
				else {
					sprite->SetSrc(80, 432, 16, 16);
					sprite->Draw(192 + i % 2 * 880, 356 + i / 2 * 350);
				}
				
			}
			else { //�J�����ݒ�I�𖢑I��
				sprite->SetSrc(80, 432, 16, 16);
				sprite->Draw(192 + selectCamera[i] * 112 + i % 2 * 880, 312 + i / 2 * 350);
			}
		}
		else { //�J���[���I��
			switch (i) {
			case 0:
				sprite->Draw(cursorPos[i].x - 10, cursorPos[i].y - 10);
				break;
			case 1:
				sprite->Draw(cursorPos[i].x + 60, cursorPos[i].y - 10);
				break;
			case 2:
				sprite->Draw(cursorPos[i].x - 10, cursorPos[i].y + 80);
				break;
			case 3:
				sprite->Draw(cursorPos[i].x + 60, cursorPos[i].y + 80);
				break;
			}
		}

	}
}

