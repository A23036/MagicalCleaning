#include "DisplayInfo.h"
#include "Player.h"

namespace {
	const int ScoreMax = 1000;   // スコアバーの表示上のスコアの最大値
}

DisplayInfo::DisplayInfo()
{
	frameUiImage = new CSpriteImage(_T("data/Image/frameUI.png"));
	sprite = new CSprite();

	SetPriority(-9999); 	// 最後より一つ前に処理する
	SetDrawOrder(-10000);	// 一番最後に描画する
}

DisplayInfo::~DisplayInfo()
{
	SAFE_DELETE(frameUiImage);
	SAFE_DELETE(sprite);
}

void DisplayInfo::Update()
{
}

void DisplayInfo::Draw()
{
	//UI表示
	std::string s = "Player";
	int n = ObjectManager::DrawCounter();
	s = s + std::to_string(n+1);

	Player* pl = ObjectManager::FindGameObjectWithTag<Player>(s);
	char str[64]; //文字列を用意
	sprintf_s<64>(str, "Weight: %4d", pl->GetWeight());
	GameDevice()->m_pFont->Draw(WINDOW_WIDTH / 2 , WINDOW_HEIGHT - 120, str, 50, RGB(255, 255, 255));

	int imgX=0, imgY=0;
	if (n%2 == 0) { //Player0,2の位置は左側
		imgX = 0;
	}
	else {
		imgX = 1;
	}
	if (n < 2) { //Player
		imgY = 0;
	}
	else {
		imgY = 1;
	}

	// 枠のUI表示
	sprite->Draw(frameUiImage, 0, 0, imgX * 512, imgY * 288, 512, 288, WINDOW_WIDTH, WINDOW_HEIGHT);
	

	/*
	float h = 0;
	TCHAR str[256];
	int   DestX, DestY;
	
		// ステータスバーの表示
	Player* obj = ObjectManager::FindGameObject<Player>();
	h = (float)obj->HpdivMax();
	if (h < 0) h = 0;

	// ＨＰと残機数の表示
	DestX = 10;
	DestY = 10;
	sprite->Draw(image, DestX, DestY, 0, 0, 213, 31);
	sprite->Draw(image, DestX + 59, DestY + 6, 59, 32, (DWORD)(144 * h), 6);

	_stprintf_s(str, _T("%d"), obj->Number());
	GameDevice()->m_pFont->Draw(DestX + 6, DestY + 15, str, 16, RGB(255, 0, 0));
	_stprintf_s(str, _T("%06d"), obj->HitPoint());
	GameDevice()->m_pFont->Draw(DestX + 26, DestY + 16, str, 12, RGB(0, 0, 0));

	// スコア枠の表示
	DestX = WINDOW_WIDTH - 213 - 10;
	DestY = 10;
	sprite->Draw(image, DestX, DestY, 0, 112, 213, 28);
	// スコアバーの表示
	int score = ObjectManager::FindGameObject<DataCarrier>()->Score();  // スコア
	h = (float)score / ScoreMax;
	if (h < 0) h = 0;
	if (h > 1) h = 1;
	sprite->Draw(image, DestX + 59, DestY + 6, 59, 142, (DWORD)(144 * h), 6);
	// スコア数値の表示
	_stprintf_s(str, _T("%06d"), score);
	GameDevice()->m_pFont->Draw(DestX + 8, DestY + 17, str, 11, RGB(0, 0, 0));
	*/
}
