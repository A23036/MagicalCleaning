//=============================================================================
//		３Ｄゲームプログラム
//		Ｃ３ＤＡｃｔ１１１　　　　　　                 ver 3.2        2023.1.31
//
//		ゲームのメイン処理
//																GameMain.h
//=============================================================================

#pragma once

//ヘッダーファイルのインクルード
#include <stdio.h>
#include <windows.h>
#include <vector>

#include "Main.h"
#include "Direct3D.h"
#include "Shader.h"
#include "FbxMesh.h"
#include "XAudio.h"
#include "Dinput.h"
#include "Sprite3D.h"
#include "MyImgui.h"
#include "Macro.h"

#include "MainControl.h"

// --------------------------------------------
class CMain;
class CDirect3D;
class CXAudio;
class CDirectInput;
class CFontTexture;
class CShader;
class CFbxMeshCtrl;

// --------------------------------------------------------------------------------------------------
//  ゲームメインクラス
// --------------------------------------------------------------------------------------------------
class CGameMain
{
public:
	// メンバ変数
	CMain*			m_pMain;		// メインクラス
	CDirect3D*		m_pD3D;			//	Direct3Dオブジェクト
	CXAudio*		m_pXAudio;		//	XAudioオブジェクト
	CDirectInput*	m_pDI;			//	DirectInputDオブジェクト
	CFontTexture*	m_pFont;		//	フォントテクスチャー
	CShader*		m_pShader;		//	シェーダー
	CFbxMeshCtrl*	m_pFbxMeshCtrl;	// メッシュコントロールクラス    // -- 2021.2.4

	DWORD           m_dwGameStatus;

	// BGM
	CXAudioSource* titleBGM;
	CXAudioSource* selectBGM;
	CXAudioSource* playBGM;
	CXAudioSource* spurtBGM;
	CXAudioSource* resultBGM;
	//SE
	CXAudioSource* gameStartSE;
	CXAudioSource* swingSE;
	CXAudioSource* downSE;
	CXAudioSource* scaleUpSE;
	CXAudioSource* colorSelectSE;
	CXAudioSource* decisionSE;
	CXAudioSource* backSE;
	CXAudioSource* entrySE;
	CXAudioSource* cancelSE;
	CXAudioSource* jumpSE;
	CXAudioSource* attack1SE;
	CXAudioSource* attack2SE;
	CXAudioSource* attack3SE;
	CXAudioSource* powerUpSE;
	CXAudioSource* powerSelectSE;
	CXAudioSource* blowSE;
	CXAudioSource* lastSpurtSE;
	CXAudioSource* blowCheerSE;
	CXAudioSource* magicCircleSE;
	CXAudioSource* chargeSE;
	CXAudioSource* itemSE;
	CXAudioSource* timeSE;
	CXAudioSource* whistleSE;
	CXAudioSource* countDownSE;

	
	// カメラ・ライト・ビュー
	VECTOR3         m_vEyePt;		//カメラ（視点）位置
	VECTOR3         m_vLookatPt;	//注視位置
	MATRIX4X4       m_mView;
	MATRIX4X4       m_mProj;
	MATRIX4X4       m_mProjStandard;	  // 標準
	MATRIX4X4       m_mProjVerticalLong;  // 縦長
	VECTOR3         m_vLightDir;	//ディレクショナルライトの方向
public:
	// メソッド
	CGameMain(CMain* pMain);
	~CGameMain();

	HRESULT Init();
	void    Update();
	void	Draw();
	void    Quit();

	void    GameMain();
	HRESULT ChangeScreenMode(int nMode=-1);    // -- 2020.1.15

	void SetWindowName(const char* name);
};

CGameMain* GameDevice();
