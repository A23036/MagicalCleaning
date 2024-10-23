#include "EditScene.h"
#include "StageEdit.h"

EditScene::EditScene()
{
	new StageEdit();
}

EditScene::~EditScene()
{
}

void EditScene::Update()
{
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_T)) {
		SceneManager::ChangeScene("TitleScene");
	}
}

void EditScene::Draw()
{
	GameDevice()->m_pFont->Draw(400, 15, _T("EDIT SCENE"), 16, RGB(255, 0, 0));
}
