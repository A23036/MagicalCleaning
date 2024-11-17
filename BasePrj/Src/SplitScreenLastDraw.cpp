#include "SplitScreenLastDraw.h"
#include "PlayScene.h"
#include "Camera.h"
#include "SplitScreen.h"


SplitScreenLastDraw::SplitScreenLastDraw()
{
	ObjectManager::DontDestroy(this);		// ������Ȃ�
	ObjectManager::SetDrawOrder(this, -100000);	// ��ԍŌ�ɕ`�揈������

	D3D11_VIEWPORT vp = {};

	// �P���	--------------------------
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
		// ����ʂ̂Ƃ�
		if (ObjectManager::DrawCounter() == ss->MultiSize() - 1)   // �Ō�̉�ʂ̂Ƃ�
		{
			// �P��ʂɖ߂�
			GameDevice()->m_pD3D->m_pDeviceContext->RSSetViewports(1, &vpSingle);
			MATRIX4X4 saveProj = GameDevice()->m_mProj;		 	  // �v���W�F�N�V�����}�g���b�N�X��Ҕ�
			GameDevice()->m_mProj = GameDevice()->m_mProjStandard;	  // �c���� �W��

			// �y�o�b�t�@�̂ݑS�N�����[����B�i��ʂ͏������ɁA�y�o�b�t�@�̂݃N�����[����j
			// ���̏����́A�����łR�c�I�u�W�F�N�g�̕`����s���Ƃ��͕K�{
			// �Q�c�X�v���C�g�̂ݕ`����s���Ƃ��͕s�v
			//if (GameDevice()->m_pD3D->m_pTarget_DSTexDSV){
			//    GameDevice()->m_pD3D->m_pDeviceContext->ClearDepthStencilView(GameDevice()->m_pD3D->m_pTarget_DSTexDSV, D3D11_CLEAR_DEPTH, 1.0f, 0); // �[�x�o�b�t�@�N���A
			//}

			GameDevice()->m_vEyePt = cm->EyePt(0); // �J�������W
			GameDevice()->m_vLookatPt = cm->LookatPt(0); // �����_
			GameDevice()->m_mView = cm->View(0);   // �r���[�}�g���b�N�X

			// -----------------------------------------------------------------

			// ��ʑS�̂̕`��

			//��ʕ����g�`��
			sprite->Draw(frameUiImage, 0, 0, 1024, 576, 1024, 576, WINDOW_WIDTH, WINDOW_HEIGHT);

			//�Q�[�����ԕ\��
			int minutes = gameTime / 60;
			int seconds = gameTime % 60;

			char str[64];

			switch (gameState) {
			case sReady:
				if (seconds <= 2) { //�J�E���g�_�E������
					sprintf_s(str, "%d", 3 - seconds);
					GameDevice()->m_pFont->Draw(WINDOW_WIDTH / 2 - 33, WINDOW_HEIGHT / 2 - 98, str, 100, RGB(0, 0, 255));
				}
				else {	//�uGO!�v
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
			GameDevice()->m_mProj = saveProj;	  // �v���W�F�N�V�����}�g���b�N�X�����ɖ߂�
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
