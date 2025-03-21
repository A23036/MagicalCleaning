#include "SelectScene.h"
#include "SceneBase.h"
#include "Player.h"
#include "SelectDisplay.h"
#include "CsvReader.h"

// ---------------------------------------------------------------------------
// �R���X�g���N�^
// ---------------------------------------------------------------------------
SelectScene::SelectScene()
{
	//�ϐ��̏�����
	Init();

	//�X�v���C�g�C���[�W�̃��[�h
	selectBackImage = new CSpriteImage(_T("data/Image/Select/BackImage.png"));
	
	sprite = new CSprite();
	sd = new SelectDisplay();
}

SelectScene::~SelectScene()
{
	SAFE_DELETE(sprite);
	SAFE_DELETE(selectBackImage);
	SAFE_DELETE(csv);
}

void SelectScene::Init()
{
	//�萔��CSV�ǂݍ���
	CsvLoad();

	for (int i = 0; i < 6; i++) {
		players[i] = new Player(VECTOR3(i * 0.5f - 1.2f, -3, 0), VECTOR3(0, 180 * DegToRad, 0), i);
		initPos[i] = VECTOR3(i * 0.5f - 1.2f, -3, 0);
		players[i]->SetScale(0.6f, 0.6f, 0.6f);
	}
	frm = 0;
	BackFrm = 120;
	scrollX = 0.0f;
}

void SelectScene::Update()
{
	auto di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_TRG, DIK_T)) {
		GameDevice()->selectBGM->Stop();
		SceneManager::ChangeScene("TitleScene");
	}
	if (di->CheckKey(KD_TRG, DIK_P)) {
		GameDevice()->selectBGM->Stop();
		GameDevice()->playBGM->Play();
		SceneManager::ChangeScene("PlayScene");
	}

	if (di->CheckJoy(KD_DAT, 1, 0)) {
		if (frm > BackFrm) { //1P�̈�莞�Ԉȏ�߂�{�^�����͂Ń^�C�g���ɖ߂�
			SceneManager::ChangeScene("TitleScene");
			GameDevice()->selectBGM->Stop();
		}
		frm++;
	}
	else {
		frm = 0;
	}
	
	for (int i = 0; i < MAXPLAYER; i++) {
		if (sd->GetIsSelectColor(i)){ //�v���C���[���J���[��I�����Ă�����
			// �v���C���[�̈ʒu�ɃL�����N�^�[���ړ�
			players[sd->GetSelectColor(i)]->SetPosition(VECTOR3(-3.7f + i % 2 * 7.4f, 1.2f + i / 2 * (-3), 0));
			// �T�C�Y���g��
			players[sd->GetSelectColor(i)]->SetScale(1.4f, 1.4f, 1.4f);
		}
		else {
			if (!sd->GetIsSelected(sd->GetSelectColor(i))) { //�I������Ă��Ȃ��J���[�����̃v���C���[�ɂ��I������Ă��Ȃ�������
				// �����ʒu�ɖ߂�
				players[sd->GetSelectColor(i)]->SetPosition(initPos[sd->GetSelectColor(i)]);
				// �T�C�Y��߂�
				players[sd->GetSelectColor(i)]->SetScale(0.6f, 0.6f, 0.6f);
			}
		}

		if (sd->GetIsReady()) {
			GameDevice()->selectBGM->Stop();
			GameDevice()->playBGM->Play();
			SceneManager::ChangeScene("PlayScene");
		}
	}	
}

void SelectScene::Draw()
{
	// �w�i�X�N���[������
	sprite->Draw(selectBackImage, int(-scrollX), 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	sprite->Draw(selectBackImage, WINDOW_WIDTH - int(scrollX), 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	//GameDevice()->m_pFont->Draw(400, 20, _T("SELECT SCENE"), 32, RGB(255, 0, 0));

	scrollX += ScrollSpeed;
	
	// �X�N���[���������l��
	if (ScrollSpeed > 0 && scrollX >= WINDOW_WIDTH) {
		scrollX = 0;
	}
	else if (ScrollSpeed < 0 && scrollX <= 0) {
		scrollX = WINDOW_WIDTH;
	}

	if (frm != 0) {
		float rate = (float)frm / (float)BackFrm;
		float width = 150 * rate;
		sprite->DrawRect(20, 90, 150, 20, RGB(120,120,120));
		sprite->DrawRect(20, 90, width, 20, RGB(200, 20, 20));
	}
	

	GameDevice()->m_mView = XMMatrixLookAtLH(
		VECTOR3(0, 0, -10), // �J�����ʒu
		VECTOR3(0, 0, 0), // �����_
		VECTOR3(0, 1, 0)); // ��x�N�g��
}

void SelectScene::CsvLoad()
{
	// csv����f�[�^�ǂݍ���

	CsvSelectDataLoader dataLoader("data/csv/AnimParam.csv");
	dataLoader.Load();
	selectParams = dataLoader.GetSelectParams();

	//�ǂݍ��񂾃f�[�^�̑��
	ScrollSpeed = selectParams.ScrollSpeed;
	BackFrm = selectParams.BackFrm;

	/*
	csv = new CsvReader("data/csv/AnimParam.csv");
	if (csv->GetLines() < 1) {
		MessageBox(NULL, "AnimParam.csv���ǂ߂܂���", "�G���[", MB_OK);
	}

	for (int i = 1; i < csv->GetLines(); i++) { //CSV�t�@�C������ݒ�̓ǂݍ���
		if (csv->GetString(i, 0) == "Select") {
			
			if (csv->GetString(i, 1) == "ScrollSpeed") {	// �w�i�X�N���[���X�s�[�h
				ScrollSpeed = csv->GetFloat(i, 3);
			}
		}
	}
	*/
}
