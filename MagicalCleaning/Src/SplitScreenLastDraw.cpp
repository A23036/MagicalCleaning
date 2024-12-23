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

	frameUiImage = new CSpriteImage(_T("data/Image/Play/frameUI.png"));
	playUiImage = new CSpriteImage(_T("data/Image/Play/UISprite3.png"));

	sprite = new CSprite();

}

SplitScreenLastDraw::~SplitScreenLastDraw()
{
	SAFE_DELETE(frameUiImage);
	SAFE_DELETE(playUiImage);
	SAFE_DELETE(sprite);
}

void SplitScreenLastDraw::Start()
{
	
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
			sprite->Draw(frameUiImage, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT);


			//4�̃}�b�v�`��
			for (int i = 0; i < 4; i++) {
				sprite->SetSrc(mapImages[st->GetMapData(i)-1], 75, 75, 75, 75);

				int width, height;
				width = sprite->GetSrcWidth();
				height = sprite->GetSrcHeight();
				float pivotX = width / 2.0f;
				float pivotY = height / 2.0f;

				MATRIX4X4 m = XMMatrixTranslation(-pivotX, -pivotY, 0)
					* XMMatrixRotationZ(st->GetMapRotData(i) * 90 * DegToRad)
					* XMMatrixScaling(1.0f, 1.0f, 1.0f)
					* XMMatrixTranslation(646 + i%2 * 75, 444 - i/2 * 75, 0);
				
				// �X�v���C�g�����[���h�s����g�p���ĕ`��
				sprite->Draw(m);
			}

			//player�ʒu�\��
			int posX,posY;

			for (Player* pl : players){
				if (pl->GetIsInvisible())
				{
					continue;
				}
				
				//���[ 600 �E�[ 750
				posX = 600;
				//��[ 320 ���[ 475
				posY = 475;

				// �~�j�}�b�v�̕`��͈� (���[, �E�[, ��[, ���[)
				const int mapLeft = 600;
				const int mapRight = 750;
				const int mapTop = 320;
				const int mapBottom = 475;

				// ���[���h���W�n�͈̔� (-50 ~ 50)
				const float worldMin = -50.0f;
				const float worldMax = 50.0f;

				// �~�j�}�b�v���X���W�v�Z (���[���hX���~�j�}�b�v�͈̔͂ɕϊ�)
				float normalizedX = (pl->Position().x - worldMin) / (worldMax - worldMin); // ���K�� (0~1)
				int posX = mapLeft + static_cast<int>(normalizedX * (mapRight - mapLeft)); // �}�b�v���W

				// �~�j�}�b�v���Y���W�v�Z (���[���hZ���~�j�}�b�v�͈̔͂ɕϊ�)
				float normalizedZ = (pl->Position().z - worldMin) / (worldMax - worldMin); // ���K�� (0~1)
				int posY = mapBottom - static_cast<int>(normalizedZ * (mapBottom - mapTop)); // �}�b�v���W

				//�v���C���[���Ƃ̃A�C�R���J���[�ύX
				sprite->SetSrc(playUiImage,1 + pl->GetColor() * 16, 1, 15, 16, 15, 16);
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

				// �X�v���C�g�����[���h�s����g�p���ĕ`��
				sprite->Draw(m);
			}

			 
			//sprite->DrawRect()

			//�Q�[�����ԕ\��
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

void SplitScreenLastDraw::SetPlayers(vector<Player*> players)
{
	this->players = players;
}

void SplitScreenLastDraw::LoadMap()
{
	st = ObjectManager::FindGameObject<Stage>();

	for (int i = 0; i < st->GetMapNum(); i++) {//�}�b�v�f�[�^�̐������C���[�W�f�[�^�����[�h
		CSpriteImage* s;
		std::string f = "data/Image/Play/map/stage" + std::to_string(i + 1);
		s = new CSpriteImage(_T((f + ".png").c_str()));
		mapImages.push_back(s);
	}
}
