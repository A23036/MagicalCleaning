#include "TitleScene.h"
#include "GameMain.h"


namespace {
	float AnimTime[4] = { 3.0f, 0.8f, 0.6f, 1.0f };
	float scaleStart = 0.0f;
	float scaleGoal = 1.0f;
	float rotateStart = 90.0f * DegToRad;
	float rotateGoal = 0.0f;
	float transStart = 500.0f;
	float transGoal = 0.0f;
	float alphaStart = 0.0f;
	float alphaGoal = 1.0f;
}

TitleScene::TitleScene()
{
	//スプライトイメージのロード
	titleBackImage	= new CSpriteImage(_T("data/Image/title/title.png"));
	broomImage		= new CSpriteImage(_T("data/Image/title/Broom.png"));
	titleText1Image = new CSpriteImage(_T("data/Image/title/Text1.png"));
	titleText2Image = new CSpriteImage(_T("data/Image/title/Text2.png"));
	titleText3Image = new CSpriteImage(_T("data/Image/title/Text3.png"));
	sprite = new CSprite();
	
	animNum = 0;
	animFrm = 0;
	animTime = 0;

	timeRate = 0;
	rate = 0;

	alpha = 0;
	rot = 0;
	scale = 0;
	trans = 0;
}

TitleScene::~TitleScene()
{
	SAFE_DELETE(titleBackImage);
	SAFE_DELETE(broomImage);
	SAFE_DELETE(titleText1Image);
	SAFE_DELETE(titleText2Image);
	SAFE_DELETE(titleText3Image);
	SAFE_DELETE(sprite);
}

void TitleScene::Update()
{
	// 2024.10.27 コードの可読性向上とコントローラー入力対応
	auto di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_TRG, DIK_S) || di->CheckJoy(KD_TRG,DIJ_B)) {
		SceneManager::ChangeScene("SelectScene");
	}
	if (di->CheckKey(KD_TRG, DIK_V)) {
		SceneManager::ChangeScene("ViewMapScene");
	}
	if (di->CheckKey(KD_TRG, DIK_P)) {
		SceneManager::ChangeScene("PlayScene");
	}
	if (di->CheckKey(KD_TRG, DIK_E)) {
		SceneManager::ChangeScene("EditScene");
	}
}

void TitleScene::Draw()
{
	animTime = animFrm * (1.0f / 60.0f);

	switch (animNum) {
	case aWait: //開始後少し待機
		
		timeRate = animTime / AnimTime[aWait];
		rate = timeRate;

		alpha = (alphaGoal + alphaStart) * rate - alphaStart;

		sprite->Draw(titleBackImage, 0, 0, 0, 0, 1536, 864, WINDOW_WIDTH, WINDOW_HEIGHT,alpha);
		if (animTime >= AnimTime[aWait]) {
			animNum = aBroom;
			animFrm = 0;
		}
		break;
	case aBroom: //箒のスプライトアニメーション表示

		sprite->Draw(titleBackImage, 0, 0, 0, 0, 1536, 864, WINDOW_WIDTH, WINDOW_HEIGHT, alpha);

		// ダミー表示(画面外に描画されている)
		sprite->Draw(broomImage, 0, WINDOW_HEIGHT, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);

		timeRate = animTime / AnimTime[aBroom];
		rate = easeOutBack(timeRate);
		rot = (rotateGoal + rotateStart) * rate - rotateStart;
		scale = (scaleGoal + scaleStart) * rate - scaleStart;

		//スプライトの表示用ワールド行列を作成
		MATRIX4X4 m = XMMatrixRotationRollPitchYaw(0, 0, rot) * XMMatrixScaling(scale, scale, 1);
		
		//ワールド座標に沿ったスプライトの表示
		sprite->Draw(m);

		if (animTime >= AnimTime[aBroom])
		{
			animNum = aText1;
			animFrm = 0;
		}
		break;
	case aText1: //テキスト「まほうつかいの」アニメーション表示
		sprite->Draw(titleBackImage, 0, 0, 0, 0, 1536, 864, WINDOW_WIDTH, WINDOW_HEIGHT, alpha);
		sprite->Draw(broomImage, 0, 0, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);

		timeRate = animTime / AnimTime[aText1];
		rate = easeOutBack(timeRate);
		trans = (transGoal + transStart) * rate - transStart;

		sprite->Draw(titleText1Image, 0, trans, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);

		if (animTime >= AnimTime[aText1])
		{
			animNum = aText2;
			animFrm = 0;
		}
		break;
	case aText2: //テキスト「おそうじ大作戦」アニメーション表示
		sprite->Draw(titleBackImage, 0, 0, 0, 0, 1536, 864, WINDOW_WIDTH, WINDOW_HEIGHT, alpha);
		sprite->Draw(broomImage, 0, 0, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);
		sprite->Draw(titleText1Image, 0, 0, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);

		timeRate = animTime / AnimTime[aText2];
		rate = easeOutBounce(timeRate);
		scale = (scaleGoal + scaleStart) * rate - scaleStart;
		trans = 1 - scale;

		sprite->Draw(titleText2Image, WINDOW_WIDTH/2 * trans, WINDOW_HEIGHT/2 * trans, 
							0, 0, 1280, 720, WINDOW_WIDTH * scale, WINDOW_HEIGHT * scale);
		if (animTime >= AnimTime[aText2])
		{
			animNum = aFinish;
			animFrm = 0;
		}
		break;
	case aFinish: //アニメーション終了、プレイヤーの入力待機

		sprite->Draw(titleBackImage, 0, 0, 0, 0, 1536, 864, WINDOW_WIDTH, WINDOW_HEIGHT, alpha);

		float d1 = sinf(animFrm / 120.0f * XM_2PI) * 5.0f;
		float d2 = 1 + fabs(sinf(animFrm / 300.0f * XM_2PI) * 0.05f);

		// 中央で拡縮するためのオフセットを計算
		float offsetX = (WINDOW_WIDTH * (1 - d2)) / 2;
		float offsetY = (WINDOW_HEIGHT * (1 - d2)) / 2;

		sprite->Draw(broomImage, 0, d1, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);
		sprite->Draw(titleText1Image, 0, 0, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);
		sprite->Draw(titleText2Image, offsetX, offsetY, 0, 0, 1280, 720, WINDOW_WIDTH * d2, WINDOW_HEIGHT * d2);

		if (animFrm % 60 < 40) {
			sprite->SetSrc(0,0,612,32);
			sprite->Draw(titleText3Image, WINDOW_WIDTH/2 - (sprite->GetSrcWidth())/2, WINDOW_HEIGHT* 3/4,
							0, 0, sprite->GetSrcWidth(), sprite->GetSrcHeight());
			int n = sprite->GetSrcWidth() / 2;
			int a = n;
		}
		GameDevice()->m_pFont->Draw(400, 15, _T("TITLE SCENE"), 32, RGB(255, 0, 0));
		GameDevice()->m_pFont->Draw(400, 100, _T("[S]key : SELECT SCENE"), 32, RGB(255, 0, 0));
		GameDevice()->m_pFont->Draw(400, 130, _T("[V]Key : VIEW MAP SCENE"), 32, RGB(255, 0, 0));
		GameDevice()->m_pFont->Draw(400, 160, _T("[P]Key : PLAY SCENE"), 32, RGB(255, 0, 0));

		break;

	}

	//　デバッグ用アニメーションタイム表示
	char str[64];
	sprintf_s<64>(str, "%.1f", animFrm * (1.0f / 60.0f));
	GameDevice()->m_pFont->Draw(0, 0, str, 50, RGB(255, 255, 255));
	
	animFrm++;
}

float TitleScene::easeOutBack(float x) //イージング関数:最後に少し飛び出してから戻る
{
	float c1 = 1.70158;
	float c3 = c1 + 1;

	return 1 + c3 * pow(x - 1, 3) + c1 * pow(x - 1, 2);
}

float TitleScene::easeOutBounce(float x) //イージング関数:最後にはねる
{
	float n1 = 7.5625f;
	float d1 = 2.75f;

	if (x < 1 / d1) {
		return n1 * x * x;
	}
	else if (x < 2 / d1) {
		x -= 1.5f / d1;
		return n1 * x * x + 0.75f;
	}
	else if (x < 2.5 / d1) {
		x -= 2.25f / d1;
		return n1 * x * x + 0.9375f;
	}
	else {
		x -= 2.625f / d1;
		return n1 * x * x + 0.984375f;
	}
}
