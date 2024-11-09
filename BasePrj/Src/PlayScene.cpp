#include "PlayScene.h"
#include "Stage.h"
#include "Camera.h"
#include "DisplayInfo.h"
#include "Dust.h"

PlayScene::PlayScene()
{
	new Stage(5);
	Instantiate<Camera>();
	Instantiate<DisplayInfo>();
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_T)) {
		SceneManager::ChangeScene("TitleScene");
	}
}

void PlayScene::Draw()
{
	GameDevice()->m_pFont->Draw(700, 15, _T("PLAY SCENE"), 16, RGB(255, 0, 0));	
}