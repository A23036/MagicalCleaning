#include "SelectScene.h"
#include "SceneBase.h"
#include "SelectDisplay.h"
#include "Player.h"

SelectScene::SelectScene()
{
	CsvLoad();
	sprite = new CSprite();
	selectBackImage = new CSpriteImage(_T("data/Image/Select/BackImage.png"));

	for (int i = 0; i < 6; i++) {
		players[i] = new Player(VECTOR3(i*0.5f - 1.2f, -3, 0), VECTOR3(0, 180 * DegToRad, 0), i);
		initPos[i] = VECTOR3(i * 0.5f - 1.2f, -3, 0);
		players[i]->SetScale(0.6f, 0.6f, 0.6f);
	}

	sd = new SelectDisplay();

	scrollX = 0.0f;
}

SelectScene::~SelectScene()
{
}

void SelectScene::Update()
{
	auto di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_TRG, DIK_T)) {
		SceneManager::ChangeScene("TitleScene");
	}
	if (di->CheckKey(KD_TRG, DIK_P)) {
		SceneManager::ChangeScene("ViewMapScene");
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
			SceneManager::ChangeScene("PlayScene");
		}
	}	
}

void SelectScene::Draw()
{
	// 背景スクロール処理
	sprite->Draw(selectBackImage, int(-scrollX), 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	sprite->Draw(selectBackImage, WINDOW_WIDTH - int(scrollX), 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	GameDevice()->m_pFont->Draw(400, 20, _T("SELECT SCENE"), 32, RGB(255, 0, 0));

	scrollX += ScrollSpeed;
	
	// スクロール方向を考慮
	if (ScrollSpeed > 0 && scrollX >= WINDOW_WIDTH) {
		scrollX = 0;
	}
	else if (ScrollSpeed < 0 && scrollX <= 0) {
		scrollX = WINDOW_WIDTH;
	}

	GameDevice()->m_mView = XMMatrixLookAtLH(
		VECTOR3(0, 0, -10), // カメラ位置
		VECTOR3(0, 0, 0), // 注視点
		VECTOR3(0, 1, 0)); // 上ベクトル
}

void SelectScene::CsvLoad()
{
	// csvからデータ読み込み
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
}
