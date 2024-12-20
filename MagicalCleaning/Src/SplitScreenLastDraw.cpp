#include "SplitScreenLastDraw.h"
#include "PlayScene.h"
#include "Camera.h"
#include "SplitScreen.h"


SplitScreenLastDraw::SplitScreenLastDraw()
{
	ObjectManager::DontDestroy(this);		// 消されない
	ObjectManager::SetDrawOrder(this, -100000);	// 一番最後に描画処理する

	D3D11_VIEWPORT vp = {};

	// １画面	--------------------------
	vpSingle = vp;
	vpSingle.Width  = WINDOW_WIDTH;
	vpSingle.Height = WINDOW_HEIGHT;
	vpSingle.MinDepth = 0.0f;
	vpSingle.MaxDepth = 1.0f;
	vpSingle.TopLeftX = 0;
	vpSingle.TopLeftY = 0;
	GameDevice()->m_pD3D->m_pDeviceContext->RSSetViewports(1, &vpSingle);

	frameUiImage = new CSpriteImage(_T("data/Image/Play/frameUI2.png"));
	playUiImage = new CSpriteImage(_T("data/Image/Play/UISprite3.png"));

	mapImage1 = new CSpriteImage(_T("data/Image/Play/map/stage1.png"));
	mapImage2 = new CSpriteImage(_T("data/Image/Play/map/stage2.png"));

	sprite = new CSprite();

}

SplitScreenLastDraw::~SplitScreenLastDraw()
{
	SAFE_DELETE(frameUiImage);
	SAFE_DELETE(playUiImage);
	SAFE_DELETE(mapImage1);
	SAFE_DELETE(mapImage2);
	SAFE_DELETE(sprite);
}

void SplitScreenLastDraw::Update()
{
}


void SplitScreenLastDraw::Draw()
{
	SplitScreen* ss = ObjectManager::FindGameObject<SplitScreen>();
	Camera* cm = ObjectManager::FindGameObject<Camera>();
	if (ss->Multi())
	{
		// 多画面のとき
		if (ObjectManager::DrawCounter() == ss->MultiSize() - 1)   // 最後の画面のとき
		{
			// １画面に戻す
			GameDevice()->m_pD3D->m_pDeviceContext->RSSetViewports(1, &vpSingle);
			MATRIX4X4 saveProj = GameDevice()->m_mProj;		 	  // プロジェクションマトリックスを待避
			GameDevice()->m_mProj = GameDevice()->m_mProjStandard;	  // 縦横比 標準

			// Ｚバッファのみ全クリヤーする。（画面は消さずに、Ｚバッファのみクリヤーする）
			// この処理は、ここで３Ｄオブジェクトの描画を行うときは必須
			// ２Ｄスプライトのみ描画を行うときは不要
			//if (GameDevice()->m_pD3D->m_pTarget_DSTexDSV){
			//    GameDevice()->m_pD3D->m_pDeviceContext->ClearDepthStencilView(GameDevice()->m_pD3D->m_pTarget_DSTexDSV, D3D11_CLEAR_DEPTH, 1.0f, 0); // 深度バッファクリア
			//}

			GameDevice()->m_vEyePt = cm->EyePt(0); // カメラ座標
			GameDevice()->m_vLookatPt = cm->LookatPt(0); // 注視点
			GameDevice()->m_mView = cm->View(0);   // ビューマトリックス

			// -----------------------------------------------------------------

			// 画面全体の描画

			//画面分割枠描画
			sprite->Draw(frameUiImage, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT);


			//マップ描画
			//sprite->Draw(mapImage1, 608, 264, 50, 50, 50, 50);
			//sprite->Draw(mapImage1, 608, 264+50, 50, 50, 50, 50);

			//player位置表示
			int posX,posY;

			for (Player* pl : players){
				if (pl->GetIsInvisible())
				{
					continue;
				}
				
				//左端 610 右端 745
				posX = 610;
				//上端 335 下端 465
				posY = 465;

				// ミニマップの描画範囲 (左端, 右端, 上端, 下端)
				const int mapLeft = 610;
				const int mapRight = 740;
				const int mapTop = 330;
				const int mapBottom = 465;

				// ワールド座標系の範囲 (-50 ~ 50)
				const float worldMin = -50.0f;
				const float worldMax = 50.0f;

				// ミニマップ上のX座標計算 (ワールドXをミニマップの範囲に変換)
				float normalizedX = (pl->Position().x - worldMin) / (worldMax - worldMin); // 正規化 (0~1)
				int posX = mapLeft + static_cast<int>(normalizedX * (mapRight - mapLeft)); // マップ座標

				// ミニマップ上のY座標計算 (ワールドZをミニマップの範囲に変換)
				float normalizedZ = (pl->Position().z - worldMin) / (worldMax - worldMin); // 正規化 (0~1)
				int posY = mapBottom - static_cast<int>(normalizedZ * (mapBottom - mapTop)); // マップ座標

				//プレイヤーごとのアイコンカラー変更
				sprite->SetSrc(playUiImage,1 + pl->GetColor() * 17, 1, 15, 16, 15, 16);
				int a = pl->GetPlayerNum();

				int width, height;
				width = sprite->GetSrcWidth();
				height = sprite->GetSrcHeight();
				float pivotX = width / 2.0f;
				float pivotY = height / 2.0f;

				MATRIX4X4 m = XMMatrixTranslation(-pivotX, -pivotY, 0)
					* XMMatrixRotationZ(pl->Rotation().y)
					* XMMatrixScaling(1.0f, 1.0f, 1.0f)
					* XMMatrixTranslation(posX+8, posY+8 ,0);

				// スプライトをワールド行列を使用して描画
				sprite->Draw(m);
			}

			 
			//sprite->DrawRect()

			//ゲーム時間表示
			int minutes = gameTime / 60;
			int seconds = gameTime % 60;

			char str[64];

			
			switch (gameState) {
			case sReady:
				sprintf_s(str, "State:Ready");
				break;

			case sPose:
				sprite->DrawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, RGB(0, 0, 0), 0.5f);
				sprintf_s(str, "State:Pose");
				break;

			case sGamePlay:
				sprintf_s(str, "%02d:%02d", minutes, seconds);
				GameDevice()->m_pFont->Draw(WINDOW_WIDTH / 2 - 33, WINDOW_HEIGHT / 2 - 98, str, 30, RGB(255, 0, 0));
				sprintf_s(str, "State:GamePlay");
				break;

			case sFinish:
				sprintf_s(str, "%02d:%02d", minutes, seconds);
				GameDevice()->m_pFont->Draw(WINDOW_WIDTH / 2 - 33, WINDOW_HEIGHT / 2 - 98, str, 30, RGB(0, 255, 0));
				sprintf_s(str, "State:Finish");
				break;
			}

			//GameDevice()->m_pFont->Draw(0, 0, str, 30, RGB(255, 255, 255));
			
			// -----------------------------------------------------------------
			GameDevice()->m_mProj = saveProj;	  // プロジェクションマトリックスを元に戻す
		}
	}

}



void SplitScreenLastDraw::SetState(int state)
{
	gameState = state;
}

void SplitScreenLastDraw::SetGameTime(int time)
{
	gameTime = time;
}

void SplitScreenLastDraw::SetPlayers(list<Player*> players)
{
	this->players = players;
}
