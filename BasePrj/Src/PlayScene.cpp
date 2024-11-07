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
	new Dust(0,VECTOR3(5,5,-5));
	new Dust(1, VECTOR3(10, 5, -5));
	new Dust(2, VECTOR3(15, 5, -5));
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