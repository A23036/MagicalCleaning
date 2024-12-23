#include "ResultDisplay.h"

ResultDisplay::ResultDisplay()
{
	dc = ObjectManager::FindGameObject<DataCarrier>();
	ec = ObjectManager::FindGameObject<EasingCalc>();

	sprite = new CSprite();
	resultUiImage1 = new CSpriteImage(_T("data/Image/Play/UISprite.png"));

	animFrm = 0;
	animTime = 0;

	timeRate = 0;
	rate = 0;

	state = aFade;
}

ResultDisplay::~ResultDisplay()
{
	SAFE_DELETE(sprite);
	SAFE_DELETE(resultUiImage1);
}

void ResultDisplay::Update()
{
}

void ResultDisplay::Draw()
{
	animTime = animFrm * (1.0f / 60.0f);

	DrawFinish(state); //アニメーションし終わったUIの常時表示
	switch (state) {
	case aFade:
		timeRate = animTime / 1.5f;
		rate = timeRate;
		float alpha;
		alpha = (0.0f - (1.0f)) * rate + (1.0f);

		sprite->DrawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, RGB(0, 0, 0), alpha);
		if (animTime >= 1.5f) {
			state = aStart;
			animFrm = 0;
		}
		break;
	case aStart:
		timeRate = animTime / 2.0f;
		rate = ec->easeOutBounce(timeRate); //バウンドするアニメーション

		float posX, posY;
		posY = (20 - (-100)) * rate + (-100);

		rate = ec->easeOutExpo(timeRate);
		posX = (20 - (-100)) * rate + (-100);


		sprite->Draw(resultUiImage1, posX, posY, 480, 384, 168, 32, 168 * 2, 32 * 2);
		if (animTime >= 2.0f) {
			state = aChart1;
			animFrm = 0;
		}
		break;
	case aChart1:
		sprite->SetSrc(resultUiImage1, 0, 375, 250, 225);
		// アニメーションの進行度計算
		timeRate = animTime / 1.0f;
		rate = ec->easeOutBack(timeRate); // 滑らかな拡大

		for (int i = 0; i < MAXPLAYER; i++) {
			int width, height;
			width = sprite->GetSrcWidth();
			height = sprite->GetSrcHeight();

			// 拡大中心を画像の中心にするための補正
			float pivotX = width / 2.0f;
			float pivotY = height / 2.0f;

			float posX;

			posX = WINDOW_WIDTH/2 - 450 + i * 300;

			float scale = 1.0f * rate;	// 拡大

			// ワールド行列の計算
			MATRIX4X4 m = XMMatrixTranslation(-pivotX, -pivotY, 0)
				* XMMatrixScaling(scale, scale, 1.0f)
				* XMMatrixTranslation(posX, 650, 0);

			sprite->Draw(m);
		}
		
		if (animTime >= 1.0f) {
			state = aChart2;
			animFrm = 0;
		}
		break;
	case aChart2:
		// アニメーションの進行度計算
		timeRate = animTime / 2.0f;
		rate = timeRate; // 線形補完

		//能力ごとの進捗表示
		for (int i = 0; i < MAXPLAYER; i++) {
			float posX, posY;
			posX = WINDOW_WIDTH / 2 - 450 + i * 300;

			posY = 654;

			//点のカラー格納用配列
			DWORD color[5];

			//表示座標オフセット
			int _offX[5], _offY[5];

			for (int j = 0; j < 5; j++) {
				// 現在の進捗
				float progress = (float)(dc->GetPowerLevel(j,i) + 1) / 10.0f;

				switch (j) {
				case 0: //移動速度
					_offX[0] = 0;
					_offY[0] = -progress * 84;
					color[0] = RGB(55, 49, 245);
					break;
				case 1: //ジャンプ回数
					_offX[1] = progress * 80;
					_offY[1] = -progress * 27;
					color[1] = RGB(245, 49, 238);
					break;
				case 2: //攻撃速度
					_offX[2] = progress * 49;
					_offY[2] = progress * 69;
					color[2] = RGB(238, 245, 49);
					break;
				case 3: //攻撃範囲
					_offX[3] = -progress * 49;
					_offY[3] = progress * 69;
					color[3] = RGB(245, 55, 49);
					break;
				case 4: //手持ちリーフ
					_offX[4] = -progress * 80;
					_offY[4] = -progress * 27;
					color[4] = RGB(49, 245, 55);
					break;
				}
			}

			//各点間の線の描画
			for (int j = 0; j < 5; j++) {
				float startX,startY,endX,endY;
				startX = posX + 2 + _offX[j]*rate;
				startY = posY + 2 + _offY[j]*rate;
				endX = posX + 2 + _offX[j + 1]*rate;
				endY = posY + 2 + _offY[j + 1]*rate;
				if (j + 1 < 5) {
					sprite->DrawLine(startX, startY, endX, endY, 6, RGB(255, 0, 0));
				}
				else {
					endX = posX + 2 + _offX[0]*rate;
					endY = posY + 2 + _offY[0]*rate;
					sprite->DrawLine(startX, startY, endX, endY, 6, RGB(255, 0, 0));
				}
			}

			//各点の描画
			for (int j = 0; j < 5; j++) {
				float _posX, _posY;
				_posX = posX + _offX[j]*rate - 2;
				_posY = posY + _offY[j]*rate - 2;
				sprite->DrawRect(_posX, _posY, 10, 10, color[j]);
			}
		}
		
		if (animTime >= 2.0f) {
			state = aBonus;
			animFrm = 0;
		}
		break;
	case aBonus:
		
		break;
	}
	animFrm++;
}

void ResultDisplay::DrawFinish(int nowAnim)
{
	if (nowAnim > aStart) {
		sprite->Draw(resultUiImage1, 20, 20, 480, 384, 168, 32, 168 * 2, 32 * 2);
	}
	if (nowAnim > aChart1) {
		sprite->SetSrc(resultUiImage1, 0, 375, 250, 225);
		for (int i = 0; i < MAXPLAYER; i++) {
			int width, height;
			width = sprite->GetSrcWidth();
			height = sprite->GetSrcHeight();
			float posX;
			posX = WINDOW_WIDTH / 2 - 450 + i * 300;

			float pivotX = width / 2.0f;
			float pivotY = height / 2.0f;

			// ワールド行列の計算
			MATRIX4X4 m = XMMatrixTranslation(-pivotX, -pivotY, 0)
				* XMMatrixScaling(1.0f, 1.0f, 1.0f)
				* XMMatrixTranslation(posX, 650, 0);

			sprite->Draw(m);
		}
	}
	if(nowAnim > aChart2){
		//能力ごとの進捗表示
		for (int i = 0; i < MAXPLAYER; i++) {
			float posX, posY;
			posX = WINDOW_WIDTH / 2 - 450 + i * 300;

			posY = 654;

			//点のカラー格納用配列
			DWORD color[5];

			//表示座標オフセット
			int _offX[5], _offY[5];

			for (int j = 0; j < 5; j++) {
				// 現在の進捗
				float progress = (float)(dc->GetPowerLevel(j, i) + 1) / 10.0f;

				switch (j) {
				case 0: //移動速度
					_offX[0] = 0;
					_offY[0] = -progress * 84;
					color[0] = RGB(55, 49, 245);
					break;
				case 1: //ジャンプ回数
					_offX[1] = progress * 80;
					_offY[1] = -progress * 27;
					color[1] = RGB(245, 49, 238);
					break;
				case 2: //攻撃速度
					_offX[2] = progress * 49;
					_offY[2] = progress * 69;
					color[2] = RGB(238, 245, 49);
					break;
				case 3: //攻撃範囲
					_offX[3] = -progress * 49;
					_offY[3] = progress * 69;
					color[3] = RGB(245, 55, 49);
					break;
				case 4: //手持ちリーフ
					_offX[4] = -progress * 80;
					_offY[4] = -progress * 27;
					color[4] = RGB(49, 245, 55);
					break;
				}
			}

			//各点間の線の描画
			for (int j = 0; j < 5; j++) {
				float startX, startY, endX, endY;
				startX = posX + 2 + _offX[j];
				startY = posY + 2 + _offY[j];
				endX = posX + 2 + _offX[j + 1];
				endY = posY + 2 + _offY[j + 1];
				if (j + 1 < 5) {
					sprite->DrawLine(startX, startY, endX, endY, 6, RGB(255, 0, 0));
				}
				else {
					endX = posX + 2 + _offX[0];
					endY = posY + 2 + _offY[0];
					sprite->DrawLine(startX, startY, endX, endY, 6, RGB(255, 0, 0));
				}
			}

			//各点の描画
			for (int j = 0; j < 5; j++) {
				float _posX, _posY;
				_posX = posX + _offX[j] - 2;
				_posY = posY + _offY[j] - 2;
				sprite->DrawRect(_posX, _posY, 10, 10, color[j]);
			}

		}
	}
}
