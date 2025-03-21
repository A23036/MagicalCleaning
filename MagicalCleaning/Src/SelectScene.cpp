#include "SelectScene.h"
#include "SceneBase.h"
#include "Player.h"
#include "SelectDisplay.h"
#include "CsvReader.h"

// ---------------------------------------------------------------------------
// コンストラクタ
// ---------------------------------------------------------------------------
SelectScene::SelectScene()
{
	//変数の初期化
	Init();

	//スプライトイメージのロード
	selectBackImage = new CSpriteImage(_T("data/Image/Select/BackImage.png"));
	
	sprite = new CSprite();
	sd = new SelectDisplay();
}

SelectScene::~SelectScene()
{
	SAFE_DELETE(sprite);
	SAFE_DELETE(selectBackImage);
	SAFE_DELETE(csv);
}

void SelectScene::Init()
{
	//定数のCSV読み込み
	CsvLoad();

	for (int i = 0; i < 6; i++) {
		players[i] = new Player(VECTOR3(i * 0.5f - 1.2f, -3, 0), VECTOR3(0, 180 * DegToRad, 0), i);
		initPos[i] = VECTOR3(i * 0.5f - 1.2f, -3, 0);
		players[i]->SetScale(0.6f, 0.6f, 0.6f);
	}
	frm = 0;
	BackFrm = 120;
	scrollX = 0.0f;
}

void SelectScene::Update()
{
	auto di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_TRG, DIK_T)) {
		GameDevice()->selectBGM->Stop();
		SceneManager::ChangeScene("TitleScene");
	}
	if (di->CheckKey(KD_TRG, DIK_P)) {
		GameDevice()->selectBGM->Stop();
		GameDevice()->playBGM->Play();
		SceneManager::ChangeScene("PlayScene");
	}

	if (di->CheckJoy(KD_DAT, 1, 0)) {
		if (frm > BackFrm) { //1Pの一定時間以上戻るボタン入力でタイトルに戻る
			SceneManager::ChangeScene("TitleScene");
			GameDevice()->selectBGM->Stop();
		}
		frm++;
	}
	else {
		frm = 0;
	}
	
	for (int i = 0; i < MAXPLAYER; i++) {
		if (sd->GetIsSelectColor(i)){ //プレイヤーがカラーを選択していたら
			// プレイヤーの位置にキャラクターを移動
			players[sd->GetSelectColor(i)]->SetPosition(VECTOR3(-3.7f + i % 2 * 7.4f, 1.2f + i / 2 * (-3), 0));
			// サイズを拡大
			players[sd->GetSelectColor(i)]->SetScale(1.4f, 1.4f, 1.4f);
		}
		else {
			if (!sd->GetIsSelected(sd->GetSelectColor(i))) { //選択されていないカラーが他のプレイヤーにも選択されていなかったら
				// 初期位置に戻す
				players[sd->GetSelectColor(i)]->SetPosition(initPos[sd->GetSelectColor(i)]);
				// サイズを戻す
				players[sd->GetSelectColor(i)]->SetScale(0.6f, 0.6f, 0.6f);
			}
		}

		if (sd->GetIsReady()) {
			GameDevice()->selectBGM->Stop();
			GameDevice()->playBGM->Play();
			SceneManager::ChangeScene("PlayScene");
		}
	}	
}

void SelectScene::Draw()
{
	// 背景スクロール処理
	sprite->Draw(selectBackImage, int(-scrollX), 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	sprite->Draw(selectBackImage, WINDOW_WIDTH - int(scrollX), 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	//GameDevice()->m_pFont->Draw(400, 20, _T("SELECT SCENE"), 32, RGB(255, 0, 0));

	scrollX += ScrollSpeed;
	
	// スクロール方向を考慮
	if (ScrollSpeed > 0 && scrollX >= WINDOW_WIDTH) {
		scrollX = 0;
	}
	else if (ScrollSpeed < 0 && scrollX <= 0) {
		scrollX = WINDOW_WIDTH;
	}

	if (frm != 0) {
		float rate = (float)frm / (float)BackFrm;
		float width = 150 * rate;
		sprite->DrawRect(20, 90, 150, 20, RGB(120,120,120));
		sprite->DrawRect(20, 90, width, 20, RGB(200, 20, 20));
	}
	

	GameDevice()->m_mView = XMMatrixLookAtLH(
		VECTOR3(0, 0, -10), // カメラ位置
		VECTOR3(0, 0, 0), // 注視点
		VECTOR3(0, 1, 0)); // 上ベクトル
}

void SelectScene::CsvLoad()
{
	// csvからデータ読み込み

	CsvSelectDataLoader dataLoader("data/csv/AnimParam.csv");
	dataLoader.Load();
	selectParams = dataLoader.GetSelectParams();

	//読み込んだデータの代入
	ScrollSpeed = selectParams.ScrollSpeed;
	BackFrm = selectParams.BackFrm;

	/*
	csv = new CsvReader("data/csv/AnimParam.csv");
	if (csv->GetLines() < 1) {
		MessageBox(NULL, "AnimParam.csvが読めません", "エラー", MB_OK);
	}

	for (int i = 1; i < csv->GetLines(); i++) { //CSVファイルから設定の読み込み
		if (csv->GetString(i, 0) == "Select") {
			
			if (csv->GetString(i, 1) == "ScrollSpeed") {	// 背景スクロールスピード
				ScrollSpeed = csv->GetFloat(i, 3);
			}
		}
	}
	*/
}
