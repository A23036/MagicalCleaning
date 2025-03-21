#include "TitleScene.h"
#include "GameMain.h"
#include "EasingCalc.h"
#include "DataCarrier.h"

// ---------------------------------------------------------------------------
// �R���X�g���N�^
// ---------------------------------------------------------------------------
TitleScene::TitleScene()
{
	//�ϐ��̏�����
	Init();

	//�X�v���C�g�C���[�W�̃��[�h
	titleBackImage	= new CSpriteImage(_T("data/Image/Title/title.png"));
	broomImage		= new CSpriteImage(_T("data/Image/Title/Broom.png"));
	titleText1Image = new CSpriteImage(_T("data/Image/Title/Text1.png"));
	titleText2Image = new CSpriteImage(_T("data/Image/Title/Text2.png"));
	titleText3Image = new CSpriteImage(_T("data/Image/Title/Text3.png"));
	
	sprite = new CSprite();
	ec = new EasingCalc();
	dc = SingleInstantiate<DataCarrier>(); //DataCarrier�̓V���O���g���Ő���(�V�[���J�ڂŏ����Ȃ�����)
}

// ---------------------------------------------------------------------------
// �f�X�g���N�^
// ---------------------------------------------------------------------------
TitleScene::~TitleScene()
{
	SAFE_DELETE(titleBackImage);
	SAFE_DELETE(broomImage);
	SAFE_DELETE(titleText1Image);
	SAFE_DELETE(titleText2Image);
	SAFE_DELETE(titleText3Image);
	SAFE_DELETE(sprite);
	SAFE_DELETE(ec);
}

// ---------------------------------------------------------------------------
// �e�ϐ��̏���������
// ---------------------------------------------------------------------------
void TitleScene::Init()
{
	//�萔��CSV�ǂݍ���
	CsvLoad();

	state = aFadeIn;
	animFrm = 0;
	animTime = 0.0f;

	timeRate = 0.0f;
	rate = 0.0f;

	alpha = 0.0f;
	rot = 0.0f;
	scale = 0.0f;
	trans = 0.0f;

	moveY = 0.0f;
	moveScale = 0.0f;
	offX = 0.0f;
	offY = 0.0f;
}

// ---------------------------------------------------------------------------
// �^�C�g����ʂ̍X�V����
// 
// �L�[/�{�^�����͂ł̃V�[���J�ڂ��s��
// ---------------------------------------------------------------------------
void TitleScene::Update()
{
	auto di = GameDevice()->m_pDI;

	if (state == aWait) {
		// 2024.10.27 �R�[�h�̉ǐ�����ƃR���g���[���[���͑Ή�
		if ((di->CheckKey(KD_TRG, DIK_S) || di->CheckJoy(KD_TRG, 2)) && state != aTransition){
			state = aTransition;
			GameDevice()->gameStartSE->Play();
			animFrm = 0;
		}
		if (di->CheckKey(KD_TRG, DIK_P)) {
			GameDevice()->titleBGM->Stop();
			GameDevice()->playBGM->Play();
			SceneManager::ChangeScene("PlayScene");
		}
	}
	else if ((di->CheckKey(KD_TRG, DIK_S) || di->CheckKey(KD_TRG, DIK_P) || di->CheckJoy(KD_TRG, 2)) && state != aTransition){
		state = aWait;
		GameDevice()->titleBGM->Play(AUDIO_LOOP);
	}

	if (state == aFinish){
		GameDevice()->titleBGM->Stop();
		GameDevice()->selectBGM->Play(AUDIO_LOOP);
		SceneManager::ChangeScene("SelectScene");
	}
}

// ---------------------------------------------------------------------------
// �^�C�g����ʂ̕`�揈��
// 
// �^�C�g����ʂł̃X�v���C�g�`��A�A�j���[�V�����������s��
// ---------------------------------------------------------------------------
void TitleScene::Draw()
{
	//�A�j���[�V�������Ԃ̃t���[��->�b�ϊ�
	animTime = animFrm * (1.0f / 60.0f);

	//�A�j���[�V������Ԃ��Ƃ̉�ʕ`��
	switch (state) {
	case aFadeIn:
		DrawFadeIn();
		break;
	case aBroom:
		DrawBroom();
		break;
	case aText1:
		DrawText1();
		break;
	case aText2:
		DrawText2();
		break;
	case aWait:
		DrawWait();
		break;
	case aTransition:
		DrawTransition();
		break;
	}

	animFrm++; //�A�j���[�V�����t���[�������Z
}

// ---------------------------------------------------------------------------
// �w�i�t�F�[�h�C������
// ---------------------------------------------------------------------------
void TitleScene::DrawFadeIn()
{
	timeRate = animTime / AnimTime[aFadeIn];
	rate = timeRate; //���`�⊮

	alpha = (AlphaGoal - AlphaStart) * rate + AlphaStart;

	sprite->Draw(titleBackImage, 0, 0, 0, 0, 1536, 864, WINDOW_WIDTH, WINDOW_HEIGHT, alpha);
	if (animTime >= AnimTime[aFadeIn]) {
		state = aBroom;
		GameDevice()->swingSE->Play();
		animFrm = 0;
	}
}

// ---------------------------------------------------------------------------
// ⴃX�v���C�g�̃A�j���[�V�����\������
// ---------------------------------------------------------------------------
void TitleScene::DrawBroom()
{
	sprite->Draw(titleBackImage, 0, 0, 0, 0, 1536, 864, WINDOW_WIDTH, WINDOW_HEIGHT);

	timeRate = animTime / AnimTime[aBroom];
	rate = ec->easeOutBack(timeRate);
	rot = (RotateGoal - RotateStart) * rate + RotateStart;
	scale = (ScaleGoal - ScaleStart) * rate + ScaleStart;

	//�X�v���C�g�̕\���p���[���h�s����쐬
	sprite->SetImage(broomImage);
	MATRIX4X4 m = XMMatrixRotationZ(rot) * XMMatrixScaling(scale, scale, 1) * XMMatrixTranslation(0, 0, 0);

	//���[���h���W�ɉ������X�v���C�g�̕\��
	sprite->Draw(m);

	if (animTime >= AnimTime[aBroom])
	{
		GameDevice()->downSE->Play();
		state = aText1;
		animFrm = 0;
	}
}

// ---------------------------------------------------------------------------
// �e�L�X�g�u�܂ق������́v�A�j���[�V�����\������
// ---------------------------------------------------------------------------
void TitleScene::DrawText1()
{
	sprite->Draw(titleBackImage, 0, 0, 0, 0, 1536, 864, WINDOW_WIDTH, WINDOW_HEIGHT);
	sprite->Draw(broomImage, 0, 0, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);

	timeRate = animTime / AnimTime[aText1];
	rate = ec->easeOutBack(timeRate);
	trans = (TransGoal - TransStart) * rate + TransStart;

	sprite->Draw(titleText1Image, 0, trans, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);

	if (animTime >= AnimTime[aText1])
	{
		state = aText2;
		animFrm = 0;
	}
}

// ---------------------------------------------------------------------------
// �e�L�X�g�u������������v�A�j���[�V�����\������
// ---------------------------------------------------------------------------
void TitleScene::DrawText2()
{
	sprite->Draw(titleBackImage, 0, 0, 0, 0, 1536, 864, WINDOW_WIDTH, WINDOW_HEIGHT);
	sprite->Draw(broomImage, 0, 0, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);
	sprite->Draw(titleText1Image, 0, 0, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);

	timeRate = animTime / AnimTime[aText2];
	rate = ec->easeOutBounce(timeRate);
	scale = (ScaleGoal - ScaleStart) * rate + ScaleStart;
	trans = 1 - scale;

	if (animTime == 0.3f) {
		GameDevice()->scaleUpSE->Play();
	}

	sprite->Draw(titleText2Image, WINDOW_WIDTH / 2 * trans, WINDOW_HEIGHT / 2 * trans,
		0, 0, 1280, 720, WINDOW_WIDTH * scale, WINDOW_HEIGHT * scale);
	if (animTime >= AnimTime[aText2])
	{
		state = aWait;
		GameDevice()->titleBGM->Play(AUDIO_LOOP);
		animFrm = 0;
	}
}

// ---------------------------------------------------------------------------
// �A�j���[�V�����I���A�v���C���[�̓��͑ҋ@���`�揈��
// ---------------------------------------------------------------------------
void TitleScene::DrawWait()
{
	sprite->Draw(titleBackImage, 0, 0, 0, 0, 1536, 864, WINDOW_WIDTH, WINDOW_HEIGHT);

	moveY = sinf(animFrm / 120.0f * XM_2PI) * 5.0f;
	moveScale = 1 + fabs(sinf(animFrm / 300.0f * XM_2PI) * 0.05f);

	// �����Ŋg�k���邽�߂̃I�t�Z�b�g���v�Z
	offX = (WINDOW_WIDTH * (1 - moveScale)) / 2;
	offY = (WINDOW_HEIGHT * (1 - moveScale)) / 2;

	sprite->Draw(broomImage, 0, moveY, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);
	sprite->Draw(titleText1Image, 0, 0, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);
	sprite->Draw(titleText2Image, offX, offY, 0, 0, 1280, 720, WINDOW_WIDTH * moveScale, WINDOW_HEIGHT * moveScale);

	if (animFrm % 60 < 40) {
		sprite->SetSrc(0, 0, 344, 64);
		sprite->Draw(titleText3Image, WINDOW_WIDTH / 2 - (sprite->GetSrcWidth()) / 2, WINDOW_HEIGHT * 3 / 4,
			0, 0, sprite->GetSrcWidth(), sprite->GetSrcHeight());
	}
}

// ---------------------------------------------------------------------------
// �V�[���J�ڎ��̃g�����W�V��������
// ---------------------------------------------------------------------------
void TitleScene::DrawTransition()
{
	sprite->Draw(titleBackImage, 0, 0, 0, 0, 1536, 864, WINDOW_WIDTH, WINDOW_HEIGHT);
	sprite->Draw(broomImage, 0, moveY, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);
	sprite->Draw(titleText1Image, 0, 0, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);
	sprite->Draw(titleText2Image, offX, offY, 0, 0, 1280, 720, WINDOW_WIDTH * moveScale, WINDOW_HEIGHT * moveScale);

	timeRate = animTime / 1.0f;
	rate = ec->easeOutExpo(timeRate);
	float width = (WINDOW_WIDTH + 10) * rate;

	sprite->DrawRect(0, 0, width, WINDOW_HEIGHT, RGB(244, 196, 255));

	timeRate = (animTime - 0.2f) / 1.0f;
	rate = ec->easeOutExpo(timeRate);
	if (timeRate < 0) {
		rate = 0;
	}
	width = (WINDOW_WIDTH + 10) * rate;
	sprite->DrawRect(0, 0, width, WINDOW_HEIGHT, RGB(195, 157, 204));

	timeRate = (animTime - 0.4f) / 1.0f;
	rate = ec->easeOutExpo(timeRate);
	if (timeRate < 0) {
		rate = 0;
	}
	width = (WINDOW_WIDTH + 10) * rate;
	sprite->DrawRect(0, 0, width, WINDOW_HEIGHT, RGB(0, 0, 0));

	if (animTime == 1.1f) {
		state = aFinish;
	}
}


// ---------------------------------------------------------------------------
// CSV�ǂݍ��ݏ���
// 
// �A�j���[�V���������Ɏg�p����萔��CSV�ǂݍ��݂��s��
// ---------------------------------------------------------------------------
void TitleScene::CsvLoad()
{
	CsvTitleDataLoader dataLoader("data/csv/AnimParam.csv");
	dataLoader.Load();
	titleParams = dataLoader.GetTitleParams();

	//�ǂݍ��񂾍\���̂���f�[�^�̑��
	for (int i = 0; i < 4; i++) {
		AnimTime[i] = titleParams.AnimTime[i];
	}
	ScaleStart = titleParams.ScaleStart;
	ScaleGoal  = titleParams.ScaleGoal;
	RotateStart = titleParams.RotateStart;
	RotateGoal = titleParams.RotateGoal;
	TransStart = titleParams.TransStart;
	TransGoal = titleParams.TransGoal;
	AlphaStart = titleParams.AlphaStart;
	AlphaGoal = titleParams.AlphaGoal;
}
