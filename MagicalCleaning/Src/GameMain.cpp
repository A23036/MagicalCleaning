//=============================================================================
//		３Ｄゲームプログラム
//		Ｃ３ＤＡｃｔ１１１　　　　　　                 ver 3.2        2023.1.31
//
//		ゲームのメイン処理
//																GameMain.cpp
//=============================================================================
#include "GameMain.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "ResourceManager.h"

namespace {
	CGameMain* gameMainInstance;
}

// ============================================================================================
//
// CGameMain ゲームメインクラスの処理
//
// ============================================================================================
//------------------------------------------------------------------------
//
//	ゲームメインクラスのコンストラクタ	
//
//  引数　なし
//
//------------------------------------------------------------------------
CGameMain::CGameMain(CMain*	pMain)
{
	gameMainInstance = this;
	m_pMain = pMain;		// メインクラス

	m_pD3D = nullptr;			//	Direct3Dオブジェクト
	m_pXAudio = nullptr;		//	XAudioオブジェクト
	m_pDI = nullptr;			//	DirectInputDオブジェクト
	m_pFont = nullptr;			//	フォントテクスチャー
	m_pShader = nullptr;		//	シェーダー
	m_pFbxMeshCtrl = nullptr;	// メッシュコントロールクラス

	m_dwGameStatus = 0;		// ゲームステータス

	// カメラ・ライト・ビュー
	m_vEyePt = VECTOR3(0,0,0);		// カメラ（視点）位置
	m_vLookatPt = VECTOR3(0,0,0);	// 注視位置
	m_mView = XMMatrixIdentity();	// ビューマトリックス
	m_mProj = XMMatrixIdentity();	// プロジェクションマトリックス
	m_vLightDir = VECTOR3(0,0,0);	// ディレクショナルライトの方向
}
//------------------------------------------------------------------------
//
//	ゲームメインクラスのデストラクタ	
//
//------------------------------------------------------------------------
CGameMain::~CGameMain()
{
	ResourceManager::Reset();
	SceneManager::Release();
	ObjectManager::Release();
	MyImgui::ImguiQuit();          // -- 2020.11.15    // MyImguiの終了処理

	SAFE_DELETE(m_pFbxMeshCtrl);    // -- 2021.2.4
	SAFE_DELETE(m_pShader);
	SAFE_DELETE(m_pFont);
	SAFE_DELETE(m_pDI);
	SAFE_DELETE(m_pXAudio);
	SAFE_DELETE(m_pD3D);
	
	CoUninitialize();   // COMの解放
}

//------------------------------------------------------------------------
//
//	アプリケーションの初期化処理	
//
//  引数　なし
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
HRESULT CGameMain::Init()
{
	// ＣＯＭの初期化
	//CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	CoInitialize(nullptr);

	// Direct3Dの初期化
	m_pD3D = new CDirect3D;
	if (FAILED(m_pD3D->InitD3D(m_pMain->m_hWnd, WINDOW_WIDTH, WINDOW_HEIGHT))) {
		MessageBox(0, _T("Direct3Dを初期化出来ません"), nullptr, MB_OK);
		return E_FAIL;
	}

	// シェーダーの初期化
	m_pShader = new CShader(m_pD3D);
	if (FAILED(m_pShader->InitShader())) {
		MessageBox(0, _T("Shaderを生成・初期化出来ません"), nullptr, MB_OK);
		return E_FAIL;
	}

	// XAudio2の初期化
	m_pXAudio = new CXAudio;
	if (FAILED(m_pXAudio->InitAudio(m_pMain->m_hWnd))) {
		MessageBox(0, _T("XAudio2を初期化出来ません"), nullptr, MB_OK);
		return E_FAIL;
	}

	// DirectInputの初期化
	m_pDI = new CDirectInput;
	if (!(m_pDI->StartDirectInput(m_pMain->m_hInstance, m_pMain->m_hWnd, 
                    INIT_KEYBOARD | INIT_MOUSE | INIT_JOYSTICK, WINDOW_WIDTH, WINDOW_HEIGHT))) {
		MessageBox(0, _T("DirectInputを初期化出来ません"), nullptr, MB_OK);
		return E_FAIL;
	}

	// フォントテクスチャーの初期化
	m_pFont = new CFontTexture(m_pShader);

	// MyImguiの初期化
	MyImgui::ImguiInit(m_pMain->m_hWnd, m_pD3D, WINDOW_WIDTH, WINDOW_HEIGHT);           // -- 2020.11.15

	// Fbxメッシュコントロールクラス
	m_pFbxMeshCtrl = new CFbxMeshCtrl(m_pShader);                       // -- 2021.2.4

	// ----------------------------------------------------------------------------------------
	// 変数の初期化
	m_vLightDir = normalize( VECTOR3(0.8f, 1, -1) );  // 光源座標の初期値。正規化する

	// プロジェクショントランスフォーム（射影変換）の初期値
	//m_mProj = XMMatrixPerspectiveFovLH((FLOAT)(XM_PI / 4), (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 1000.0);
	m_mProjStandard = XMMatrixPerspectiveFovLH(XMConvertToRadians(38.0f), (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 1000.0);
	m_mProjVerticalLong = XMMatrixPerspectiveFovLH(XMConvertToRadians(38.0f), ((FLOAT)WINDOW_WIDTH / 2) / (FLOAT)WINDOW_HEIGHT, 0.1f, 1000.0);
	m_mProj = XMMatrixPerspectiveFovLH(XMConvertToRadians(38.0f), (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 1000.0);

	// ビュートランスフォーム（視点座標変換）の初期値
	VECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//上方位置
	m_vEyePt = VECTOR3(0.0f, 4.0f, -5.0f);
	m_vLookatPt = VECTOR3(0.0f, 2.0f, 0.0f);
	m_mView = XMMatrixLookAtLH(m_vEyePt, m_vLookatPt, vUpVec);

	//サウンドデータのロード
	// BGM
	titleBGM = new CXAudioSource(m_pXAudio, _T("data/BGM/TitleBGM.wav"));
	titleBGM->VolumeAudio(0.5f);
	selectBGM = new CXAudioSource(m_pXAudio, _T("data/BGM/SelectBGM.wav"));
	selectBGM->VolumeAudio(0.5f);
	playBGM = new CXAudioSource(m_pXAudio, _T("data/BGM/PlayBGM.wav"));
	playBGM->VolumeAudio(0.5f);
	spurtBGM = new CXAudioSource(m_pXAudio, _T("data/BGM/LastSpurtBGM.wav"));
	spurtBGM->VolumeAudio(0.5f);
	resultBGM = new CXAudioSource(m_pXAudio, _T("data/BGM/ResultBGM.wav"));
	//SE
	gameStartSE = new CXAudioSource(m_pXAudio, _T("data/SE/GameStart.wav"));
	swingSE		= new CXAudioSource(m_pXAudio, _T("data/SE/Swing.wav"));
	swingSE->VolumeAudio(1.5f);
	downSE		= new CXAudioSource(m_pXAudio, _T("data/SE/Down.wav"));
	scaleUpSE	= new CXAudioSource(m_pXAudio, _T("data/SE/ScaleUp.wav"));
	colorSelectSE = new CXAudioSource(m_pXAudio, _T("data/SE/ColorSelect.wav"), 4);
	decisionSE	= new CXAudioSource(m_pXAudio, _T("data/SE/Disision.wav"), 4);
	backSE		= new CXAudioSource(m_pXAudio, _T("data/SE/Back.wav"), 4);
	entrySE		= new CXAudioSource(m_pXAudio, _T("data/SE/Entry.wav"), 4);
	cancelSE	= new CXAudioSource(m_pXAudio, _T("data/SE/Cancel.wav"), 4);
	jumpSE		= new CXAudioSource(m_pXAudio, _T("data/SE/Jump.wav"), 4);
	attack1SE	= new CXAudioSource(m_pXAudio, _T("data/SE/Attack1.wav"), 4);
	attack2SE	= new CXAudioSource(m_pXAudio, _T("data/SE/Attack2.wav"), 4);
	attack3SE	= new CXAudioSource(m_pXAudio, _T("data/SE/Attack3.wav"), 4);
	powerUpSE	= new CXAudioSource(m_pXAudio, _T("data/SE/PowerUp.wav"), 4);
	powerSelectSE = new CXAudioSource(m_pXAudio, _T("data/SE/PowerSelect.wav"), 4);
	blowSE		= new CXAudioSource(m_pXAudio, _T("data/SE/Blow.wav"), 4);
	lastSpurtSE = new CXAudioSource(m_pXAudio, _T("data/SE/LastSpurt.wav"), 4);
	blowCheerSE = new CXAudioSource(m_pXAudio, _T("data/SE/BlowCheer.wav"), 4);
	magicCircleSE = new CXAudioSource(m_pXAudio, _T("data/SE/MagicCircle.wav"),4);
	chargeSE = new CXAudioSource(m_pXAudio, _T("data/SE/Charge.wav"), 4);
	chargeSE->VolumeAudio(2.5f);
	itemSE = new CXAudioSource(m_pXAudio, _T("data/SE/Item.wav"));
	itemSE->VolumeAudio(1.5f);
	timeSE = new CXAudioSource(m_pXAudio, _T("data/SE/Time.wav"));
	whistleSE = new CXAudioSource(m_pXAudio, _T("data/SE/Whistle.wav"));
	whistleSE->VolumeAudio(0.6f);
	countDownSE = new CXAudioSource(m_pXAudio, _T("data/SE/CountDown.wav"));
	countDownSE->VolumeAudio(2.5f);
	bonus1SE = new CXAudioSource(m_pXAudio, _T("data/SE/Bonus1.wav"));
	bonus2SE = new CXAudioSource(m_pXAudio, _T("data/SE/Bonus2.wav"));
	whistleUpSE = new CXAudioSource(m_pXAudio, _T("data/SE/WhistleUp.wav"));;
	drumSE = new CXAudioSource(m_pXAudio, _T("data/SE/Drum.wav"));;
	cymbalSE = new CXAudioSource(m_pXAudio, _T("data/SE/Cymbals.wav"));;

	SceneManager::Start();
	ObjectManager::Start();

	//リフレッシュレートの設定
	MainControl::UseFrameTimer(1.0f / 60.0f);

	return S_OK;
}

//------------------------------------------------------------------------
//
//	ゲームのループ処理	
//
//  引数　なし
//
//	戻り値 なし
//
//------------------------------------------------------------------------
void CGameMain::Update()
{
	MyImgui::ImguiNewFrame();   // MyImgui描画前処理    // -- 2020.11.15

	m_pDI->GetInput();			// 入力情報の取得
	m_pFont->Refresh();         // フォント情報のリフレッシュ

	if (m_pDI->CheckKey(KD_TRG, DIK_F4))  ChangeScreenMode();   // フルスクリーンの切り替え       // -- 2018.12.14

//	switch (m_dwGameStatus)
//	{
//	case GAMEEND:			// ゲーム終了
//		PostQuitMessage(0);
//		break;
//	}
	SceneManager::Update();
	ObjectManager::Update();
}

//------------------------------------------------------------------------
//
//	ゲームのメイン処理	
//
//  引数　なし
//
//	戻り値 なし
//
//------------------------------------------------------------------------
void CGameMain::GameMain()
{
}

void CGameMain::Draw()
{
	//画面クリア（実際は単色で画面を塗りつぶす処理）
	float ClearColor[4] = { 0,0,0,1 };// クリア色作成　RGBAの順
	m_pD3D->ClearRenderTarget(ClearColor); // 画面クリア

	SceneManager::Draw();
	ObjectManager::Draw();

	MyImgui::ImguiRender();      // MyImgui実描画処理    // -- 2020.11.15

	//画面更新（バックバッファをフロントバッファに）
	//m_pD3D->m_pSwapChain->Present(1, 0);                   // 60fps Vsync
	m_pD3D->m_pSwapChain->Present(0, 0);                   // Vsyncなし
}

//------------------------------------------------------------------------
//
//	アプリケーションの終了処理	
//
//  引数　なし
//
//	戻り値 なし
//
//------------------------------------------------------------------------
void CGameMain::Quit()
{
	;
}

//------------------------------------------------------------------------    // -- 2018.12.14
//
//	ウィンドウモードとフルスクリーンモードを切り替える処理	
//
//  引数　モード  0:ウインドウモード　1:フルスクリーン　-1:モード切り替え（省略値）
//
//	戻り値 なし
//
//------------------------------------------------------------------------
HRESULT CGameMain::ChangeScreenMode(int nMode)
{
	HRESULT Ret = S_OK;
	BOOL bFullScreen;  // フルスクリーンモードか？

	//	現在のスクリーンモードを得る
	m_pD3D->m_pSwapChain->GetFullscreenState(&bFullScreen, nullptr);

	switch (nMode)
	{
	case 0:    // ウインドウモードにする
		if( bFullScreen ) Ret = m_pD3D->m_pSwapChain->SetFullscreenState(false, nullptr);
		break;
	case 1:    // フルスクリーンモードにする
		if( !bFullScreen ) Ret = m_pD3D->m_pSwapChain->SetFullscreenState(true, nullptr);
		break;	
	default:   // 現在のモードを切り替える
		if (bFullScreen)
		{
			Ret = m_pD3D->m_pSwapChain->SetFullscreenState(false, nullptr);
		}
		else {
			Ret = m_pD3D->m_pSwapChain->SetFullscreenState(true, nullptr);
		}
	}

	return Ret;
}

void CGameMain::SetWindowName(const char* name)
{
	SetWindowText(m_pMain->m_hWnd, name);
}

CGameMain* GameDevice()
{
	return gameMainInstance;
}
