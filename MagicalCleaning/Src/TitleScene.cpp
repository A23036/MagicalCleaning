#include "TitleScene.h"
#include "GameMain.h"
#include "EasingCalc.h"
#include "DataCarrier.h"

// ---------------------------------------------------------------------------
// コンストラクタ
// ---------------------------------------------------------------------------
TitleScene::TitleScene()
{
	//変数の初期化
	Init();

	//スプライトイメージのロード
	titleBackImage	= new CSpriteImage(_T("data/Image/Title/title.png"));
	broomImage		= new CSpriteImage(_T("data/Image/Title/Broom.png"));
	titleText1Image = new CSpriteImage(_T("data/Image/Title/Text1.png"));
	titleText2Image = new CSpriteImage(_T("data/Image/Title/Text2.png"));
	titleText3Image = new CSpriteImage(_T("data/Image/Title/Text3.png"));
	
	sprite = new CSprite();
	ec = new EasingCalc();
	dc = SingleInstantiate<DataCarrier>(); //DataCarrierはシングルトンで生成(シーン遷移で消えないため)
}

// ---------------------------------------------------------------------------
// デストラクタ
// ---------------------------------------------------------------------------
TitleScene::~TitleScene()
{
	SAFE_DELETE(titleBackImage);
	SAFE_DELETE(broomImage);
	SAFE_DELETE(titleText1Image);
	SAFE_DELETE(titleText2Image);
	SAFE_DELETE(titleText3Image);
	SAFE_DELETE(sprite);
	SAFE_DELETE(ec);
}

// ---------------------------------------------------------------------------
// 各変数の初期化処理
// ---------------------------------------------------------------------------
void TitleScene::Init()
{
	//定数のCSV読み込み
	CsvLoad();

	state = aFadeIn;
	animFrm = 0;
	animTime = 0.0f;

	timeRate = 0.0f;
	rate = 0.0f;

	alpha = 0.0f;
	rot = 0.0f;
	scale = 0.0f;
	trans = 0.0f;

	moveY = 0.0f;
	moveScale = 0.0f;
	offX = 0.0f;
	offY = 0.0f;
}

// ---------------------------------------------------------------------------
// タイトル画面の更新処理
// 
// キー/ボタン入力でのシーン遷移を行う
// ---------------------------------------------------------------------------
void TitleScene::Update()
{
	auto di = GameDevice()->m_pDI;

	if (state == aWait) {
		// 2024.10.27 コードの可読性向上とコントローラー入力対応
		if ((di->CheckKey(KD_TRG, DIK_S) || di->CheckJoy(KD_TRG, 2)) && state != aTransition){
			state = aTransition;
			GameDevice()->gameStartSE->Play();
			animFrm = 0;
		}
		if (di->CheckKey(KD_TRG, DIK_P)) {
			GameDevice()->titleBGM->Stop();
			GameDevice()->playBGM->Play();
			SceneManager::ChangeScene("PlayScene");
		}
	}
	else if ((di->CheckKey(KD_TRG, DIK_S) || di->CheckKey(KD_TRG, DIK_P) || di->CheckJoy(KD_TRG, 2)) && state != aTransition){
		state = aWait;
		GameDevice()->titleBGM->Play(AUDIO_LOOP);
	}

	if (state == aFinish){
		GameDevice()->titleBGM->Stop();
		GameDevice()->selectBGM->Play(AUDIO_LOOP);
		SceneManager::ChangeScene("SelectScene");
	}
}

// ---------------------------------------------------------------------------
// タイトル画面の描画処理
// 
// タイトル画面でのスプライト描画、アニメーション処理を行う
// ---------------------------------------------------------------------------
void TitleScene::Draw()
{
	//アニメーション時間のフレーム->秒変換
	animTime = animFrm * (1.0f / 60.0f);

	//アニメーション状態ごとの画面描画
	switch (state) {
	case aFadeIn:
		DrawFadeIn();
		break;
	case aBroom:
		DrawBroom();
		break;
	case aText1:
		DrawText1();
		break;
	case aText2:
		DrawText2();
		break;
	case aWait:
		DrawWait();
		break;
	case aTransition:
		DrawTransition();
		break;
	}

	animFrm++; //アニメーションフレームを加算
}

// ---------------------------------------------------------------------------
// 背景フェードイン処理
// ---------------------------------------------------------------------------
void TitleScene::DrawFadeIn()
{
	timeRate = animTime / AnimTime[aFadeIn];
	rate = timeRate; //線形補完

	alpha = (AlphaGoal - AlphaStart) * rate + AlphaStart;

	sprite->Draw(titleBackImage, 0, 0, 0, 0, 1536, 864, WINDOW_WIDTH, WINDOW_HEIGHT, alpha);
	if (animTime >= AnimTime[aFadeIn]) {
		state = aBroom;
		GameDevice()->swingSE->Play();
		animFrm = 0;
	}
}

// ---------------------------------------------------------------------------
// 箒スプライトのアニメーション表示処理
// ---------------------------------------------------------------------------
void TitleScene::DrawBroom()
{
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
		GameDevice()->downSE->Play();
		state = aText1;
		animFrm = 0;
	}
}

// ---------------------------------------------------------------------------
// テキスト「まほうつかいの」アニメーション表示処理
// ---------------------------------------------------------------------------
void TitleScene::DrawText1()
{
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
}

// ---------------------------------------------------------------------------
// テキスト「おそうじ大作戦」アニメーション表示処理
// ---------------------------------------------------------------------------
void TitleScene::DrawText2()
{
	sprite->Draw(titleBackImage, 0, 0, 0, 0, 1536, 864, WINDOW_WIDTH, WINDOW_HEIGHT);
	sprite->Draw(broomImage, 0, 0, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);
	sprite->Draw(titleText1Image, 0, 0, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);

	timeRate = animTime / AnimTime[aText2];
	rate = ec->easeOutBounce(timeRate);
	scale = (ScaleGoal - ScaleStart) * rate + ScaleStart;
	trans = 1 - scale;

	if (animTime == 0.3f) {
		GameDevice()->scaleUpSE->Play();
	}

	sprite->Draw(titleText2Image, WINDOW_WIDTH / 2 * trans, WINDOW_HEIGHT / 2 * trans,
		0, 0, 1280, 720, WINDOW_WIDTH * scale, WINDOW_HEIGHT * scale);
	if (animTime >= AnimTime[aText2])
	{
		state = aWait;
		GameDevice()->titleBGM->Play(AUDIO_LOOP);
		animFrm = 0;
	}
}

// ---------------------------------------------------------------------------
// アニメーション終了、プレイヤーの入力待機中描画処理
// ---------------------------------------------------------------------------
void TitleScene::DrawWait()
{
	sprite->Draw(titleBackImage, 0, 0, 0, 0, 1536, 864, WINDOW_WIDTH, WINDOW_HEIGHT);

	moveY = sinf(animFrm / 120.0f * XM_2PI) * 5.0f;
	moveScale = 1 + fabs(sinf(animFrm / 300.0f * XM_2PI) * 0.05f);

	// 中央で拡縮するためのオフセットを計算
	offX = (WINDOW_WIDTH * (1 - moveScale)) / 2;
	offY = (WINDOW_HEIGHT * (1 - moveScale)) / 2;

	sprite->Draw(broomImage, 0, moveY, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);
	sprite->Draw(titleText1Image, 0, 0, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);
	sprite->Draw(titleText2Image, offX, offY, 0, 0, 1280, 720, WINDOW_WIDTH * moveScale, WINDOW_HEIGHT * moveScale);

	if (animFrm % 60 < 40) {
		sprite->SetSrc(0, 0, 344, 64);
		sprite->Draw(titleText3Image, WINDOW_WIDTH / 2 - (sprite->GetSrcWidth()) / 2, WINDOW_HEIGHT * 3 / 4,
			0, 0, sprite->GetSrcWidth(), sprite->GetSrcHeight());
	}
}

// ---------------------------------------------------------------------------
// シーン遷移時のトランジション処理
// ---------------------------------------------------------------------------
void TitleScene::DrawTransition()
{
	sprite->Draw(titleBackImage, 0, 0, 0, 0, 1536, 864, WINDOW_WIDTH, WINDOW_HEIGHT);
	sprite->Draw(broomImage, 0, moveY, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);
	sprite->Draw(titleText1Image, 0, 0, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);
	sprite->Draw(titleText2Image, offX, offY, 0, 0, 1280, 720, WINDOW_WIDTH * moveScale, WINDOW_HEIGHT * moveScale);

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

	if (animTime == 1.1f) {
		state = aFinish;
	}
}


// ---------------------------------------------------------------------------
// CSV読み込み処理
// 
// アニメーション処理に使用する定数のCSV読み込みを行う
// ---------------------------------------------------------------------------
void TitleScene::CsvLoad()
{
	CsvTitleDataLoader dataLoader("data/csv/AnimParam.csv");
	dataLoader.Load();
	titleParams = dataLoader.GetTitleParams();

	//読み込んだ構造体からデータの代入
	for (int i = 0; i < 4; i++) {
		AnimTime[i] = titleParams.AnimTime[i];
	}
	ScaleStart = titleParams.ScaleStart;
	ScaleGoal  = titleParams.ScaleGoal;
	RotateStart = titleParams.RotateStart;
	RotateGoal = titleParams.RotateGoal;
	TransStart = titleParams.TransStart;
	TransGoal = titleParams.TransGoal;
	AlphaStart = titleParams.AlphaStart;
	AlphaGoal = titleParams.AlphaGoal;
}
