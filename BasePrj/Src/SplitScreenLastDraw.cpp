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

	frameUiImage = new CSpriteImage(_T("data/Image/frameUI.png"));
	sprite = new CSprite();

}

SplitScreenLastDraw::~SplitScreenLastDraw()
{
	SAFE_DELETE(frameUiImage);
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
			sprite->Draw(frameUiImage, 0, 0, 1024, 576, 1024, 576, WINDOW_WIDTH, WINDOW_HEIGHT);

			//ゲーム時間表示
			int minutes = gameTime / 60;
			int seconds = gameTime % 60;

			char str[64];

			switch (gameState) {
			case sReady:
				if (seconds <= 2) { //カウントダウン数字
					sprintf_s(str, "%d", 3 - seconds);
					GameDevice()->m_pFont->Draw(WINDOW_WIDTH / 2 - 33, WINDOW_HEIGHT / 2 - 98, str, 100, RGB(0, 0, 255));
				}
				else {	//「GO!」
					sprintf_s(str, "GO!");
					GameDevice()->m_pFont->Draw(WINDOW_WIDTH / 2 - 66, WINDOW_HEIGHT / 2 - 98, str, 100, RGB(0, 0, 255));
				}
				sprintf_s(str, "State:Ready");
				GameDevice()->m_pFont->Draw(0, 0, str, 100, RGB(255, 255, 255));
				break;

			case sPose:
				sprite->DrawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, RGB(0, 0, 0), 0.5f);
				sprintf_s(str, "State:Pose");
				GameDevice()->m_pFont->Draw(0, 0, str, 100, RGB(255, 255, 255));
				break;

			case sGamePlay:
				sprintf_s(str, "%02d:%02d", minutes, seconds);
				GameDevice()->m_pFont->Draw(WINDOW_WIDTH / 2 - 33, WINDOW_HEIGHT / 2 - 98, str, 30, RGB(255, 0, 0));
				sprintf_s(str, "State:GamePlay");
				GameDevice()->m_pFont->Draw(0, 0, str, 100, RGB(255, 255, 255));
				break;

			case sFinish:
				sprintf_s(str, "%02d:%02d", minutes, seconds);
				GameDevice()->m_pFont->Draw(WINDOW_WIDTH / 2 - 33, WINDOW_HEIGHT / 2 - 98, str, 30, RGB(0, 255, 0));
				sprintf_s(str, "State:Finish");
				GameDevice()->m_pFont->Draw(0, 0, str, 100, RGB(255, 255, 255));
				break;
			}

			

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
