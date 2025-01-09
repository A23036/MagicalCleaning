#include "TitleScene.h"
#include "GameMain.h"


TitleScene::TitleScene()
{
	CsvLoad();

	//スプライトイメージのロード
	titleBackImage	= new CSpriteImage(_T("data/Image/Title/title.png"));
	broomImage		= new CSpriteImage(_T("data/Image/Title/Broom.png"));
	titleText1Image = new CSpriteImage(_T("data/Image/Title/Text1.png"));
	titleText2Image = new CSpriteImage(_T("data/Image/Title/Text2.png"));
	titleText3Image = new CSpriteImage(_T("data/Image/Title/Text3.png"));
	
	sprite = new CSprite();
	ec = new EasingCalc();
	dc = SingleInstantiate<DataCarrier>(); //DataCarrierはシングルトンで生成

	state = aFadeIn;
	animFrm = 0;
	animTime = 0;

	timeRate = 0;
	rate = 0;

	alpha = 0;
	rot = 0;
	scale = 0;
	trans = 0;

	d1 = 0;
	d2 = 0;
	offX = 0;
	offY = 0;
}

TitleScene::~TitleScene()
{
	SAFE_DELETE(titleBackImage);
	SAFE_DELETE(broomImage);
	SAFE_DELETE(titleText1Image);
	SAFE_DELETE(titleText2Image);
	SAFE_DELETE(titleText3Image);
	SAFE_DELETE(sprite);
	SAFE_DELETE(ec);
	SAFE_DELETE(csv);
}

void TitleScene::Update()
{
	auto di = GameDevice()->m_pDI;

	if (state == aWait) {
		// 2024.10.27 コードの可読性向上とコントローラー入力対応
		if ((di->CheckKey(KD_TRG, DIK_S) || di->CheckJoy(KD_TRG, 2)) && state != aTransition){
			state = aTransition;
			animFrm = 0;
		}
		if (di->CheckKey(KD_TRG, DIK_V)) {
			SceneManager::ChangeScene("ViewMapScene");
		}
		if (di->CheckKey(KD_TRG, DIK_P)) {
			SceneManager::ChangeScene("PlayScene");
		}
		/*
		if (di->CheckKey(KD_TRG, DIK_E)) {
			SceneManager::ChangeScene("EditScene");
		}
		*/
	}
	else if ((di->CheckKey(KD_TRG, DIK_S) || di->CheckKey(KD_TRG, DIK_P) || di->CheckJoy(KD_TRG, 2)) && state != aTransition){
		state = aWait;
	}

	if (state == aFinish){
		SceneManager::ChangeScene("SelectScene");
	}
}

void TitleScene::Draw()
{
	animTime = animFrm * (1.0f / 60.0f);

	switch (state) {
	case aFadeIn: //背景フェードイン
		
		timeRate = animTime / AnimTime[aFadeIn];
		rate = timeRate; //線形補完

		alpha = (AlphaGoal -AlphaStart) * rate + AlphaStart;

		sprite->Draw(titleBackImage, 0, 0, 0, 0, 1536, 864, WINDOW_WIDTH, WINDOW_HEIGHT,alpha);
		if (animTime >= AnimTime[aFadeIn]) {
			state = aBroom;
			animFrm = 0;
		}
		break;
	case aBroom: //箒のスプライトアニメーション表示

		sprite->Draw(titleBackImage, 0, 0, 0, 0, 1536, 864, WINDOW_WIDTH, WINDOW_HEIGHT);

		timeRate = animTime / AnimTime[aBroom];
		rate = ec->easeOutBack(timeRate);
		rot = (RotateGoal - RotateStart) * rate + RotateStart;
		scale = (ScaleGoal - ScaleStart) * rate + ScaleStart;

		//スプライトの表示用ワールド行列を作成
		sprite->SetImage(broomImage);
		MATRIX4X4 m = XMMatrixRotationZ(rot) * XMMatrixScaling(scale, scale, 1) * XMMatrixTranslation(0, 0, 0);

		//ワールド座標に沿ったスプライトの表示
		sprite->Draw(m);

		if (animTime >= AnimTime[aBroom])
		{
			state = aText1;
			animFrm = 0;
		}
		break;
	case aText1: //テキスト「まほうつかいの」アニメーション表示
		sprite->Draw(titleBackImage, 0, 0, 0, 0, 1536, 864, WINDOW_WIDTH, WINDOW_HEIGHT);
		sprite->Draw(broomImage, 0, 0, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);

		timeRate = animTime / AnimTime[aText1];
		rate = ec->easeOutBack(timeRate);
		trans = (TransGoal - TransStart) * rate + TransStart;

		sprite->Draw(titleText1Image, 0, trans, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);

		if (animTime >= AnimTime[aText1])
		{
			state = aText2;
			animFrm = 0;
		}
		break;
	case aText2: //テキスト「おそうじ大作戦」アニメーション表示
		sprite->Draw(titleBackImage, 0, 0, 0, 0, 1536, 864, WINDOW_WIDTH, WINDOW_HEIGHT);
		sprite->Draw(broomImage, 0, 0, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);
		sprite->Draw(titleText1Image, 0, 0, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);

		timeRate = animTime / AnimTime[aText2];
		rate = ec->easeOutBounce(timeRate);
		scale = (ScaleGoal - ScaleStart) * rate + ScaleStart;
		trans = 1 - scale;

		sprite->Draw(titleText2Image, WINDOW_WIDTH/2 * trans, WINDOW_HEIGHT/2 * trans, 
							0, 0, 1280, 720, WINDOW_WIDTH * scale, WINDOW_HEIGHT * scale);
		if (animTime >= AnimTime[aText2])
		{
			state = aWait;
			animFrm = 0;
		}
		break;
	case aWait: //アニメーション終了、プレイヤーの入力待機
		sprite->Draw(titleBackImage, 0, 0, 0, 0, 1536, 864, WINDOW_WIDTH, WINDOW_HEIGHT);

		d1 = sinf(animFrm / 120.0f * XM_2PI) * 5.0f;
		d2 = 1 + fabs(sinf(animFrm / 300.0f * XM_2PI) * 0.05f);

		// 中央で拡縮するためのオフセットを計算
		offX = (WINDOW_WIDTH * (1 - d2)) / 2;
		offY = (WINDOW_HEIGHT * (1 - d2)) / 2;

		sprite->Draw(broomImage, 0, d1, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);
		sprite->Draw(titleText1Image, 0, 0, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);
		sprite->Draw(titleText2Image, offX, offY, 0, 0, 1280, 720, WINDOW_WIDTH * d2, WINDOW_HEIGHT * d2);

		if (animFrm % 60 < 40) {
			sprite->SetSrc(0, 0, 344, 64);
			sprite->Draw(titleText3Image, WINDOW_WIDTH / 2 - (sprite->GetSrcWidth()) / 2, WINDOW_HEIGHT * 3 / 4,
				0, 0, sprite->GetSrcWidth(), sprite->GetSrcHeight());
		}
		break;
	case aTransition:
		sprite->Draw(titleBackImage, 0, 0, 0, 0, 1536, 864, WINDOW_WIDTH, WINDOW_HEIGHT);
		sprite->Draw(broomImage, 0, d1, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);
		sprite->Draw(titleText1Image, 0, 0, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);
		sprite->Draw(titleText2Image, offX, offY, 0, 0, 1280, 720, WINDOW_WIDTH * d2, WINDOW_HEIGHT * d2);

		timeRate = animTime / 1.0f;
		rate = ec->easeOutExpo(timeRate);
		float width = (WINDOW_WIDTH + 10) * rate;

		sprite->DrawRect(0, 0, width, WINDOW_HEIGHT, RGB(244, 196, 255));

		timeRate = (animTime - 0.2f) / 1.0f;
		rate = ec->easeOutExpo(timeRate);
		if (timeRate < 0) {
			rate = 0;
		}
		width = (WINDOW_WIDTH + 10) * rate;
		sprite->DrawRect(0, 0, width, WINDOW_HEIGHT, RGB(195, 157, 204));

		timeRate = (animTime - 0.4f) / 1.0f;
		rate = ec->easeOutExpo(timeRate);
		if (timeRate < 0) {
			rate = 0;
		}
		width = (WINDOW_WIDTH + 10) * rate;
		sprite->DrawRect(0, 0, width, WINDOW_HEIGHT, RGB(0, 0, 0));

		if (animTime == 1.1f){
			state = aFinish;
		}
		break;

	}

	//　デバッグ用アニメーションタイム表示
	/*
	char str[64];
	sprintf_s<64>(str, "%.1f", animFrm * (1.0f / 60.0f));
	GameDevice()->m_pFont->Draw(0, 0, str, 50, RGB(255, 255, 255));
	*/
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
