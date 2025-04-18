#include "PlayDisplay.h"
#include "PlayScene.h"
#include "Player.h"
#include "DataCarrier.h"
#include "EasingCalc.h"

//定数定義
namespace {
	//アニメーション用定数
	static const float ROT_START = 0.0f;
	static const float ROT_GOAL = 360.0f * DegToRad;
	static const float SCALE_START = 0.0f;
	static const float SCALE_GOAL = 2.0f;

	//能力UI位置座標定数
	static const int ICON_POS_BASE = 64;
	static const int TEXT_POS_BASE = 68;

	//プレイヤー初期化用定数
	static const int NOT_PLAYER = -1;
}

// ---------------------------------------------------------------------------
// コンストラクタ
// ---------------------------------------------------------------------------
PlayDisplay::PlayDisplay()
{
	SetPriority(-9999); 	// 最後より一つ前に処理する
	SetDrawOrder(-10000);	// 一番最後に描画する

	//変数の初期化
	Init();

	countDownImage = new CSpriteImage(_T("data/Image/Play/CountDown.png"));
	playUiImage = new CSpriteImage(_T("data/Image/Play/UISprite.png"));
	playUiImage2 = new CSpriteImage(_T("data/Image/Play/UISprite2.png"));

	sprite = new CSprite();
}

// ---------------------------------------------------------------------------
// デストラクタ
// ---------------------------------------------------------------------------
PlayDisplay::~PlayDisplay()
{
	SAFE_DELETE(countDownImage);
	SAFE_DELETE(playUiImage);
	SAFE_DELETE(playUiImage2);
	SAFE_DELETE(sprite);
}

// ---------------------------------------------------------------------------
// 開始時に呼ばれる関数
// 
// オブジェクトを生成後、最初にUpdate()の前に呼ばれる
// ---------------------------------------------------------------------------
void PlayDisplay::Start()
{
	dc = ObjectManager::FindGameObject<DataCarrier>();
	ec = ObjectManager::FindGameObject<EasingCalc>();
}

// ---------------------------------------------------------------------------
// 各変数の初期化処理
// ---------------------------------------------------------------------------
void PlayDisplay::Init()
{
	animFrm = 0;
	animTime = 0;
	isLastSpurt = false;

	for (int i = 0; i < 4; i++) {
		msIconPosX[i] = 0;
		jnIconPosX[i] = ICON_POS_BASE;
		asIconPosX[i] = ICON_POS_BASE * 2;
		arIconPosX[i] = ICON_POS_BASE * 3;
		cwIconPosX[i] = ICON_POS_BASE * 4;

		msTextPosX[i] = 0;
		jnTextPosX[i] = TEXT_POS_BASE;
		asTextPosX[i] = TEXT_POS_BASE * 2;
		arTextPosX[i] = TEXT_POS_BASE * 3;
		cwTextPosX[i] = TEXT_POS_BASE * 4;

		for (int j = 0; j < MAXPLAYER; j++) {
			isMaxLv[i][j] = false;
		}
		isPlaySound[i] = false;
		blowPlayerList[i] = NOT_PLAYER;
		blowAnimFrm[i] = 0;
	}
}

// ---------------------------------------------------------------------------
// プレイ画面UIの更新処理
// 
// UI描画のためのゲーム状態、経過時間の取得を行う
// ---------------------------------------------------------------------------
void PlayDisplay::Update()
{
	animTime = (1.0f / 60.0f) * animFrm;

	gameState = dc->GetGameState();
	gameTime = dc->GetGameTime();

	if (animTime >= 1.0f && gameState != sFinish) {
		animFrm = 0;
	}
	animFrm++;
}

// ---------------------------------------------------------------------------
// プレイ画面UIの描画処理
// 
// 各プレイヤー画面ごとにそれぞれのUI描画を行う
// ---------------------------------------------------------------------------
void PlayDisplay::Draw()
{
	//スプライトの透明度の初期化
	sprite->m_vDiffuse = VECTOR4(1, 1, 1, 1.0f);

	//アニメーションUI表示
	DrawAnimationUI();

	//プレイヤーごとの基本UI表示
	std::string s = "Player";
	int n = ObjectManager::DrawCounter();
	s = s + std::to_string(n + 1);
	Player* pl = ObjectManager::FindGameObjectWithTag<Player>(s);
	int plNum = pl->GetPlayerNum();

	//基本UI描画
	DrawBaseUI(pl);

	//透明化時、残り時間表示
	if (pl->GetIsInvisible()) {
		float time = pl->GetCurInvisibleTime() / 60.0f;
		float rate = (pl->GetInvisibleTime() - time) / pl->GetInvisibleTime();
		float width = 150 * rate;
		sprite->DrawRect(WINDOW_WIDTH/2 - 75, WINDOW_HEIGHT/2 - 60, width,20, RGB(220, 20, 20));
	}

	//吹っ飛ばされた、UIアニメーションUI再生
	if (blowPlayerList[plNum] != -1) {
		//アイコンの設定
		sprite->SetSrc(playUiImage, 256 + 72 * dc->GetColor(blowPlayerList[plNum]), 624, 68, 64);
		//アイコンアニメーション
		DrawBlowPlayer(blowPlayerList[plNum], plNum);
	}
}

// ---------------------------------------------------------------------------
// プレイ画面アニメーションUIの描画処理
// 
// 各プレイヤー画面ごとにアニメーションUIの描画を行う
// ---------------------------------------------------------------------------
void PlayDisplay::DrawAnimationUI()
{
	//開始待機中
	if (gameState == sReady) {
		//カウントダウン表示
		DrawCountDown();
	}

	//ゲームプレイ中
	if (gameState == sGamePlay) {
		//ラストスパート時UI表示
		DrawLastSpurt();

		if (gameTime < 10 && gameTime != 0) {
			//終了カウントダウンアニメーション表示
			DrawFinishCount();
		}
	}
	sprite->m_vDiffuse = VECTOR4(1, 1, 1, 1.0f);

	//終了時
	if (gameState == sFinish) {
		DrawFinish();
	}
}

// ---------------------------------------------------------------------------
// プレイ画面アニメーションUIの描画処理サブ関数
// 
// バトル開始前カウントダウンのアニメーション描画を行う
// ---------------------------------------------------------------------------
void PlayDisplay::DrawCountDown()
{
	if (gameTime == 0) {
		sprite->SetSrc(countDownImage, 132, 0, 66, 66);
		if (!isPlaySound[0]) {
			GameDevice()->countDownSE->Play();
			isPlaySound[0] = true;
		}
	}
	if (gameTime == 1) {
		sprite->SetSrc(countDownImage, 66, 0, 66, 66);
		if (!isPlaySound[1]) {
			GameDevice()->countDownSE->Play();
			isPlaySound[1] = true;
		}
	}
	if (gameTime == 2) {
		sprite->SetSrc(countDownImage, 0, 0, 66, 66);
		if (!isPlaySound[2]) {
			GameDevice()->countDownSE->Play();
			isPlaySound[2] = true;
		}
	}
	if (gameTime == 3) {
		sprite->SetSrc(countDownImage, 198, 0, 186, 66);
		if (!isPlaySound[3]) {
			GameDevice()->whistleSE->Play();
			isPlaySound[3] = true;
		}
	}
	int width, height;
	VECTOR2 center;
	width = sprite->GetSrcWidth();
	height = sprite->GetSrcHeight();
	center.x = WINDOW_WIDTH / 2;
	center.y = WINDOW_HEIGHT / 2;

	// アニメーションの進行度計算
	float timeRate = animTime / 1.0f;
	float rate = ec->easeOutBack(timeRate); // 滑らかな拡大
	
	float rotation = (ROT_GOAL - ROT_START) * rate + ROT_START;	// 回転
	float scale = (SCALE_GOAL - SCALE_START) * rate + SCALE_START;	// 拡大倍率

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

// ---------------------------------------------------------------------------
// プレイ画面アニメーションUIの描画処理サブ関数
// 
// バトル中のラストスパートのアニメーション描画を行う
// ---------------------------------------------------------------------------
void PlayDisplay::DrawLastSpurt()
{
	if (gameTime == 60) {
		if (!isLastSpurt) {
			GameDevice()->timeSE->Play();
			GameDevice()->playBGM->Stop();
			isLastSpurt = true;
			GameDevice()->spurtBGM->Play();
		}
		sprite->SetSrc(playUiImage, 260, 380, 200, 80);
		int width, height;
		VECTOR2 center;
		width = sprite->GetSrcWidth();
		height = sprite->GetSrcHeight();
		center.x = WINDOW_WIDTH / 2;
		center.y = WINDOW_HEIGHT / 2;

		// アニメーションの進行度計算
		float timeRate = animTime / 1.0f;
		float rate = ec->easeOutExpo(timeRate); // 滑らかな拡大

		float scale = (SCALE_GOAL - SCALE_START) * rate + SCALE_START;	// 拡大倍率

		// 回転中心を画像の中心にするための補正
		float pivotX = width / 2.0f;
		float pivotY = height / 2.0f;

		// ワールド行列の計算
		MATRIX4X4 m = XMMatrixTranslation(-pivotX, -pivotY, 0)
			* XMMatrixScaling(scale, scale, 1.0f)
			* XMMatrixTranslation(center.x, center.y, 0);

		// スプライトをワールド行列を使用して描画
		sprite->m_vDiffuse = VECTOR4(1, 1, 1, 0.6f);
		sprite->Draw(m);
	}
	if (gameTime == 59) {
		sprite->SetSrc(playUiImage, 260, 380, 200, 80, 400, 160);
		int width, height;
		VECTOR2 center;
		width = sprite->GetSrcWidth();
		height = sprite->GetSrcHeight();
		center.x = WINDOW_WIDTH / 2 - width;
		center.y = WINDOW_HEIGHT / 2 - height;

		// アニメーションの進行度計算
		float timeRate = animTime / 1.0f;
		float alpha = -0.6f * timeRate + 0.6f;	// 透明度

		sprite->m_vDiffuse = VECTOR4(1, 1, 1, alpha);
		sprite->Draw(center.x, center.y);
	}
}

// ---------------------------------------------------------------------------
// プレイ画面アニメーションUIの描画処理サブ関数
// 
// バトル終了カウントダウンのアニメーション描画を行う
// ---------------------------------------------------------------------------
void PlayDisplay::DrawFinishCount()
{
	int posX = (gameTime - 1) * 56;
	sprite->SetSrc(playUiImage, 326 + posX, 470, 48, 64);
	int a = 270 + posX;

	int width, height;
	VECTOR2 center;
	width = sprite->GetSrcWidth();
	height = sprite->GetSrcHeight();
	center.x = WINDOW_WIDTH / 2;
	center.y = WINDOW_HEIGHT / 2;

	// アニメーションの進行度計算
	float timeRate = animTime / 1.0f;
	float rate = ec->easeOutBack(timeRate); // 滑らかな拡大

	float rotation = (ROT_GOAL - ROT_START) * rate + ROT_START;	// 回転
	float scale = (SCALE_GOAL - SCALE_START) * rate + SCALE_START;	// 拡大倍率

	// 回転中心を画像の中心にするための補正
	float pivotX = width / 2.0f;
	float pivotY = height / 2.0f;

	// ワールド行列の計算
	MATRIX4X4 m = XMMatrixTranslation(-pivotX, -pivotY, 0)
		* XMMatrixRotationZ(rotation)
		* XMMatrixScaling(scale, scale, 1.0f)
		* XMMatrixTranslation(center.x, center.y, 0);

	sprite->m_vDiffuse = VECTOR4(1, 1, 1, 0.5f);
	// スプライトをワールド行列を使用して描画
	sprite->Draw(m);
}

// ---------------------------------------------------------------------------
// プレイ画面アニメーションUIの描画処理サブ関数
// 
// バトル終了時のアニメーション描画を行う
// ---------------------------------------------------------------------------
void PlayDisplay::DrawFinish()
{
	sprite->SetSrc(playUiImage, 270, 550, 100, 36);

	// アニメーションの進行度計算
	float timeRate = animTime / 1.0f;
	float rate = ec->easeOutExpo(timeRate); // 滑らかな拡大

	float posX = (WINDOW_WIDTH / 2 - sprite->GetSrcWidth() * 4 / 2 + 300) * rate - 300;
	float alpha = 1.0f * rate;

	sprite->Draw(playUiImage, posX, WINDOW_HEIGHT / 2 - 50, 270, 550, 100, 36, 100 * 4, 36 * 4, alpha);
}

// ---------------------------------------------------------------------------
// プレイ画面UIの描画処理
// 
// バトル中の基本UI描画を行う
// ---------------------------------------------------------------------------
void PlayDisplay::DrawBaseUI(Player* pl)
{
	int posX = 0, posY = 0, offX = 0, offY = 0;

	//Leaf、MP、能力表示----------
	//UIベース
	sprite->SetSrc(playUiImage, 0, 112, 412, 48, 412 * 2, 48 * 2);
	sprite->Draw(200, WINDOW_HEIGHT - 130);

	//Leaf表示
	//Leaf所持数を割合ゲージで表示
	float rate;
	rate = (float)pl->GetLeaf() / (float)pl->GetCarWeight();
	posY = 48 * rate;
	sprite->SetSrc(playUiImage, 0, 160 + 48 - posY, 48, 48, 48 * 2, 48 * 2);
	sprite->Draw(200, WINDOW_HEIGHT - 130 + 48 * 2 - posY * 2);

	//Leaf所持数の表示
	int leaf = pl->GetLeaf();
	posX = 228;
	offX = 0;
	if (leaf >= pl->GetCarWeight()) { //所持上限の時赤文字表示
		offX = 140;
	}

	if (leaf >= 10) {
		offX += (leaf / 10) % 10 * 14;
		sprite->SetSrc(playUiImage, 275 + offX, 195, 12, 16, 36, 48);
		sprite->Draw(posX - 16, 658);
		posX += 16;
		offX -= (leaf / 10) % 10 * 14;
	}

	offX += leaf % 10 * 14;
	sprite->SetSrc(playUiImage, 275 + offX, 195, 12, 16, 36, 48);
	sprite->Draw(posX, 658);


	//MP表示
	int mp = pl->GetMP();
	posX = 370;

	if (mp >= 100) {
		offX = (mp / 100) % 10 * 14;
		sprite->SetSrc(playUiImage, 275 + offX, 195, 12, 16, 42, 56);
		sprite->Draw(posX - 44, 667);
		posX += 22;
	}

	if (mp >= 10) {
		offX = (mp / 10) % 10 * 14;
		sprite->SetSrc(playUiImage, 275 + offX, 195, 12, 16, 42, 56);
		sprite->Draw(posX - 22, 667);
		posX += 22;
	}

	offX = mp % 10 * 14;
	sprite->SetSrc(playUiImage, 275 + offX, 195, 12, 16, 42, 56);
	sprite->Draw(posX, 667);

	// 必要MP割合メーター表示
	rate = (float)pl->GetMP() / (float)pl->GetPowerCost(pl->GetSelectPower());
	if (rate >= 1.0f) {
		rate = 1.0f;
	}
	posX = 236 * rate;
	sprite->SetSrc(playUiImage, 172, 166, posX, 4, posX * 2, 4 * 2);
	sprite->Draw(200 + 172 * 2, WINDOW_HEIGHT - 130 + 80);

	//選択中パワー描画(アイコンと文字)

	int plNum = pl->GetPlayerNum();
	//次の能力強化が最終強化の時アイコンを変更
	for (int i = 0; i < 5; i++) {
		if (pl->GetPowerLv(i) == pl->GetMaxPowerLv(i) - 2 && !isMaxLv[i][plNum]) { //次の強化が能力レベルMAX
			switch (i) {
			case pMS:
				msIconPosX[plNum] += 320;
				msTextPosX[plNum] += 340;
				break;
			case pJN:
				jnIconPosX[plNum] += 320;
				jnTextPosX[plNum] += 340;
				break;
			case pAS:
				asIconPosX[plNum] += 320;
				asTextPosX[plNum] += 340;
				break;
			case pAR:
				arIconPosX[plNum] += 320;
				arTextPosX[plNum] += 340;
				break;
			case pCW:
				cwIconPosX[plNum] += 320;
				cwTextPosX[plNum] += 340;
				break;
			}
			isMaxLv[i][plNum] = true;
		}
	}

	switch (pl->GetSelectPower()) {
	case pMS:
		sprite->SetSrc(playUiImage, msIconPosX[plNum], 0, 64, 64, 90, 90);	//アイコン
		sprite->Draw(680, 610);
		sprite->SetSrc(playUiImage, msTextPosX[plNum], 68, 68, 40, 68 * 2.4, 40 * 2.4); //文字
		sprite->Draw(780, 606);
		sprite->m_vDiffuse = VECTOR4(1, 1, 1, 0.6f);			//左右のパワーは半透明
		sprite->SetSrc(playUiImage, cwIconPosX[plNum], 0, 64, 64, 40, 40);	//左のパワー
		sprite->Draw(580, 660);
		sprite->SetSrc(playUiImage, jnIconPosX[plNum], 0, 64, 64, 40, 40);		//右のパワー
		sprite->Draw(970, 660);
		sprite->m_vDiffuse = VECTOR4(1, 1, 1, 1);				//透明度を戻す
		break;
	case pJN:
		sprite->SetSrc(playUiImage, jnIconPosX[plNum], 0, 64, 64, 90, 90);
		sprite->Draw(680, 610);
		sprite->SetSrc(playUiImage, jnTextPosX[plNum], 68, 68, 40, 68 * 2.4, 40 * 2.4);
		sprite->Draw(780, 606);
		sprite->m_vDiffuse = VECTOR4(1, 1, 1, 0.6f);
		sprite->SetSrc(playUiImage, msIconPosX[plNum], 0, 64, 64, 40, 40);
		sprite->Draw(580, 660);
		sprite->SetSrc(playUiImage, asIconPosX[plNum], 0, 64, 64, 40, 40);
		sprite->Draw(970, 660);
		sprite->m_vDiffuse = VECTOR4(1, 1, 1, 1);
		break;
	case pAS:
		sprite->SetSrc(playUiImage, asIconPosX[plNum], 0, 64, 64, 90, 90);
		sprite->Draw(680, 610);
		sprite->SetSrc(playUiImage, asTextPosX[plNum], 68, 68, 40, 68 * 2.4, 40 * 2.4);
		sprite->Draw(780, 606);
		sprite->m_vDiffuse = VECTOR4(1, 1, 1, 0.6f);
		sprite->SetSrc(playUiImage, jnIconPosX[plNum], 0, 64, 64, 40, 40);
		sprite->Draw(580, 660);
		sprite->SetSrc(playUiImage, arIconPosX[plNum], 0, 64, 64, 40, 40);
		sprite->Draw(970, 660);
		sprite->m_vDiffuse = VECTOR4(1, 1, 1, 1);
		break;
	case pAR:
		sprite->SetSrc(playUiImage, arIconPosX[plNum], 0, 64, 64, 90, 90);
		sprite->Draw(680, 610);
		sprite->SetSrc(playUiImage, arTextPosX[plNum], 68, 68, 40, 68 * 2.4, 40 * 2.4);
		sprite->Draw(780, 606);
		sprite->m_vDiffuse = VECTOR4(1, 1, 1, 0.6f);
		sprite->SetSrc(playUiImage, asIconPosX[plNum], 0, 64, 64, 40, 40);
		sprite->Draw(580, 660);
		sprite->SetSrc(playUiImage, cwIconPosX[plNum], 0, 64, 64, 40, 40);
		sprite->Draw(970, 660);
		sprite->m_vDiffuse = VECTOR4(1, 1, 1, 1);
		break;
	case pCW:
		sprite->SetSrc(playUiImage, cwIconPosX[plNum], 0, 64, 64, 90, 90);
		sprite->Draw(680, 610);
		sprite->SetSrc(playUiImage, cwTextPosX[plNum], 68, 68, 40, 68 * 2.4, 40 * 2.4);
		sprite->Draw(780, 606);
		sprite->m_vDiffuse = VECTOR4(1, 1, 1, 0.6f);
		sprite->SetSrc(playUiImage, arIconPosX[plNum], 0, 64, 64, 40, 40);
		sprite->Draw(580, 660);
		sprite->SetSrc(playUiImage, msIconPosX[plNum], 0, 64, 64, 40, 40);
		sprite->Draw(970, 660);
		sprite->m_vDiffuse = VECTOR4(1, 1, 1, 1);
		break;
	}

	//能力レベル表示
	int level;
	level = pl->GetPowerLv(pl->GetSelectPower());
	if (level == pl->GetMaxPowerLv(pl->GetSelectPower()) - 1) { //能力レベルMAX
		//ベース
		sprite->SetSrc(playUiImage, 275, 175, 34, 16, 34 * 2, 16 * 2);
		sprite->Draw(510, 620);
		//「MAX」文字
		sprite->SetSrc(playUiImage, 383, 175, 34, 16, 34 * 2, 16 * 2);
		sprite->Draw(580, 620);
	}
	else {
		//ベース
		sprite->SetSrc(playUiImage, 275, 175, 34, 16, 34 * 2, 16 * 2);
		sprite->Draw(530, 620);
		//レベルに応じた数字
		posX = level * 14;
		sprite->SetSrc(playUiImage, 289 + posX, 195, 12, 16, 12 * 2, 16 * 2);
		sprite->Draw(600, 620);
	}

	//MPコスト表示
	//「COST」文字
	sprite->SetSrc(playUiImage, 325, 175, 56, 16, 56 * 2, 16 * 2);
	sprite->Draw(1036, 620);

	//能力レベルがMAXでないとき
	if (pl->GetMaxPowerLv(pl->GetSelectPower()) > pl->GetPowerLv(pl->GetSelectPower()) + 1) {
		//所持MP(MAX:必要コスト)
		int mp = pl->GetMP();
		if (mp >= pl->GetPowerCost(pl->GetSelectPower())) { //MPは必要コストを超えないようにする
			mp = pl->GetPowerCost(pl->GetSelectPower());
			offX = 0;
		}
		else {
			offX = 140;
		}

		if (mp / 10 > 0) { //MPが10より大きいとき
			posX = mp / 10 * 14;
			sprite->SetSrc(playUiImage, 275 + posX + offX, 195, 12, 16, 12 * 2, 16 * 2);
			sprite->Draw(1030, 660);
		}
		posX = mp % 10 * 14;
		sprite->SetSrc(playUiImage, 275 + posX + offX, 195, 12, 16, 12 * 2, 16 * 2);
		sprite->Draw(1050, 660);

		offX = 0;

		//「/」表示
		sprite->SetSrc(playUiImage, 313, 175, 10, 16, 10 * 2, 16 * 2);
		sprite->Draw(1076, 680);


		//必要コスト
		int cost = pl->GetPowerCost(pl->GetSelectPower());
		offX = 20;
		if (cost / 10 > 0) { //コストが10より大きいとき
			posX = cost / 10 * 14;
			sprite->SetSrc(playUiImage, 275 + posX, 195, 12, 16, 12 * 2, 16 * 2);
			sprite->Draw(1100, 700);
			offX = 0;
		}
		posX = cost % 10 * 14;
		sprite->SetSrc(playUiImage, 275 + posX, 195, 12, 16, 12 * 2, 16 * 2);
		sprite->Draw(1120 - offX, 700);
	}

	//スコア表示
	sprite->SetSrc(64, 176, 200, 40, 200 * 1.5, 40 * 1.5);
	sprite->Draw(WINDOW_WIDTH / 2 - sprite->GetSrcWidth() - 30, 22);

	int score = pl->GetScore();
	posX = WINDOW_WIDTH / 2 + 150;
	offX = (score / 100) % 10 * 56;
	sprite->SetSrc(playUiImage, 270 + offX, 470, 48, 64, 48, 64);
	sprite->Draw(posX - 48, 20);

	offX = (score / 10) % 10 * 56;
	sprite->SetSrc(playUiImage, 270 + offX, 470, 48, 64, 48, 64);
	sprite->Draw(posX, 20);

	offX = score % 10 * 56;
	sprite->SetSrc(playUiImage, 270 + offX, 470, 48, 64, 48, 64);
	sprite->Draw(posX + 48, 20);


	//順位表示
	posY = 590;
	switch (pl->GetPlayerNum()) { //プレイヤーごとの表示位置変更
	case 0:
	case 2:
		posX = 20;
		break;
	case 1:
	case 3:
		posX = 1200;
		break;
	}

	switch (dc->GetRank(pl->GetPlayerNum())) { //順位ごとのスプライト変更
	case 1:
		sprite->SetSrc(playUiImage, 2, 225, 146, 150, 146, 150);
		break;
	case 2:
		sprite->SetSrc(playUiImage, 152, 225, 146, 150, 146, 150);
		break;
	case 3:
		sprite->SetSrc(playUiImage, 301, 225, 146, 150, 146, 150);
		break;
	case 4:
		sprite->SetSrc(playUiImage, 452, 225, 146, 150, 146, 150);
		break;
	}
	sprite->Draw(posX, posY);

	//レーダーチャート表示
	//ベース
	sprite->SetSrc(playUiImage, 0, 375, 250, 225);
	switch (pl->GetPlayerNum()) { //プレイヤーごとの表示位置変更
	case 0:
	case 2:
		posX = 40;
		break;
	case 1:
	case 3:
		posX = 1070;
		break;
	}
	sprite->m_vDiffuse = VECTOR4(1, 1, 1, 0.8f); //半透明で表示
	sprite->Draw(posX, 40);
	sprite->m_vDiffuse = VECTOR4(1, 1, 1, 1.0f); //透明度を戻す

	//能力ごとの進捗表示
	//中心位置 posX,posY
	switch (pl->GetPlayerNum()) { //プレイヤーごとの表示位置変更
	case 0:
	case 2:
		posX = 164;
		break;
	case 1:
	case 3:
		posX = 1195;
		break;
	}
	posY = 158;

	//点のカラー格納用配列
	DWORD color[5];

	//表示座標オフセット
	int _offX[5], _offY[5];

	//能力進捗保存用変数
	int num = 0;
	int power1 = -1, power2 = -1;
	float max1 = -1, max2 = -1;
	bool isAllRounder = false;

	for (int i = 0; i < 5; i++) {
		// 現在の進捗
		float progress = (float)(pl->GetPowerLv(i) + 1) / (float)pl->GetMaxPowerLv(i);

		switch (i) {
		case 0: //移動速度
			_offX[0] = 0;
			_offY[0] = -progress * 84;
			color[0] = RGB(55, 49, 245);
			break;
		case 1: //ジャンプ回数
			_offX[1] = progress * 80;
			_offY[1] = -progress * 28;
			color[1] = RGB(245, 49, 238);
			break;
		case 2: //攻撃速度
			_offX[2] = progress * 48;
			_offY[2] = progress * 66;
			color[2] = RGB(238, 245, 49);
			break;
		case 3: //攻撃範囲
			_offX[3] = -progress * 48;
			_offY[3] = progress * 66;
			color[3] = RGB(245, 55, 49);
			break;
		case 4: //手持ちリーフ
			_offX[4] = -progress * 80;
			_offY[4] = -progress * 28;
			color[4] = RGB(49, 245, 55);
			break;
		}

		//強化進捗上位2つの能力を保存
		if (progress > max1) {	// 最大進捗
			max2 = max1;		// 現在の最大を2番目に
			power2 = power1;	// 現在の最大能力を2番目に
			max1 = progress;	// 新しい最大進捗
			power1 = i;			// 新しい最大能力
		}
		else if (progress > max2) { // 2番目に大きい進捗
			max2 = progress;
			power2 = i;
		}
		if (progress > 0.5) { //5割を超えた強化をした能力の数をカウント
			num++;
		}
	}
	if (num == 5) { //5つの能力全ての強化進捗が八割を超えていたら「オールラウンダー」
		isAllRounder = true;
	}

	//各点間の線の描画
	for (int i = 0; i < 5; i++) {
		if (i + 1 < 5) {
			sprite->DrawLine(posX + 2 + _offX[i], posY + 3 + _offY[i], posX + 2 + _offX[i + 1], posY + 3 + _offY[i + 1], 2, RGB(255, 0, 0));
		}
		else {
			sprite->DrawLine(posX + 2 + _offX[i], posY + 3 + _offY[i], posX + 2 + _offX[0], posY + 3 + _offY[0], 2, RGB(255, 0, 0));
		}
	}

	//各点の描画
	for (int i = 0; i < 5; i++) {
		sprite->DrawRect(posX + _offX[i], posY + _offY[i], 6, 6, color[i]);
	}

	//能力強化状態に応じた二つ名表示
	switch (pl->GetPlayerNum()) { //プレイヤーごとの表示位置変更
	case 0:
	case 2:
		posX = 30;
		break;
	case 1:
	case 3:
		posX = 1060;
		break;
	}
	//能力の強化進捗順に二つ
	if (isAllRounder) { //「オールラウンダー」
		sprite->DrawRect(posX - 10, 268, 300, 40, RGB(255, 255, 255), 0.5f);
		sprite->SetSrc(playUiImage2, 0, 4, 280, 36, 280, 36);
		sprite->Draw(posX, 270);
	}
	else if ((max1 - max2) > 0.4) { //ひとつの能力が突出しているとき
		sprite->DrawRect(posX - 10, 268, 300, 40, RGB(255, 255, 255), 0.5f);
		switch (power1) {
		case 0: //移動速度
			sprite->SetSrc(playUiImage2, 0, 44, 280, 36, 280, 36);
			break;
		case 1: //ジャンプ回数
			sprite->SetSrc(playUiImage2, 0, 244, 280, 36, 280, 36);
			break;
		case 2: //攻撃速度
			sprite->SetSrc(playUiImage2, 0, 404, 280, 36, 280, 36);
			break;
		case 3: //攻撃範囲
			sprite->SetSrc(playUiImage2, 0, 524, 280, 36, 280, 36);
			break;
		case 4: //手持ちリーフ
			sprite->SetSrc(playUiImage2, 0, 604, 280, 36, 280, 36);
			break;
		}
		sprite->Draw(posX, 270);
	}
	else if (max2 > 0.2f) {
		sprite->DrawRect(posX - 10, 268, 300, 40, RGB(255, 255, 255), 0.5f);
		switch (min(power1, power2)) {
		case 0: //移動速度
			offY = (max(power1, power2)) * 40;
			sprite->SetSrc(playUiImage2, 0, 44 + offY, 280, 36, 280, 36);
			break;
		case 1: //ジャンプ回数
			offY = (max(power1, power2) - 1) * 40;
			sprite->SetSrc(playUiImage2, 0, 244 + offY, 280, 36, 280, 36);
			break;
		case 2: //攻撃速度
			offY = (max(power1, power2) - 2) * 40;
			sprite->SetSrc(playUiImage2, 0, 404 + offY, 280, 36, 280, 36);
			break;
		case 3: //攻撃範囲
			offY = (max(power1, power2) - 3) * 40;
			sprite->SetSrc(playUiImage2, 0, 524 + offY, 280, 36, 280, 36);
			break;
		case 4: //手持ちリーフ
			offY = (max(power1, power2) - 4) * 40;
			sprite->SetSrc(playUiImage2, 0, 650 + offY, 280, 36, 280, 36);
			break;
		}
		sprite->Draw(posX, 270);
	}

	//アイテム所持アイコン表示
	if (pl->GetItemNum() != -1) {
		switch (pl->GetItemNum()) {
		case 0:
			sprite->SetSrc(playUiImage, 289, 593, 30, 30, 90, 90);
			if (pl->GetPlayerNum() % 2) { //画面位置によって表示位置を変更
				sprite->Draw(1260, 400);
			}
			else {
				sprite->Draw(20, 400);
			}
			break;
		default:
			break;
		}
	}
}

// ---------------------------------------------------------------------------
// 吹っ飛ばされたプレイヤーのセット関数
// 
// 吹っ飛ばされたプレイヤー番号をblowPlayerListに登録する
// ---------------------------------------------------------------------------
void PlayDisplay::SetBlowPlayer(int atkPlayerNum, int blowPlayerNum)
{
	blowPlayerList[blowPlayerNum] = atkPlayerNum;
}

// ---------------------------------------------------------------------------
// 吹っ飛ばされたプレイヤーのアニメーション描画関数
// 
// 吹っ飛ばされたプレイヤーのアイコンのアニメーション描画を行う
// ---------------------------------------------------------------------------
void PlayDisplay::DrawBlowPlayer(int atkPlayerNum, int blowPlayerNum)
{
	float time = blowAnimFrm[blowPlayerNum] / 60.0f;
	
	int width, height;
	VECTOR2 center;
	width = sprite->GetSrcWidth();
	height = sprite->GetSrcHeight();
	center.x = WINDOW_WIDTH / 2;
	center.y = WINDOW_HEIGHT / 2 - 150;

	//プレイヤーアイコンアニメーション処理

	//アイコンの出現
	if (time < 0.5f) {
		// アニメーションの進行度計算
		float timeRate = time / 0.5f;
		float rate = ec->easeOutExpo(timeRate); // 滑らかな拡大

		// 回転中心を画像の中心にするための補正
		float pivotX = width / 2.0f;
		float pivotY = height / 2.0f;

		float scale = 3 * rate;	// 拡大倍率

		// ワールド行列の計算
		MATRIX4X4 m = XMMatrixTranslation(-pivotX, -pivotY, 0)
			* XMMatrixScaling(scale, scale, 1.0f)
			* XMMatrixTranslation(center.x, center.y, 0);

		// スプライトをワールド行列を使用して描画
		sprite->Draw(m);
	}
	else if (0.5f <= time < 1.5f) {
		// ワールド行列の計算
		MATRIX4X4 m = XMMatrixTranslation(-width / 2.0f, -height / 2.0f, 0)
			* XMMatrixScaling(3.0f, 3.0f, 1.0f)
			* XMMatrixTranslation(center.x, center.y, 0);

		// スプライトをワールド行列を使用して描画
		sprite->Draw(m);
	}

	if (time > 0.2f) {
		//アイコンの設定 
		sprite->SetSrc(playUiImage, 258, 594, 28, 28);
		
		width = sprite->GetSrcWidth();
		height = sprite->GetSrcHeight();
		// 回転中心を画像の中心にするための補正
		float pivotX = width / 2.0f;
		float pivotY = height / 2.0f;

		//0°~45°で揺れる
		float rotation = (XM_PI / 4) * (0.5f * sin(time * XM_2PI * 2) + 0.5f);

		// ワールド行列の計算
		MATRIX4X4 m = XMMatrixTranslation(-pivotX, -pivotY, 0)
			* XMMatrixRotationZ(rotation)
			* XMMatrixScaling(3.0f, 3.0f, 1.0f)
			* XMMatrixTranslation(center.x + 100, center.y - 70, 0);

		// スプライトをワールド行列を使用して描画
		sprite->Draw(m);
	}
	
	blowAnimFrm[blowPlayerNum]++;

	//アニメーション終了後,吹っ飛ばしたプレイヤー情報を初期化
	if (time > 1.5f) {
		blowPlayerList[blowPlayerNum] = -1;
		blowAnimFrm[blowPlayerNum] = 0;
	}

}
