#include "TitleScene.h"
#include "GameMain.h"
#include "EasingCalc.h"


TitleScene::TitleScene()
{
	CsvLoad();

	//スプライトイメージのロード
	titleBackImage	= new CSpriteImage(_T("data/Image/title/title.png"));
	broomImage		= new CSpriteImage(_T("data/Image/title/Broom.png"));
	titleText1Image = new CSpriteImage(_T("data/Image/title/Text1.png"));
	titleText2Image = new CSpriteImage(_T("data/Image/title/Text2.png"));
	titleText3Image = new CSpriteImage(_T("data/Image/title/Text3.png"));
	
	sprite = new CSprite();
	easing = new EasingCalc();

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
	SAFE_DELETE(easing);
}

void TitleScene::Update()
{
	auto di = GameDevice()->m_pDI;

	if (animNum == aFinish) {
		// 2024.10.27 コードの可読性向上とコントローラー入力対応
		if (di->CheckKey(KD_TRG, DIK_S) || di->CheckJoy(KD_TRG, DIJ_B)) {
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
	else if (di->CheckKey(KD_TRG, DIK_SPACE) || di->CheckJoy(KD_TRG, DIJ_B)) {
		animNum = aFinish;
	}
	
}

void TitleScene::Draw()
{
	animTime = animFrm * (1.0f / 60.0f);

	switch (animNum) {
	case aWait: //背景フェードイン
		
		timeRate = animTime / AnimTime[aWait];
		rate = timeRate; //線形補完

		alpha = (AlphaGoal + AlphaStart) * rate - AlphaStart;

		sprite->Draw(titleBackImage, 0, 0, 0, 0, 1536, 864, WINDOW_WIDTH, WINDOW_HEIGHT,alpha);
		if (animTime >= AnimTime[aWait]) {
			animNum = aBroom;
			animFrm = 0;
		}
		break;
	case aBroom: //箒のスプライトアニメーション表示

		sprite->Draw(titleBackImage, 0, 0, 0, 0, 1536, 864, WINDOW_WIDTH, WINDOW_HEIGHT);

		timeRate = animTime / AnimTime[aBroom];
		rate = easing->easeOutBack(timeRate);
		rot = (RotateGoal + RotateStart) * rate - RotateStart;
		scale = (ScaleGoal + ScaleStart) * rate - ScaleStart;

		//スプライトの表示用ワールド行列を作成
		sprite->SetImage(broomImage);
		MATRIX4X4 m = XMMatrixRotationZ(rot) * XMMatrixScaling(scale, scale, 1) * XMMatrixTranslation(0, 0, 0);

		//ワールド座標に沿ったスプライトの表示
		sprite->Draw(m);

		if (animTime >= AnimTime[aBroom])
		{
			animNum = aText1;
			animFrm = 0;
		}
		break;
	case aText1: //テキスト「まほうつかいの」アニメーション表示
		sprite->Draw(titleBackImage, 0, 0, 0, 0, 1536, 864, WINDOW_WIDTH, WINDOW_HEIGHT);
		sprite->Draw(broomImage, 0, 0, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);

		timeRate = animTime / AnimTime[aText1];
		rate = easing->easeOutBack(timeRate);
		trans = (TransGoal + TransStart) * rate - TransStart;

		sprite->Draw(titleText1Image, 0, trans, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);

		if (animTime >= AnimTime[aText1])
		{
			animNum = aText2;
			animFrm = 0;
		}
		break;
	case aText2: //テキスト「おそうじ大作戦」アニメーション表示
		sprite->Draw(titleBackImage, 0, 0, 0, 0, 1536, 864, WINDOW_WIDTH, WINDOW_HEIGHT);
		sprite->Draw(broomImage, 0, 0, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);
		sprite->Draw(titleText1Image, 0, 0, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);

		timeRate = animTime / AnimTime[aText2];
		rate = easing->easeOutBounce(timeRate);
		scale = (ScaleGoal + ScaleStart) * rate - ScaleStart;
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

		sprite->Draw(titleBackImage, 0, 0, 0, 0, 1536, 864, WINDOW_WIDTH, WINDOW_HEIGHT);

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


void TitleScene::CsvLoad()
{
	// csvからデータ読み込み
	csv = new CsvReader("data/csv/AnimParam.csv");
	if (csv->GetLines() < 1) {
		MessageBox(NULL, "AnimParam.csvが読めません", "エラー", MB_OK);
	}

	for (int i = 1; i < csv->GetLines(); i++) { //CSVファイルから設定の読み込み
		if (csv->GetString(i, 0) == "Title") {
			if (csv->GetString(i, 1) == "AnimTime") {	// 各アニメーション時間
				AnimTime[0] = csv->GetFloat(i, 3);
				AnimTime[1] = csv->GetFloat(i, 4);
				AnimTime[2] = csv->GetFloat(i, 5);
				AnimTime[3] = csv->GetFloat(i, 6);
			}
			if (csv->GetString(i, 1) == "ScaleStart"){	// 開始時拡大率
				ScaleStart = csv->GetFloat(i, 3);
			}
			if (csv->GetString(i, 1) == "ScaleGoal") {	// 終了時拡大率
				ScaleGoal = csv->GetFloat(i, 3);
			}
			if (csv->GetString(i, 1) == "RotateStart"){// 開始時回転
				RotateStart = csv->GetFloat(i, 3) * DegToRad;
			}
			if (csv->GetString(i, 1) == "RotateGoal") {	// 終了時回転
				RotateGoal = csv->GetFloat(i, 3) * DegToRad;;
			}
			if (csv->GetString(i, 1) == "TransStart") {	// 開始時位置
				TransStart = csv->GetFloat(i, 3);
			}
			if (csv->GetString(i, 1) == "TransGoal")  {	// 終了時位置
				TransGoal = csv->GetFloat(i, 3);
			}
			if (csv->GetString(i, 1) == "AlphaStart") {	// 開始時透明度
				AlphaStart = csv->GetFloat(i, 3);
			}
			if (csv->GetString(i, 1) == "AlphaGoal") {	// 終了時透明度
				AlphaGoal = csv->GetFloat(i, 3);
			}
		}
		if (csv->GetString(i, 0) == "Select") { // Titleが終わったら読み込み終了
			break;
		}
	}
}
