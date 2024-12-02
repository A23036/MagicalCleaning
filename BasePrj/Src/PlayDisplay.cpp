#include "PlayDisplay.h"
#include "PlayScene.h"
#include "Player.h"
#include "DataCarrier.h"
#include "EasingCalc.h"

PlayDisplay::PlayDisplay()
{
	SetPriority(-9999); 	// �Ō����O�ɏ�������
	SetDrawOrder(-10000);	// ��ԍŌ�ɕ`�悷��

	countDownImage = new CSpriteImage(_T("data/Image/Play/CountDown.png"));

	sprite = new CSprite();
	
	animFrm = 0;
	animTime = 0;
	
	rotStart = 0;
	rotGoal = 360 * DegToRad;
	ScaleStart = 0.0f;
	ScaleGoal = 3.0f;
}

PlayDisplay::~PlayDisplay()
{
	SAFE_DELETE(countDownImage);
	SAFE_DELETE(sprite);
}

void PlayDisplay::Start()
{
	dc = ObjectManager::FindGameObject<DataCarrier>();
	ec = ObjectManager::FindGameObject<EasingCalc>();
}

void PlayDisplay::Update()
{
	animTime = (1.0f / 60.0f) * animFrm;
	if (animTime >= 1.0f) {
		animFrm = 0;
	}
	gameState = dc->GetGameState();
	gameTime = dc->GetGameTime();

	animFrm++;
}

void PlayDisplay::Draw()
{
	
	if (gameState == sReady) {
		if (gameTime == 0) {
			sprite->SetSrc(countDownImage, 64, 0, 32, 32);
		}
		if (gameTime == 1) {
			sprite->SetSrc(countDownImage, 32, 0, 32, 32);
		}
		if (gameTime == 2) {
			sprite->SetSrc(countDownImage, 0, 0, 32, 32);
		}
		if (gameTime == 3) {
			sprite->SetSrc(countDownImage, 96, 0, 91, 32);
		}
		int width, height;
		VECTOR2 center;
		width = sprite->GetSrcWidth();
		height = sprite->GetSrcHeight();
		center.x = WINDOW_WIDTH / 2 - width / 2;
		center.y = WINDOW_HEIGHT / 2 - height / 2;

		// �A�j���[�V�����̐i�s�x���v�Z
		float timeRate = animTime / 1.0f;
		float rate = ec->easeOutBack(timeRate); // �C�[�W���O�Ŋ��炩�Ȋg��

		// ��]�p�ƃX�P�[�����v�Z
		float rotation = (rotGoal + rotStart) * rate - rotStart;	// ��]
		float scale = (ScaleGoal + ScaleStart) * rate - ScaleStart;	// �g��{��

		// ��]���S���摜�̒��S�ɂ��邽�߂̕␳
		float pivotX = width / 2.0f;  // �摜�̕��̔���
		float pivotY = height / 2.0f; // �摜�̍����̔���

		// ���[���h�s��̌v�Z
		MATRIX4X4 m = XMMatrixTranslation(-pivotX, -pivotY, 0) 
			* XMMatrixRotationZ(rotation)
			* XMMatrixScaling(scale, scale, 1.0f)
			* XMMatrixTranslation(center.x, center.y, 0);

		// �X�v���C�g�����[���h�s����g�p���ĕ`��
		sprite->Draw(m);
	}


	//UI�\��
	std::string s = "Player";
	int n = ObjectManager::DrawCounter();
	s = s + std::to_string(n+1);

	Player* pl = ObjectManager::FindGameObjectWithTag<Player>(s);
	char str[64]; //�������p��
	sprintf_s<64>(str, "MP:%3d", pl->GetMP());
	GameDevice()->m_pFont->Draw(WINDOW_WIDTH * 1/4 , WINDOW_HEIGHT - 120, str, 50, RGB(255, 0,0));

	switch (pl->GetSelectPower()){ //�I�𒆃p���[�\�L
	case pMS:
		sprintf_s<64>(str, "MoveSpeed");
		break;
	case pJN:
		sprintf_s<64>(str, "JumpNum");
		break;
	case pAS:
		sprintf_s<64>(str, "AtkSpeed");
		break;
	case pAR:
		sprintf_s<64>(str, "AtkRange");
		break;
	case pCW:
		sprintf_s<64>(str, "CarWeight");
		break;
	}
	GameDevice()->m_pFont->Draw(WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT - 120, str, 50, RGB(255, 0,0));

	GameDevice()->m_pFont->Draw(WINDOW_WIDTH - 400, WINDOW_HEIGHT - 120, _T("COST"), 40, RGB(255, 255, 255));
	sprintf_s<64>(str, "%3d", pl->GetPowerCost(pl->GetSelectPower()));

	if (pl->GetPowerCost(pl->GetSelectPower()) <= pl->GetMP()) { //mp������Ă�����R�X�g�𔒐F�\��
		GameDevice()->m_pFont->Draw(WINDOW_WIDTH - 380, WINDOW_HEIGHT - 80, str, 40, RGB(255, 255, 255));
	}
	else{
		GameDevice()->m_pFont->Draw(WINDOW_WIDTH - 380, WINDOW_HEIGHT - 80, str, 40, RGB(255, 0, 0));
	}
	
	//�X�R�A
	sprintf_s<64>(str, "Score:%3d", pl->GetScore() );
	GameDevice()->m_pFont->Draw(WINDOW_WIDTH / 2 - 70, 30, str, 50, RGB(255, 255, 0));

}
