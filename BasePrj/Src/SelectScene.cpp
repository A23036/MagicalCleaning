#include "SelectScene.h"
#include "SceneBase.h"
#include "SelectDisplay.h"
#include "Player.h"

SelectScene::SelectScene()
{
	sprite = new CSprite();
	selectBackImage = new CSpriteImage(_T("data/Image/Select/BackImage.png"));
	selectBackImageR = new CSpriteImage(_T("data/Image/Select/BackImageR.png"));
	Player* p0 = new Player(VECTOR3(0, 0, 0),	VECTOR3(0, 180 * DegToRad, 0), 0);
	Player* p1 = new Player(VECTOR3(2, 0, 0),	VECTOR3(0, 180 * DegToRad, 0), 1);
	Player* p2 = new Player(VECTOR3(-2, 0, 0),	VECTOR3(0, 180 * DegToRad, 0), 2);
	Player* p3 = new Player(VECTOR3(0, -2, 0),	VECTOR3(0, 180 * DegToRad, 0), 3);
	Player* p4 = new Player(VECTOR3(2, -2, 0),	VECTOR3(0, 180 * DegToRad, 0), 4);
	Player* p5 = new Player(VECTOR3(-2, -2, 0), VECTOR3(0, 180 * DegToRad, 0), 5);
	new SelectDisplay();
	scrollSpeed = 1;
	offX = 0;
}

SelectScene::~SelectScene()
{
}

void SelectScene::Update()
{
	auto di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_TRG, DIK_P) || di->CheckJoy(KD_TRG, DIJ_B)) {
		SceneManager::ChangeScene("ViewMapScene");
	}
}

void SelectScene::Draw()
{
	// 背景スクロール処理
	sprite->Draw(selectBackImage, offX, 0, 0, 0, WINDOW_WIDTH - offX, WINDOW_HEIGHT, WINDOW_WIDTH - offX, WINDOW_HEIGHT);
	sprite->Draw(selectBackImageR, 0, 0, offX, 0, -offX, WINDOW_HEIGHT, offX, WINDOW_HEIGHT);

	GameDevice()->m_pFont->Draw(400, 20, _T("SELECT SCENE"), 32, RGB(255, 0, 0));

	offX += scrollSpeed;
	if (offX >= WINDOW_WIDTH) {
		offX = 0;
	}

	GameDevice()->m_mView = XMMatrixLookAtLH(
		VECTOR3(0, 0, -10), // カメラ位置
		VECTOR3(0, 0, 0), // 注視点
		VECTOR3(0, 1, 0)); // 上ベクトル
}
