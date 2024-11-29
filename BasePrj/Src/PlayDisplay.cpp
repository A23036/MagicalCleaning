#include "PlayDisplay.h"
#include "PlayScene.h"
#include "Player.h"
#include "DataCarrier.h"

PlayDisplay::PlayDisplay()
{
	SetPriority(-9999); 	// 最後より一つ前に処理する
	SetDrawOrder(-10000);	// 一番最後に描画する
}

PlayDisplay::~PlayDisplay()
{
}

void PlayDisplay::Update()
{
}

void PlayDisplay::Draw()
{
	//UI表示
	std::string s = "Player";
	int n = ObjectManager::DrawCounter();
	s = s + std::to_string(n+1);

	Player* pl = ObjectManager::FindGameObjectWithTag<Player>(s);
	char str[64]; //文字列を用意
	sprintf_s<64>(str, "MP:%3d", pl->GetMP());
	GameDevice()->m_pFont->Draw(WINDOW_WIDTH * 1/4 , WINDOW_HEIGHT - 120, str, 50, RGB(255, 0,0));

	switch (pl->GetSelectPower()){ //選択中パワー表記
	case pMS:
		sprintf_s<64>(str, "MoveSpeed");
		break;
	case pJN:
		sprintf_s<64>(str, "JumpNum");
		break;
	case pAS:
		sprintf_s<64>(str, "AtkSpeed");
		break;
	case pAR:
		sprintf_s<64>(str, "AtkRange");
		break;
	case pCW:
		sprintf_s<64>(str, "CarWeight");
		break;
	}
	GameDevice()->m_pFont->Draw(WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT - 120, str, 50, RGB(255, 0,0));

	GameDevice()->m_pFont->Draw(WINDOW_WIDTH - 400, WINDOW_HEIGHT - 120, _T("COST"), 40, RGB(255, 255, 255));
	sprintf_s<64>(str, "%3d", pl->GetPowerCost(pl->GetSelectPower()));

	if (pl->GetPowerCost(pl->GetSelectPower()) <= pl->GetMP()) { //mpが足りていたらコストを白色表示
		GameDevice()->m_pFont->Draw(WINDOW_WIDTH - 380, WINDOW_HEIGHT - 80, str, 40, RGB(255, 255, 255));
	}
	else{
		GameDevice()->m_pFont->Draw(WINDOW_WIDTH - 380, WINDOW_HEIGHT - 80, str, 40, RGB(255, 0, 0));
	}
	
	//スコア
	sprintf_s<64>(str, "Score:%3d", pl->GetScore() );
	GameDevice()->m_pFont->Draw(WINDOW_WIDTH / 2 - 70, 30, str, 50, RGB(255, 255, 0));

}
