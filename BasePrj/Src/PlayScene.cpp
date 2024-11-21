#include "PlayScene.h"
#include "Stage.h"
#include "Camera.h"
#include "DataCarrier.h"
#include "PlayDisplay.h"
#include "SplitScreenLastDraw.h"
#include "Player.h"

PlayScene::PlayScene()
{
	pd = new PlayDisplay();

	dc = SingleInstantiate<DataCarrier>(); //DataCarrierはシングルトンで生成
	
	new Camera();
	new Stage(5);
	
	ssld = ObjectManager::FindGameObject<SplitScreenLastDraw>();
	
	timer = 0;
	frm = 0;
	isPlay = false;
	state = sReady;
	GameTime = 10;
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	ssld->SetState(state);
	ssld->SetGameTime(timer);
	dc->SetGameState(state);
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
	
	if (timer <= 0){ //ゲーム終了
		frm = 0;
		state = sFinish;
		isPlay = false;
	}
	
	//ポーズ
	if (GameDevice()->m_pDI->CheckJoy(KD_DAT, 6) && GameDevice()->m_pDI->CheckJoy(KD_DAT, 7)) {
		state = sPose;
		isPlay = false;
	}
}

void PlayScene::UpdateFinish()
{
	timer = frm * (1.0f / 60.0f);
	if (timer >= 3) { //リザルトへ
		std::list<Player*> players = ObjectManager::FindGameObjects<Player>();
		int max=0;
		int winner = -1;
		for (Player* pl : players)
		{
			if (max < pl->GetScore()) { //一番スコアが高いプレイヤーの探索
				max = pl->GetScore();
				winner = pl->GetPlayerNum();
			}
		}
		dc->SetWinnerId(winner); // 勝者を保存
		SceneManager::ChangeScene("ResultScene");
	}
}