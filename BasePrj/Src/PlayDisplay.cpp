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
	sprintf_s<64>(str, "Weight: %4d", pl->GetWeight());
	GameDevice()->m_pFont->Draw(WINDOW_WIDTH / 2 , WINDOW_HEIGHT - 120, str, 50, RGB(255, 255, 255));

}
