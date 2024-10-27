#include "TitleScene.h"
#include "GameMain.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Update()
{
	// 2024.10.27 コードの可読性向上とコントローラー入力対応
	auto di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_TRG, DIK_P) || di->CheckJoy(KD_TRG,DIJ_B)) {
		SceneManager::ChangeScene("PlayScene");
	}
	if (di->CheckKey(KD_TRG, DIK_E)) {
		SceneManager::ChangeScene("EditScene");
	}
}

void TitleScene::Draw()
{
	GameDevice()->m_pFont->Draw(400, 15, _T("TITLE SCENE"), 16, RGB(255, 0, 0));
}