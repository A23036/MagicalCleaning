#include "PlayDisplay.h"
#include "PlayScene.h"
#include "Player.h"
#include "DataCarrier.h"

PlayDisplay::PlayDisplay()
{
	SetPriority(-9999); 	// ÅŒã‚æ‚èˆê‚Â‘O‚Éˆ—‚·‚é
	SetDrawOrder(-10000);	// ˆê”ÔÅŒã‚É•`‰æ‚·‚é
}

PlayDisplay::~PlayDisplay()
{
}

void PlayDisplay::Update()
{
}

void PlayDisplay::Draw()
{
	//UI•\¦
	std::string s = "Player";
	int n = ObjectManager::DrawCounter();
	s = s + std::to_string(n+1);

	Player* pl = ObjectManager::FindGameObjectWithTag<Player>(s);
	char str[64]; //•¶š—ñ‚ğ—pˆÓ
	sprintf_s<64>(str, "Weight: %4d", pl->GetWeight());
	GameDevice()->m_pFont->Draw(WINDOW_WIDTH / 2 , WINDOW_HEIGHT - 120, str, 50, RGB(255, 255, 255));

}
