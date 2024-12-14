#include "PlayDisplay.h"
#include "PlayScene.h"
#include "Player.h"
#include "DataCarrier.h"
#include "EasingCalc.h"

PlayDisplay::PlayDisplay()
{
	SetPriority(-9999); 	// 最後より一つ前に処理する
	SetDrawOrder(-10000);	// 一番最後に描画する

	countDownImage = new CSpriteImage(_T("data/Image/Play/CountDown.png"));
	playUiImage = new CSpriteImage(_T("data/Image/Play/UISprite.png"));

	sprite = new CSprite();
	
	animFrm = 0;
	animTime = 0;
	
	rotStart = 0;
	rotGoal = 360 * DegToRad;
	ScaleStart = 0.0f;
	ScaleGoal = 2.0f;
}

PlayDisplay::~PlayDisplay()
{
	SAFE_DELETE(countDownImage);
	SAFE_DELETE(playUiImage);
	SAFE_DELETE(sprite);
}

void PlayDisplay::Start()
{
	dc = ObjectManager::FindGameObject<DataCarrier>();
	ec = ObjectManager::FindGameObject<EasingCalc>();
}

void PlayDisplay::Update()
{
	animTime = (1.0f / 60.0f) * animFrm;
	if (animTime >= 1.0f) {
		animFrm = 0;
	}
	gameState = dc->GetGameState();
	gameTime = dc->GetGameTime();

	animFrm++;
}

void PlayDisplay::Draw()
{
	char str[64]; //文字列
	auto font = GameDevice()->m_pFont;

	//ゲーム開始時のカウントダウン表示
	if (gameState == sReady) { 
		if (gameTime == 0) {
			sprite->SetSrc(countDownImage, 132, 0, 66, 66);
		}
		if (gameTime == 1) {
			sprite->SetSrc(countDownImage, 66, 0, 66, 66);
		}
		if (gameTime == 2) {
			sprite->SetSrc(countDownImage, 0, 0, 66, 66);
		}
		if (gameTime == 3) {
			sprite->SetSrc(countDownImage, 198, 0, 186, 66);
		}
		int width, height;
		VECTOR2 center;
		width = sprite->GetSrcWidth();
		height = sprite->GetSrcHeight();
		center.x = WINDOW_WIDTH / 2 - width / 2;
		center.y = WINDOW_HEIGHT / 2 - height / 2;

		// アニメーションの進行度計算
		float timeRate = animTime / 1.0f;
		float rate = ec->easeOutBack(timeRate); // 滑らかな拡大

		float rotation = (rotGoal - rotStart) * rate + rotStart;	// 回転
		float scale = (ScaleGoal - ScaleStart) * rate + ScaleStart;	// 拡大倍率

		// 回転中心を画像の中心にするための補正
		float pivotX = width / 2.0f;
		float pivotY = height / 2.0f;

		// ワールド行列の計算
		MATRIX4X4 m = XMMatrixTranslation(-pivotX, -pivotY, 0) 
			* XMMatrixRotationZ(rotation)
			* XMMatrixScaling(scale, scale, 1.0f)
			* XMMatrixTranslation(center.x, center.y, 0);

		// スプライトをワールド行列を使用して描画
		sprite->Draw(m);
	}


	//基本UI表示
	//プレイヤーごとのUI描画
	std::string s = "Player";
	int n = ObjectManager::DrawCounter();
	s = s + std::to_string(n+1);
	Player* pl = ObjectManager::FindGameObjectWithTag<Player>(s);

	//Leaf、MP、能力表示----------
	//UIベース
	sprite->SetSrc(playUiImage, 0, 112, 412, 48,412 * 2,48 * 2);
	sprite->Draw(200, WINDOW_HEIGHT - 130);

	//Leaf表示
	//Leaf所持数を割合ゲージで表示
	int offY;
	float rate;		
	rate = (float)pl->GetLeaf() / (float)pl->GetCarWeight();
	offY = 48 * rate;
	sprite->SetSrc(playUiImage, 0, 160 + 48 - offY, 48, 48, 48 * 2, 48 * 2);
	sprite->Draw(200, WINDOW_HEIGHT - 130 + 48*2 - offY*2);
	
	//Leaf所持数の表示
	sprintf_s<64>(str, "%3d", pl->GetLeaf());
	//文字影
	font->Draw(200 - 10, WINDOW_HEIGHT - 115, str, 70, RGB(0, 0, 120));

	if (rate == 1) { //Leafが所持上限の時赤文字
		font->Draw(200 - 15, WINDOW_HEIGHT - 120, str, 70, RGB(255, 0, 0));
	}
	else {
		font->Draw(200 - 15, WINDOW_HEIGHT - 120, str, 70, RGB(255, 255, 255));
	}
	
	//MP表示
	sprintf_s<64>(str, "%3d", pl->GetMP());
	GameDevice()->m_pFont->Draw(340, WINDOW_HEIGHT - 105, str, 65, RGB(255, 255, 255));

	int offX;
	rate = (float)pl->GetMP() / (float)pl->GetPowerCost(pl->GetSelectPower());
	if (rate >= 1.0f) {
		rate = 1.0f;
	}
	offX = 236 * rate;
	sprite->SetSrc(playUiImage, 172, 166, offX, 4, offX*2, 4*2);
	sprite->Draw(200 + 172*2, WINDOW_HEIGHT - 130 + 80);

	
	switch (pl->GetSelectPower()){ //選択中パワー描画(アイコンと文字)
	case pMS:
		sprite->SetSrc(playUiImage, 0, 0, 64, 64, 90, 90);		//アイコン
		sprite->Draw(680, 610);
		sprite->SetSrc(playUiImage, 0, 68, 68, 40, 68*2.5, 40*2.5); //文字
		sprite->Draw(780, 605);
		sprite->m_vDiffuse = VECTOR4(1, 1, 1, 0.6f);			//左右のパワーは半透明
		sprite->SetSrc(playUiImage, 272, 0, 64, 64, 40, 40);	//左のパワー
		sprite->Draw(580, 660);
		sprite->SetSrc(playUiImage, 64, 0, 64, 64, 40, 40);		//右のパワー
		sprite->Draw(970, 660);
		sprite->m_vDiffuse = VECTOR4(1, 1, 1, 1);				//透明度を戻す
		break;
	case pJN:
		sprite->SetSrc(playUiImage, 64, 0, 64, 64, 90, 90);
		sprite->Draw(680, 610);
		sprite->SetSrc(playUiImage, 72, 68, 68, 40, 68 * 2.5, 40 * 2.5);
		sprite->Draw(780, 605);
		sprite->m_vDiffuse = VECTOR4(1, 1, 1, 0.6f);
		sprite->SetSrc(playUiImage, 0, 0, 64, 64, 40, 40);
		sprite->Draw(580, 660);
		sprite->SetSrc(playUiImage, 128, 0, 64, 64, 40, 40);
		sprite->Draw(970, 660);
		sprite->m_vDiffuse = VECTOR4(1, 1, 1, 1);
		break;
	case pAS:
		sprite->SetSrc(playUiImage, 128, 0, 64, 64, 90, 90);
		sprite->Draw(680, 610);
		sprite->SetSrc(playUiImage, 144, 68, 68, 40, 68 * 2.5, 40 * 2.5);
		sprite->Draw(780, 605);
		sprite->m_vDiffuse = VECTOR4(1, 1, 1, 0.6f);
		sprite->SetSrc(playUiImage, 64, 0, 64, 64, 40, 40);
		sprite->Draw(580, 660);
		sprite->SetSrc(playUiImage, 192, 0, 64, 64, 40, 40);
		sprite->Draw(970, 660);
		sprite->m_vDiffuse = VECTOR4(1, 1, 1, 1);
		break;
	case pAR:
		sprite->SetSrc(playUiImage, 192, 0, 64, 64, 90, 90);
		sprite->Draw(680, 610);
		sprite->SetSrc(playUiImage, 216, 68, 68, 40, 68 * 2.5, 40 * 2.5);
		sprite->Draw(780, 605);
		sprite->m_vDiffuse = VECTOR4(1, 1, 1, 0.6f);
		sprite->SetSrc(playUiImage, 128, 0, 64, 64, 40, 40);
		sprite->Draw(580, 660);
		sprite->SetSrc(playUiImage, 272, 0, 64, 64, 40, 40);
		sprite->Draw(970, 660);
		sprite->m_vDiffuse = VECTOR4(1, 1, 1, 1);
		break;
	case pCW:
		sprite->SetSrc(playUiImage, 272, 0, 64, 64, 90, 90);
		sprite->Draw(680, 610);
		sprite->SetSrc(playUiImage, 288, 68, 68, 40, 68 * 2.5, 40 * 2.5);
		sprite->Draw(780, 605);
		sprite->m_vDiffuse = VECTOR4(1, 1, 1, 0.6f);
		sprite->SetSrc(playUiImage, 192, 0, 64, 64, 40, 40);
		sprite->Draw(580, 660);
		sprite->SetSrc(playUiImage, 0, 0, 64, 64, 40, 40);
		sprite->Draw(970, 660);
		sprite->m_vDiffuse = VECTOR4(1, 1, 1, 1);
		break;
	}
	
	GameDevice()->m_pFont->Draw(WINDOW_WIDTH - 320, WINDOW_HEIGHT - 120, _T("COST"), 40, RGB(255, 255, 255));
	sprintf_s<64>(str, "%3d", pl->GetPowerCost(pl->GetSelectPower()));

	if (pl->GetPowerCost(pl->GetSelectPower()) <= pl->GetMP()) { //mpが足りていたらコストを白色表示
		GameDevice()->m_pFont->Draw(WINDOW_WIDTH - 300, WINDOW_HEIGHT - 80, str, 40, RGB(255, 255, 255));
	}
	else{
		GameDevice()->m_pFont->Draw(WINDOW_WIDTH - 300, WINDOW_HEIGHT - 80, str, 40, RGB(255, 0, 0));
	}
	
	//スコア
	sprite->SetSrc(75, 175, 176, 32, 176 * 1.5, 32 * 1.5);
	sprite->Draw(WINDOW_WIDTH/2 - sprite->GetSrcWidth()-30,40);
	sprintf_s<64>(str, "%3d", pl->GetScore() );
	
	// テキスト幅を計算する
	int textWidth = strlen(str) * 60;

	// 描画位置を右揃えに調整
	int xPosition = (WINDOW_WIDTH / 2 - 130) + textWidth;

	// テキストを描画
	GameDevice()->m_pFont->Draw(xPosition+5, 32+5, str, 60, RGB(0,0,0));
	GameDevice()->m_pFont->Draw(xPosition, 32, str, 60, RGB(255, 255, 255));
	
}
