//=============================================================================
//		�R�c�Q�[���v���O����
//		�b�R�c�`�����P�P�P�@�@�@�@�@�@                 ver 3.2        2023.1.31
//
//		�Q�[���̃��C������
//																GameMain.h
//=============================================================================

#pragma once

//�w�b�_�[�t�@�C���̃C���N���[�h
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
//  �Q�[�����C���N���X
// --------------------------------------------------------------------------------------------------
class CGameMain
{
public:
	// �����o�ϐ�
	CMain*			m_pMain;		// ���C���N���X
	CDirect3D*		m_pD3D;			//	Direct3D�I�u�W�F�N�g
	CXAudio*		m_pXAudio;		//	XAudio�I�u�W�F�N�g
	CDirectInput*	m_pDI;			//	DirectInputD�I�u�W�F�N�g
	CFontTexture*	m_pFont;		//	�t�H���g�e�N�X�`���[
	CShader*		m_pShader;		//	�V�F�[�_�[
	CFbxMeshCtrl*	m_pFbxMeshCtrl;	// ���b�V���R���g���[���N���X    // -- 2021.2.4

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

	
	// �J�����E���C�g�E�r���[
	VECTOR3         m_vEyePt;		//�J�����i���_�j�ʒu
	VECTOR3         m_vLookatPt;	//�����ʒu
	MATRIX4X4       m_mView;
	MATRIX4X4       m_mProj;
	MATRIX4X4       m_mProjStandard;	  // �W��
	MATRIX4X4       m_mProjVerticalLong;  // �c��
	VECTOR3         m_vLightDir;	//�f�B���N�V���i�����C�g�̕���
public:
	// ���\�b�h
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
