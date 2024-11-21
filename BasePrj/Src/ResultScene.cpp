#include "ResultScene.h"
#include "DataCarrier.h"

ResultScene::ResultScene()
{
	dc = ObjectManager::FindGameObject<DataCarrier>();
}

ResultScene::~ResultScene()
{
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
	GameDevice()->m_pFont->Draw(400, 15, _T("RESULTSCENE"), 16, RGB(255, 0, 0));

	switch (dc->GetWinnerId()) {
	case 0:
		GameDevice()->m_pFont->Draw(400, 150, _T("Winner : player1"), 50, RGB(255, 255, 255));
		break;
	case 1:
		GameDevice()->m_pFont->Draw(400, 150, _T("Winner : player2"), 50, RGB(255, 255, 255));
		break;
	case 2:
		GameDevice()->m_pFont->Draw(400, 150, _T("Winner : player3"), 50, RGB(255, 255, 255));
		break;
	case 3:
		GameDevice()->m_pFont->Draw(400, 150, _T("Winner : player4"), 50, RGB(255, 255, 255));
		break;
	case -1:
		GameDevice()->m_pFont->Draw(400, 150, _T("Draw"), 50, RGB(255, 255, 255));
		break;
	}
	GameDevice()->m_pFont->Draw(400, 300, _T("Push [T]: TitleScene"), 50, RGB(255, 255, 255));
}
