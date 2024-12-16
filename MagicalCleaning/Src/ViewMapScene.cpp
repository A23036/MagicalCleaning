#include "ViewMapScene.h"

ViewMapScene::ViewMapScene()
{
}

ViewMapScene::~ViewMapScene()
{
}

void ViewMapScene::Update()
{
	auto di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_TRG, DIK_P) || di->CheckJoy(KD_TRG, DIJ_B)) {
		SceneManager::ChangeScene("PlayScene");
	}
}

void ViewMapScene::Draw()
{
	GameDevice()->m_pFont->Draw(400, 15, _T("VIEW MAP SCENE"), 16, RGB(255, 0, 0));
}
