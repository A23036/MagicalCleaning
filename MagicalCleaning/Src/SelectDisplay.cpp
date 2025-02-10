#include "SelectDisplay.h"
#include "EasingCalc.h"

SelectDisplay::SelectDisplay()
{
	SetDrawOrder(-10000);	// 一番最後に描画する
	
	dc = ObjectManager::FindGameObject<DataCarrier>();
	ec = ObjectManager::FindGameObject<EasingCalc>();

	sprite = new CSprite();
	selectUiImage = new CSpriteImage(_T("data/Image/Select/UI.png"));

	isTransition = true; //開始時、フェードイン
	isFirst = true;
	transFrm = 0;

	ColorPosY = WINDOW_HEIGHT * 2/5;		//キャラ選択カラー位置Y
	ColorIconSize = 64;						//キャラ選択カラーサイズ
	ColorIconDispSize = 100;				//キャラ選択カラー表示サイズ
	UiSpace = 50;							//キャラ選択カラー幅
	GuideUiPosY = WINDOW_HEIGHT - 100;		//操作方法ガイドUI位置Y
	BackUiPos = VECTOR2(20,20);	//戻るUI位置

	isReadyAll = false;
	MoveFrm = 10;

	for (int i = 0; i < 4; i++) {
		state[i] = sColor;
		playerEntry[i] = false;
		isSetUpCamera[i] = false;
		isReady[i] = false;
		selectCamera[i] = 0;
		moveFrm[i] = MoveFrm;
		animFrm[i] = 0;
	}

	for (int i = 0; i < 6; i++) {
		isSelect[i] = false;
	}

	//カラー選択初期値
	selectColor[0] = 0;
	selectColor[1] = 0;
	selectColor[2] = 0;
	selectColor[3] = 0;
}

SelectDisplay::~SelectDisplay()
{
	SAFE_DELETE(sprite);
	SAFE_DELETE(selectUiImage);
}

void SelectDisplay::Update()
{
	auto di = GameDevice()->m_pDI;

	//プレイヤーごとの設定処理
	for (int i = 0; i < MAXPLAYER; i++) {
		// コントローラースティックの入力を取得、正規化
		float ix = di->GetJoyState(i).lX / 1000.0f; // 左右の入力 -1.0~1.0
		float iy = di->GetJoyState(i).lY / 1000.0f; // 上下の入力 -1.0~1.0

		//カーソル移動用フレーム加算
		moveFrm[i]++;

		switch (state[i]) {
		case sColor:
			UpdateColorSelect(i,ix,iy);
			break;
		case sCamera:
			UpdateCameraSetting(i,ix,iy);
			break;
		case sReady:
			UpdateReady(i);
			break;
		case sEnd:
			UpdateEnd(i);
			break;
		}
	}
	
	int num = 0; //準備完了人数
	for (int i = 0; i < MAXPLAYER; i++) {
		if (isReady[i]) {
			animFrm[i]++;
			if (animFrm[i] * (1.0f / 60.0f) > 1.0f) { //準備完了アニメーションが終了している
				num++;
			}
		}
	}

	if (num == MAXPLAYER) { //全員が準備完了
		//DataCarrierに設定情報を保存
		dc->SetCameraSetteing(selectCamera);
		dc->SetColor(selectColor);

		isTransition = true;
	}
	else {
		isReadyAll = false;
	}
}

void SelectDisplay::Draw()
{
	//UIイメージの設定
	sprite->SetImage(selectUiImage);

	//戻るアイコン
	sprite->SetSrc(1, 330, 64, 64);
	sprite->Draw(BackUiPos.x, BackUiPos.y);
	//戻るボタン
	sprite->SetSrc(133, 67, 64, 64);
	sprite->Draw(BackUiPos.x + 64 + 16, BackUiPos.y);

	//カーソル操作ガイドUI
	//Lスティックアイコン
	sprite->SetSrc(272, 48, 132, 80);
	sprite->Draw(500, 100);
	//決定ボタン
	sprite->SetSrc(67, 67, 64, 64);
	sprite->Draw(642, 106);
	//「OK」UI
	sprite->SetSrc(1, 34, 92, 32);
	sprite->Draw(570, 180);
	//戻るボタン
	sprite->SetSrc(133, 67, 64, 64);
	sprite->Draw(750, 106);
	//「BACK」UI
	sprite->SetSrc(96, 34, 140, 32);
	sprite->Draw(712, 180);

	//キャラクターカラー
	//赤
	sprite->SetSrc(1, 132, ColorIconSize, ColorIconSize, ColorIconDispSize, ColorIconDispSize);
	sprite->Draw(WINDOW_WIDTH / 2 - ColorIconDispSize / 2 - UiSpace - ColorIconDispSize, ColorPosY);

	//青
	sprite->SetSrc(67, 132, ColorIconSize, ColorIconSize, ColorIconDispSize, ColorIconDispSize);
	sprite->Draw(WINDOW_WIDTH / 2 - ColorIconDispSize / 2, ColorPosY);

	//黄
	sprite->SetSrc(133, 132, ColorIconSize, ColorIconSize, ColorIconDispSize, ColorIconDispSize);
	sprite->Draw(WINDOW_WIDTH / 2 - ColorIconDispSize / 2 + UiSpace + ColorIconDispSize, ColorPosY);

	//緑
	sprite->SetSrc(1, 197, ColorIconSize, ColorIconSize, ColorIconDispSize, ColorIconDispSize);
	sprite->Draw(WINDOW_WIDTH / 2 - ColorIconDispSize / 2 - UiSpace - ColorIconDispSize, ColorPosY + UiSpace + ColorIconDispSize);

	//紫
	sprite->SetSrc(67, 197, ColorIconSize, ColorIconSize, ColorIconDispSize, ColorIconDispSize);
	sprite->Draw(WINDOW_WIDTH / 2 - ColorIconDispSize / 2, ColorPosY + UiSpace + ColorIconDispSize);

	//黒
	sprite->SetSrc(133, 197, ColorIconSize, ColorIconSize, ColorIconDispSize, ColorIconDispSize);
	sprite->Draw(WINDOW_WIDTH / 2 - ColorIconDispSize / 2 + UiSpace + ColorIconDispSize, ColorPosY + UiSpace + ColorIconDispSize);

	//プレイヤーの状態に応じたUIの描画
	DrawUI();

	for (int i = 0; i < MAXPLAYER; i++) {
		sprite->SetSrc(selectUiImage, 1, 1, 192, 32);
		// アニメーションの進行度計算
		float timeRate = animFrm[i]*(1.0f/60.0f) / 1.0f;
		if (timeRate > 1.0f) {
			timeRate = 1.0f;
		}
		float rate = ec->easeOutExpo(timeRate); // 滑らかな拡大

		int width, height;
		width = sprite->GetSrcWidth();
		height = sprite->GetSrcHeight();

		// 拡大中心を画像の中心にするための補正
		float pivotX = width / 2.0f;
		float pivotY = height / 2.0f;

		float posX;
		float posY;

		posX = 260 + i % 2 * 840;
		posY = 250 + i / 2 * 340;

		float scale = 2.0f * rate;	// 拡大

		// ワールド行列の計算
		MATRIX4X4 m = XMMatrixTranslation(-pivotX, -pivotY, 0)
			* XMMatrixScaling(scale, scale, 1.0f)
			* XMMatrixTranslation(posX, posY, 0);

		sprite->Draw(m);
	}

	//トランジション処理
	if (isTransition) {
		Transition();
	}
}

void SelectDisplay::DrawUI()
{
	for (int i = 0; i < MAXPLAYER; i++) {
		int offX = 0, offY = 0;
		int dispOffX = 0, dispOffY = 0;
		int width, height, dispWidth, dispHeight;

		//カーソルサイズの幅を設定
 		switch (i) {
		case 0:
		case 2:
			//1Pか3Pが選択中の色を2Pか4Pが選択していた時
			if (selectColor[1] == selectColor[i] || selectColor[3] == selectColor[i]) {
				width = ColorIconSize / 2; //カーソル幅を半分に
			}
			else {
				width = ColorIconSize; //カーソル幅はそのまま
			}
			break;
		case 1:
		case 3:
			//2Pか4Pが選択中の色を1Pか3Pが選択していた時
			if (selectColor[0] == selectColor[i] || selectColor[2] == selectColor[i]) {
				width = ColorIconSize / 2;  //カーソル幅を半分に
				offX = ColorIconSize / 2;	//カーソルUIの右半分の位置を設定
				dispOffX = ColorIconDispSize / 2; //表示位置をカーソルアイコンの半分だけ右に
			}
			else {
				width = ColorIconSize; //カーソル幅はそのまま
			}
			break;
		}

		//カーソルサイズの高さを設定
		switch (i) {
		case 0:
			//1Pが選択中の色を3Pが選択していた時
			if (selectColor[2] == selectColor[i]) {
				height = ColorIconSize / 2; //カーソルサイズを半分に
			}
			else {
				height = ColorIconSize; //カーソルサイズはそのまま
			}
			break;
		case 1:
			//2Pが選択中の色を4Pが選択していた時
			if (selectColor[3] == selectColor[i]) {
				height = ColorIconSize / 2; //カーソルサイズを半分に
			}
			else {
				height = ColorIconSize; //カーソルサイズはそのまま
			}
			break;
		case 2:
			//3Pが選択中の色を1Pが選択していた時
			if (selectColor[0] == selectColor[i]) {
				height = ColorIconSize / 2; //カーソルサイズを半分に
				offY = ColorIconSize / 2;	//カーソルUIの下半分の位置を設定
				dispOffY = ColorIconDispSize / 2; //表示位置をカーソルアイコンの半分だけ下に
			}
			else {
				height = ColorIconSize; //カーソルサイズはそのまま
			}
			break;
		case 3:
			//4Pが選択中の色を2Pが選択していた時
			if (selectColor[1] == selectColor[i]) {
				height = ColorIconSize / 2; //カーソルサイズを半分に
				offY = ColorIconSize / 2;	//カーソルUIの下半分の位置を設定
				dispOffY = ColorIconDispSize / 2; //表示位置をカーソルアイコンの半分だけ下に
			}
			else {
				height = ColorIconSize; //カーソルサイズはそのまま
			}
			break;
		}

		//カーソルスプライトの設定
		dispWidth = ColorIconDispSize * (float)width / (float)ColorIconSize;
		dispHeight = ColorIconDispSize * (float)height / (float)ColorIconSize;

		sprite->SetSrc(1 + i * 66 + offX, 264 + offY, width, height, dispWidth, dispHeight);
		sprite->Draw(cursorPos[i].x + dispOffX, cursorPos[i].y + dispOffY);	//カーソル位置を計算し描画


		//「○P」スプライトの描画
		sprite->SetSrc(208, 144 + i * 18, 24, 16, 48, 32);
		
		if (playerEntry[i]) { //カラー選択済
			//セレクト中の時、アイコンの真ん中に表示
			sprite->Draw(cursorPos[i].x + 28, cursorPos[i].y + 36);

			sprite->SetSrc(80, 336, 444, 80); //カメラ設定、準備完了UI表示
			sprite->Draw(20+i%2 * 880, 300 + i/2 * 350); //プレイヤーごとに位置指定

			//チェックボックスUI表示
			if (isSetUpCamera[i]) { //カメラ設定選択済み
				sprite->SetSrc(100, 432, 16, 16);
				sprite->Draw(192 + selectCamera[i] * 112 + i % 2 * 880, 312 + i / 2 * 350);

				if (isReady[i]) { //準備完了
					sprite->SetSrc(100, 432, 16, 16);
					sprite->Draw(192 + i % 2 * 880, 356 + i / 2 * 350);
				}
				else {
					sprite->SetSrc(80, 432, 16, 16);
					sprite->Draw(192 + i % 2 * 880, 356 + i / 2 * 350);
				}
				
			}
			else { //カメラ設定選択未選択
				sprite->SetSrc(80, 432, 16, 16);
				sprite->Draw(192 + selectCamera[i] * 112 + i % 2 * 880, 312 + i / 2 * 350);
			}
		}
		else { //カラー未選択
			switch (i) {
			case 0:
				sprite->Draw(cursorPos[i].x - 10, cursorPos[i].y - 10);
				break;
			case 1:
				sprite->Draw(cursorPos[i].x + 60, cursorPos[i].y - 10);
				break;
			case 2:
				sprite->Draw(cursorPos[i].x - 10, cursorPos[i].y + 80);
				break;
			case 3:
				sprite->Draw(cursorPos[i].x + 60, cursorPos[i].y + 80);
				break;
			}
		}

	}
}

void SelectDisplay::Transition()
{
	if (isFirst) {
		float animTime = transFrm * (1.0f / 60.0f);

		float timeRate = (animTime - 0.4f) / 1.0f;
		float rate = ec->easeOutExpo(timeRate);
		if (timeRate < 0) {
			rate = 0;
		}
		float height = -WINDOW_HEIGHT * rate + WINDOW_HEIGHT;

		sprite->DrawRect(0, 0, WINDOW_WIDTH, height, RGB(205, 200, 255));

		timeRate = (animTime - 0.2f) / 1.0f;
		rate = ec->easeOutExpo(timeRate);
		if (timeRate < 0) {
			rate = 0;
		}
		height = -WINDOW_HEIGHT * rate + WINDOW_HEIGHT;
		sprite->DrawRect(0, 0, WINDOW_WIDTH, height, RGB(154, 145, 255));

		timeRate = animTime / 1.0f;
		rate = ec->easeOutExpo(timeRate);
		height = -WINDOW_HEIGHT * rate + WINDOW_HEIGHT;
		sprite->DrawRect(0, 0, WINDOW_WIDTH, height, RGB(0, 0, 0));

		transFrm++;
		if (rate >= 1.0f) {
			isTransition = false;
			isFirst = false;
			transFrm = 0;
		}
	}
	else {
		//上記コードを反転させる結果になる処理
		float animTime = transFrm * (1.0f / 60.0f);

		float timeRate = animTime / 1.0f;
		float rate = ec->easeOutExpo(timeRate);
		float posY = -WINDOW_HEIGHT * rate + WINDOW_HEIGHT;

		sprite->DrawRect(0, posY, WINDOW_WIDTH, WINDOW_HEIGHT, RGB(205, 200, 255));

		timeRate = (animTime - 0.2f) / 1.0f;
		rate = ec->easeOutExpo(timeRate);
		if (timeRate < 0) {
			rate = 0;
		}
		posY = -WINDOW_HEIGHT * rate + WINDOW_HEIGHT;
		sprite->DrawRect(0, posY, WINDOW_WIDTH, WINDOW_HEIGHT, RGB(154, 145, 255));

		timeRate = (animTime - 0.4f) / 1.0f;
		rate = ec->easeOutExpo(timeRate);
		if (timeRate < 0) {
			rate = 0;
		}
		posY = -WINDOW_HEIGHT * rate + WINDOW_HEIGHT;
		sprite->DrawRect(0, posY, WINDOW_WIDTH, WINDOW_HEIGHT, RGB(0, 0, 0));

		transFrm++;
		if (rate >= 0.9f) {
			isReadyAll = true;
		}
	}
	
}

void SelectDisplay::UpdateColorSelect(int playerNum, int ix, int iy)
{
	auto di = GameDevice()->m_pDI;

	//カラーの選択
		// 横方向の移動
	if (moveFrm[playerNum] >= MoveFrm) {
		if (ix > 0.8f) { // 右方向入力
			GameDevice()->colorSelectSE->Play();
			if (selectColor[playerNum] % 3 == 2) { // 右端の場合
				selectColor[playerNum] -= 2;      // 左端にループ
			}
			else {
				selectColor[playerNum]++;         // 右に移動
			}
			moveFrm[playerNum] = 0;
		}
		else if (ix < -0.8f) { // 左方向入力
			GameDevice()->colorSelectSE->Play();
			if (selectColor[playerNum] % 3 == 0) { // 左端の場合
				selectColor[playerNum] += 2;      // 右端にループ
			}
			else {
				selectColor[playerNum]--;         // 左に移動
			}
			moveFrm[playerNum] = 0;
		}
	}

	// 縦方向の移動
	if (moveFrm[playerNum] >= MoveFrm) {
		if (iy > 0.8f) { // 下方向入力
			GameDevice()->colorSelectSE->Play();
			if (selectColor[playerNum] / 3 == 1) { // 下端の場合
				selectColor[playerNum] -= 3;      // 上端にループ
			}
			else {
				selectColor[playerNum] += 3;      // 下に移動
			}
			moveFrm[playerNum] = 0;
		}
		else if (iy < -0.8f) { // 上方向入力
			GameDevice()->colorSelectSE->Play();
			if (selectColor[playerNum] / 3 == 0) { // 上端の場合
				selectColor[playerNum] += 3;      // 下端にループ
			}
			else {
				selectColor[playerNum] -= 3;      // 上に移動
			}
			moveFrm[playerNum] = 0;
		}
	}

	//カラー決定処理
	if (di->CheckJoy(KD_TRG, 2, playerNum)) {
		if (isSelect[selectColor[playerNum]]) { //決定したカラーがすでに選択されていた時
			//せんたくされているよ！
			GameDevice()->cancelSE->Play();
		}
		else {
			GameDevice()->decisionSE->Play();
			isSelect[selectColor[playerNum]] = true; //カラーを選択済みに
			playerEntry[playerNum] = true;		//プレイヤーを選択完了に
			state[playerNum] = sCamera;
		}
	}

	//選択カラーごとの位置情報の設定
	int offX = WINDOW_WIDTH / 2 - ColorIconDispSize / 2; //表示位置x座標オフセット
	
	switch (selectColor[playerNum]) {
	case Red:
		cursorPos[playerNum] = VECTOR2(offX - UiSpace - ColorIconDispSize, ColorPosY);
		break;
	case Blue:
		cursorPos[playerNum] = VECTOR2(offX, ColorPosY);
		break;
	case Yellow:
		cursorPos[playerNum] = VECTOR2(offX + UiSpace + ColorIconDispSize, ColorPosY);
		break;
	case Green:
		cursorPos[playerNum] = VECTOR2(offX - UiSpace - ColorIconDispSize, ColorPosY + UiSpace + ColorIconDispSize);
		break;
	case Purple:
		cursorPos[playerNum] = VECTOR2(offX, ColorPosY + UiSpace + ColorIconDispSize);
		break;
	case Black:
		cursorPos[playerNum] = VECTOR2(offX + UiSpace + ColorIconDispSize, ColorPosY + UiSpace + ColorIconDispSize);
		break;
	}
}

void SelectDisplay::UpdateCameraSetting(int playerNum, int ix, int iy)
{
	auto di = GameDevice()->m_pDI;

	// チェックマーク横方向の移動
	if (moveFrm[playerNum] >= MoveFrm) {
		if (ix > 0.8f || ix < -0.8f) { // 左右入力
			GameDevice()->colorSelectSE->Play();
			if (selectCamera[playerNum] == 0) {
				selectCamera[playerNum] = 1;
			}
			else {
				selectCamera[playerNum] = 0;
			}
			moveFrm[playerNum] = 0;
		}
	}

	//カメラ設定決定処理
	if (di->CheckJoy(KD_TRG, 2, playerNum)) {
		GameDevice()->decisionSE->Play();
		isSetUpCamera[playerNum] = true;
		state[playerNum] = sReady;
	}
	//カラー決定キャンセル処理
	else if (di->CheckJoy(KD_TRG, 1, playerNum)) {
		GameDevice()->backSE->Play();
		isSelect[selectColor[playerNum]] = false;	//カラーを未選択に
		playerEntry[playerNum] = false;				//プレイヤーを選択未完了に
		state[playerNum] = sColor;
	}
}

void SelectDisplay::UpdateReady(int playerNum)
{
	auto di = GameDevice()->m_pDI;

	//準備完了処理
	if (di->CheckJoy(KD_TRG, 2, playerNum)) {
		GameDevice()->entrySE->Play();
		isReady[playerNum] = true;
		GameDevice()->m_pDI->PlayJoyEffect(0, 1, playerNum); // 振動の再生
		state[playerNum] = sEnd;
	}
	//カメラ設定キャンセル処理
	else if (di->CheckJoy(KD_TRG, 1, playerNum)) {
		GameDevice()->backSE->Play();
		isSetUpCamera[playerNum] = false;
		state[playerNum] = sCamera;
	}
}

void SelectDisplay::UpdateEnd(int playerNum)
{
	auto di = GameDevice()->m_pDI;

	//準備完了キャンセル処理
	if (di->CheckJoy(KD_TRG, 1, playerNum)) { //キャンセルボタンの入力
		GameDevice()->backSE->Play();
		isReady[playerNum] = false;
		animFrm[playerNum] = 0;
		state[playerNum] = sReady;
	}
}
