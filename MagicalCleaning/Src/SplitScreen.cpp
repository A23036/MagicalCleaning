#include "SplitScreen.h"
#include "PlayScene.h"
#include "Camera.h"
#include "SplitScreenLastDraw.h"

SplitScreen::SplitScreen()
{
	ObjectManager::DontDestroy(this);		// SplitScreenは消されない
	ObjectManager::SetActive(this, false);		// SplitScreenは更新しない
	ObjectManager::SetDrawOrder(this, 100000);	// 一番最初に描画処理する

	// 画面分割の最後に全画面に描画する処理（必須ではない）
	SingleInstantiate <SplitScreenLastDraw>();	// SplitScreenLastDrawは一つしか作らない。NoDestroy。


	D3D11_VIEWPORT vp = {};

	// １画面	--------------------------
	vpSingle = vp;
	vpSingle.Width  = WINDOW_WIDTH;
	vpSingle.Height = WINDOW_HEIGHT;
	vpSingle.MinDepth = 0.0f;
	vpSingle.MaxDepth = 1.0f;
	vpSingle.TopLeftX = 0;
	vpSingle.TopLeftY = 0;

	/*
	// 多画面(２画面)	---------------------------
	// 左半分画面　[0]
	vpMulti.emplace_back(vp);
	vpMulti.back().Width  = WINDOW_WIDTH/2;
	vpMulti.back().Height = WINDOW_HEIGHT;
	vpMulti.back().MinDepth = 0.0f;
	vpMulti.back().MaxDepth = 1.0f;
	vpMulti.back().TopLeftX = 0;
	vpMulti.back().TopLeftY = 0;

	// 右半分画面　[1]
	vpMulti.emplace_back(vp);
	vpMulti.back().Width  = WINDOW_WIDTH/2;
	vpMulti.back().Height = WINDOW_HEIGHT;
	vpMulti.back().MinDepth = 0.0f;
	vpMulti.back().MaxDepth = 1.0f;
	vpMulti.back().TopLeftX = WINDOW_WIDTH/2;
	vpMulti.back().TopLeftY = 0;
	*/

	// 多画面(４画面)	---------------------------
	// 左上半分画面　[0]
	vpMulti.emplace_back(vp);
	vpMulti.back().Width  = WINDOW_WIDTH/2;
	vpMulti.back().Height = WINDOW_HEIGHT/2;
	vpMulti.back().MinDepth = 0.0f;
	vpMulti.back().MaxDepth = 1.0f;
	vpMulti.back().TopLeftX = 0;
	vpMulti.back().TopLeftY = 0;

	// 右上半分画面　[1]
	vpMulti.emplace_back(vp);
	vpMulti.back().Width  = WINDOW_WIDTH/2;
	vpMulti.back().Height = WINDOW_HEIGHT/2;
	vpMulti.back().MinDepth = 0.0f;
	vpMulti.back().MaxDepth = 1.0f;
	vpMulti.back().TopLeftX = WINDOW_WIDTH/2;
	vpMulti.back().TopLeftY = 0;

	// 左下半分画面　[2]
	vpMulti.emplace_back(vp);
	vpMulti.back().Width = WINDOW_WIDTH / 2;
	vpMulti.back().Height = WINDOW_HEIGHT / 2;
	vpMulti.back().MinDepth = 0.0f;
	vpMulti.back().MaxDepth = 1.0f;
	vpMulti.back().TopLeftX = 0;
	vpMulti.back().TopLeftY = WINDOW_HEIGHT / 2;

	// 右下半分画面　[3]
	vpMulti.emplace_back(vp);
	vpMulti.back().Width  = WINDOW_WIDTH/2;
	vpMulti.back().Height = WINDOW_HEIGHT/2;
	vpMulti.back().MinDepth = 0.0f;
	vpMulti.back().MaxDepth = 1.0f;
	vpMulti.back().TopLeftX = WINDOW_WIDTH/2;
	vpMulti.back().TopLeftY = WINDOW_HEIGHT/2;

	// -----------------------------------------------
	// 最初は全画面をセット
	multi = false;
	multiNo = 0;
	GameDevice()->m_pD3D->m_pDeviceContext->RSSetViewports(1, &vpSingle);

}

SplitScreen::~SplitScreen()
{
}

void SplitScreen::Start()
{

}

void SplitScreen::SetSingleScreen()
{
	multi = false;
	GameDevice()->m_mProj = GameDevice()->m_mProjStandard;	  // 縦横比 標準
	ObjectManager::SetDrawTimes(1);
}

void SplitScreen::SetMultiScreen()
{
	multi = true;
	//GameDevice()->m_mProj = GameDevice()->m_mProjVerticalLong;	  // 縦横比 縦長(２画面のとき)
	GameDevice()->m_mProj = GameDevice()->m_mProjStandard;	  // 縦横比 標準
	ObjectManager::SetDrawTimes((int)vpMulti.size());
}


void SplitScreen::Draw()
{
	Camera* cm = ObjectManager::FindGameObject<Camera>();
	if (multi)
	{
		// 多画面
		GameDevice()->m_pD3D->m_pDeviceContext->RSSetViewports(1, &vpMulti[ObjectManager::DrawCounter()]);

		// その画面ObjectManager::DrawCount()に対応したカメラ座標をGameMainに設定する
		if (cm != nullptr)
		{
			GameDevice()->m_vEyePt = cm->EyePt(ObjectManager::DrawCounter()); // カメラ座標
			GameDevice()->m_vLookatPt = cm->LookatPt(ObjectManager::DrawCounter()); // 注視点
			GameDevice()->m_mView = cm->View(ObjectManager::DrawCounter());  // ビューマトリックス
		}
	}
	else
	{
		// １画面
		GameDevice()->m_pD3D->m_pDeviceContext->RSSetViewports(1, &vpSingle);

		// カメラ座標をGameMainに設定する
		if (cm != nullptr)
		{
			GameDevice()->m_vEyePt = cm->EyePt(0); // カメラ座標
			GameDevice()->m_vLookatPt = cm->LookatPt(0); // 注視点
			GameDevice()->m_mView = cm->View(0);   // ビューマトリックス
		}
	}
}
