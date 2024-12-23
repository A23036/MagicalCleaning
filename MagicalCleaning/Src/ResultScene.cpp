#include "ResultScene.h"

ResultScene::ResultScene()
{
	dc = ObjectManager::FindGameObject<DataCarrier>();
	rd = new ResultDisplay();

	sprite = new CSprite();
	resultBackImage = new CSpriteImage(_T("data/Image/Result/BackImage.png"));
}

ResultScene::~ResultScene()
{
	SAFE_DELETE(sprite);
	SAFE_DELETE(resultBackImage);
}

void ResultScene::Update()
{
	auto di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_TRG, DIK_T) || di->CheckJoy(KD_TRG, DIJ_B)) {
		SceneManager::ChangeScene("TitleScene");
	}
}

void ResultScene::Draw()
{
	sprite->Draw(resultBackImage, 0, 0, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);

	GameDevice()->m_pFont->Draw(400, 15, _T("RESULTSCENE   Push [T]: TitleScene"), 50, RGB(255, 0, 0));

}
