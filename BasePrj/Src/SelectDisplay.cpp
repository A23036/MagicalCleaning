#include "SelectDisplay.h"

SelectDisplay::SelectDisplay()
{
	SetDrawOrder(-10000);	// 一番最後に描画する
	
	sprite = new CSprite();
	selectUiImage = new CSpriteImage(_T("data/Image/Select/UI.png"));

	ColorPosY = WINDOW_HEIGHT * 2/5;		//キャラ選択カラー位置Y
	ColorIconSize = 64;						//キャラ選択カラーサイズ
	ColorIconDispSize = 100;				//キャラ選択カラー表示サイズ
	UiSpace = 50;							//キャラ選択カラー幅
	GuideUiPosY = WINDOW_HEIGHT - 100;		//操作方法ガイドUI位置Y
	BackUiPos = VECTOR2(20,20);	//戻るUI位置

	for (int i = 0; i < 4; i++) {
		playerEntry[i] = false;
	}
}

SelectDisplay::~SelectDisplay()
{
	SAFE_DELETE(sprite);
	SAFE_DELETE(selectUiImage);
}

void SelectDisplay::Update()
{
}

void SelectDisplay::Draw()
{
	//前面UIの描画
	sprite->SetImage(selectUiImage);

	//戻るアイコン
	sprite->SetSrc(0, 320, 64, 64);
	sprite->Draw(BackUiPos.x, BackUiPos.y);

	//戻るボタン
	sprite->SetSrc(64 * DOWN, 64, 64, 64);
	sprite->Draw(BackUiPos.x + 64 + 16, BackUiPos.y);

	//キャラクターカラー
	//赤
	sprite->SetSrc(96, 128, ColorIconSize, ColorIconSize, ColorIconDispSize, ColorIconDispSize);
	sprite->Draw(WINDOW_WIDTH / 2 - ColorIconDispSize /2  - UiSpace - ColorIconDispSize, ColorPosY);
	
	//青
	sprite->SetSrc(96 + ColorIconSize, 128, ColorIconSize, ColorIconSize, ColorIconDispSize, ColorIconDispSize);
	sprite->Draw(WINDOW_WIDTH / 2 - ColorIconDispSize /2, ColorPosY);

	//黄
	sprite->SetSrc(96 + ColorIconSize * 2, 128, ColorIconSize, ColorIconSize, ColorIconDispSize, ColorIconDispSize);
	sprite->Draw(WINDOW_WIDTH / 2 - ColorIconDispSize /2 + UiSpace + ColorIconDispSize, ColorPosY);

	//緑
	sprite->SetSrc(96, 128+ ColorIconSize, ColorIconSize, ColorIconSize, ColorIconDispSize, ColorIconDispSize);
	sprite->Draw(WINDOW_WIDTH / 2 - ColorIconDispSize / 2 - UiSpace - ColorIconDispSize, ColorPosY + UiSpace + ColorIconDispSize);

	//紫
	sprite->SetSrc(96 + ColorIconSize, 128 + ColorIconSize, ColorIconSize, ColorIconSize, ColorIconDispSize, ColorIconDispSize);
	sprite->Draw(WINDOW_WIDTH / 2 - ColorIconDispSize / 2, ColorPosY + UiSpace + ColorIconDispSize);

	//黒
	sprite->SetSrc(96 + ColorIconSize * 2, 128 + ColorIconSize, ColorIconSize, ColorIconSize, ColorIconDispSize, ColorIconDispSize);
	sprite->Draw(WINDOW_WIDTH / 2 - ColorIconDispSize / 2 + UiSpace + ColorIconDispSize, ColorPosY + UiSpace + ColorIconDispSize);

}
