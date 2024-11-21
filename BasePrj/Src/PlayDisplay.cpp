#include "PlayDisplay.h"
#include "PlayScene.h"
#include "Player.h"
#include "DataCarrier.h"

PlayDisplay::PlayDisplay()
{
	SetPriority(-9999); 	// �Ō����O�ɏ�������
	SetDrawOrder(-10000);	// ��ԍŌ�ɕ`�悷��
}

PlayDisplay::~PlayDisplay()
{
}

void PlayDisplay::Update()
{
}

void PlayDisplay::Draw()
{
	//UI�\��
	std::string s = "Player";
	int n = ObjectManager::DrawCounter();
	s = s + std::to_string(n+1);

	Player* pl = ObjectManager::FindGameObjectWithTag<Player>(s);
	char str[64]; //�������p��
	sprintf_s<64>(str, "MP:%3d", pl->GetMP());
	GameDevice()->m_pFont->Draw(WINDOW_WIDTH * 1/4 , WINDOW_HEIGHT - 120, str, 50, RGB(255, 0,0));

	switch (pl->GetSelectPower()){
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
	GameDevice()->m_pFont->Draw(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 120, str, 50, RGB(255, 0,0));

	sprintf_s<64>(str, "Score:%3d", pl->GetScore() );
	GameDevice()->m_pFont->Draw(WINDOW_WIDTH / 2 - 70, 30, str, 50, RGB(255, 255, 0));

}
