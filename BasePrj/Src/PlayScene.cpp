#include "PlayScene.h"
#include "Stage.h"
#include "Camera.h"
#include "DataCarrier.h"
#include "PlayDisplay.h"
#include "SplitScreenLastDraw.h"
#include "Player.h"
#include "Dust.h"

PlayScene::PlayScene()
{
	CsvLoad();

	pd = new PlayDisplay();

	dc = SingleInstantiate<DataCarrier>(); //DataCarrier�̓V���O���g���Ő���
	
	new Stage();

	Player* p1 = new Player(0);
	p1->SetPosition(0, 0, 0);
	p1->SetTag("Player1");
	Player* p2 = new Player(1);
	p2->SetPosition(0, 0, 2);
	p2->SetTag("Player2");
	Player* p3 = new Player(2);
	p3->SetPosition(2, 0, 0);
	p3->SetTag("Player3");
	Player* p4 = new Player(3);
	p4->SetPosition(2, 0, 2);
	p4->SetTag("Player4");

	new Dust(0, VECTOR3(10, 1, 0));
	new Dust(1, VECTOR3(10, 1, 10));
	new Dust(2, VECTOR3(10, 1, -10));

	new Camera();
	//new Stage(4);
	
	
	ssld = ObjectManager::FindGameObject<SplitScreenLastDraw>();
	
	timer = 0;
	frm = 0;
	isPlay = false;
	state = sReady;
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	ssld->SetState(state);
	ssld->SetGameTime(timer);
	dc->SetGameState(state);
	dc->SetGameTime(timer);
	dc->SetIsPlay(isPlay);

	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_T)) {
		SceneManager::ChangeScene("TitleScene");
		frm = 0;
		timer = 0;
		isPlay = false;
	}

	switch (state) {
	case sReady:
		UpdateReady();
		break;
	case sPose:
		UpdatePose();
		break;
	case sGamePlay:
		UpdateGamePlay();
		break;
	case sFinish:
		UpdateFinish();
		break;
	}

	if (state != sPose) {
		frm++;
	}
}

void PlayScene::Draw()
{
	GameDevice()->m_pFont->Draw(750, 15, _T("PLAY SCENE"), 32, RGB(255, 0, 0));	
}

void PlayScene::CsvLoad()
{
	// csv����f�[�^�ǂݍ���
	csv = new CsvReader("data/csv/AnimParam.csv");
	if (csv->GetLines() < 1) {
		MessageBox(NULL, "AnimParam.csv���ǂ߂܂���", "�G���[", MB_OK);
	}

	for (int i = 1; i < csv->GetLines(); i++) { //CSV�t�@�C������ݒ�̓ǂݍ���
		if (csv->GetString(i, 0) == "Play") {
			
			if (csv->GetString(i, 1) == "GameTime") {	// �I���������x
				GameTime = csv->GetFloat(i, 3);
			}
		}
	}
}

void PlayScene::UpdateReady()
{
	timer = frm * (1.0f / 60.0f);
	if (timer >= 4) {
		frm = 0;
		state = sGamePlay;
		isPlay = true;
	}
}

void PlayScene::UpdatePose()
{
	if (GameDevice()->m_pDI->CheckJoy(KD_TRG, 2, 0)) {
		state = sGamePlay;
		isPlay = true;
	}
}

void PlayScene::UpdateGamePlay()
{
	timer = GameTime - int(frm * (1.0f / 60.0f));
	
	if (timer <= 0){ //�Q�[���I��
		frm = 0;
		state = sFinish;
		isPlay = false;
	}
	
	//�|�[�Y
	if (GameDevice()->m_pDI->CheckJoy(KD_DAT, 6) && GameDevice()->m_pDI->CheckJoy(KD_DAT, 7)) {
		state = sPose;
		isPlay = false;
	}
}

void PlayScene::UpdateFinish()
{
	timer = frm * (1.0f / 60.0f);
	if (timer >= 3) { //���U���g��
		std::list<Player*> players = ObjectManager::FindGameObjects<Player>();
		int max=0;
		int winner = -1;
		for (Player* pl : players)
		{
			if (max < pl->GetScore()) { //��ԃX�R�A�������v���C���[�̒T��
				max = pl->GetScore();
				winner = pl->GetPlayerNum();
			}
		}
		dc->SetWinnerId(winner); // ���҂�ۑ�
		SceneManager::ChangeScene("ResultScene");
	}
}