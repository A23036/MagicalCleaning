#include "SelectScene.h"
#include "SceneBase.h"
#include "SelectDisplay.h"
#include "Player.h"

SelectScene::SelectScene()
{
	CsvLoad();
	sprite = new CSprite();
	selectBackImage = new CSpriteImage(_T("data/Image/Select/BackImage.png"));
	Player* p0 = new Player(VECTOR3(0, 0, 0),	VECTOR3(0, 180 * DegToRad, 0), 0);
	Player* p1 = new Player(VECTOR3(2, 0, 0),	VECTOR3(0, 180 * DegToRad, 0), 1);
	Player* p2 = new Player(VECTOR3(-2, 0, 0),	VECTOR3(0, 180 * DegToRad, 0), 2);
	Player* p3 = new Player(VECTOR3(0, -2, 0),	VECTOR3(0, 180 * DegToRad, 0), 3);
	Player* p4 = new Player(VECTOR3(2, -2, 0),	VECTOR3(0, 180 * DegToRad, 0), 4);
	Player* p5 = new Player(VECTOR3(-2, -2, 0), VECTOR3(0, 180 * DegToRad, 0), 5);
	new SelectDisplay();
	offX = 0.0f;
}

SelectScene::~SelectScene()
{
}

void SelectScene::Update()
{
	auto di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_TRG, DIK_T)) {
		SceneManager::ChangeScene("TitleScene");
	}
	if (di->CheckKey(KD_TRG, DIK_P) || di->CheckJoy(KD_TRG, DIJ_B)) {
		SceneManager::ChangeScene("ViewMapScene");
	}
}

void SelectScene::Draw()
{
	// �w�i�X�N���[������
	sprite->Draw(selectBackImage, int(-offX), 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	sprite->Draw(selectBackImage, WINDOW_WIDTH - int(offX), 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	GameDevice()->m_pFont->Draw(400, 20, _T("SELECT SCENE"), 32, RGB(255, 0, 0));

	offX += ScrollSpeed;
	
	// �X�N���[���������l��
	if (ScrollSpeed > 0 && offX >= WINDOW_WIDTH) {
		offX = 0;
	}
	else if (ScrollSpeed < 0 && offX <= 0) {
		offX = WINDOW_WIDTH;
	}

	GameDevice()->m_mView = XMMatrixLookAtLH(
		VECTOR3(0, 0, -10), // �J�����ʒu
		VECTOR3(0, 0, 0), // �����_
		VECTOR3(0, 1, 0)); // ��x�N�g��
}

void SelectScene::CsvLoad()
{
	// csv����f�[�^�ǂݍ���
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
}
