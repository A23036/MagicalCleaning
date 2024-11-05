#include "SelectScene.h"
#include "SceneBase.h"

SelectScene::SelectScene()
{
}

SelectScene::~SelectScene()
{
}

void SelectScene::Update()
{
	auto di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_TRG, DIK_P) || di->CheckJoy(KD_TRG, DIJ_B)) {
		SceneManager::ChangeScene("PlayScene");
	}
}

void SelectScene::Draw()
{
	GameDevice()->m_pFont->Draw(400, 15, _T("SELECT SCENE"), 16, RGB(255, 0, 0));
}
