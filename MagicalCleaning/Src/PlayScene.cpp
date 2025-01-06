#include "PlayScene.h"
#include "Stage.h"
#include "Camera.h"
#include "DataCarrier.h"
#include "PlayDisplay.h"
#include "SplitScreenLastDraw.h"
#include "Player.h"
#include "Dust.h"
#include "Sky.h"

PlayScene::PlayScene()
{
	CsvLoad();

	pd = new PlayDisplay();

	dc = ObjectManager::FindGameObject<DataCarrier>();
	
	st = new Stage();
	sky = new Sky();

	p1 = new Player(0, dc->GetColor(0));
	p1->SetPosition(0, 0, -45);
	p1->SetTag("Player1");
	p2= new Player(1, dc->GetColor(1));
	p2->SetPosition(-45, 0, 0);
	p2->SetTag("Player2");
	p3 = new Player(2, dc->GetColor(2));
	p3->SetPosition(0, 0, 45);
	p3->SetTag("Player3");
	p4 = new Player(3, dc->GetColor(3));
	p4->SetPosition(45, 0, 0);
	p4->SetTag("Player4");

	cm = new Camera();
	
	Dust* d = new Dust(3 ,VECTOR3(0, 1, 0));
	
	ssld = ObjectManager::FindGameObject<SplitScreenLastDraw>();
	
	players.push_back(p1);
	players.push_back(p2);
	players.push_back(p3);
	players.push_back(p4);

	ssld->SetPlayers(players);

	timer = 0;
	frm = 0;
	isPlay = false;
	state = sTransition;
}

PlayScene::~PlayScene()
{
	SAFE_DELETE(csv);
}

void PlayScene::Update()
{
	ssld->SetState(state);
	ssld->SetGameTime(timer);
	dc->SetGameState(state);
	dc->SetGameTime(timer);
	dc->SetIsPlay(isPlay);

	SetFinalRank();	//順位計算
	dc->SetRank(rank);	//データ格納

	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_T)) {
		SceneManager::ChangeScene("TitleScene");
		frm = 0;
		timer = 0;
		isPlay = false;
	}

	switch (state) {
	case sTransition:
		UpdateTransition();
		break;
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
	//GameDevice()->m_pFont->Draw(750, 15, _T("PLAY SCENE"), 32, RGB(255, 0, 0));	
}

void PlayScene::CsvLoad()
{
	// csvからデータ読み込み
	csv = new CsvReader("data/csv/AnimParam.csv");
	if (csv->GetLines() < 1) {
		MessageBox(NULL, "AnimParam.csvが読めません", "エラー", MB_OK);
	}

	for (int i = 1; i < csv->GetLines(); i++) { //CSVファイルから設定の読み込み
		if (csv->GetString(i, 0) == "Play") {
			
			if (csv->GetString(i, 1) == "GameTime") {	// 終了時透明度
				GameTime = csv->GetFloat(i, 3);
			}
		}
	}
}

void PlayScene::UpdateTransition()
{
	if (ssld->GetIsTransFinish()) {
		frm = 0;
		timer = 0;
		ssld->SetIsTransFinish(false);
		state = sReady;
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

	// 配列の初期化と Dust の生成
	if (dustArray.empty())
	{
		for (int i = 0; i < MAX_DUST_NUM; ++i)
		{
			int size = GetRandomSize();
			VECTOR3 position = GetRandomPosition();
			dustArray.push_back(new Dust(size, position));
		}
	}
}

void PlayScene::UpdatePose()
{
	if (GameDevice()->m_pDI->CheckJoy(KD_TRG, 9, 0)) {
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

	

	// インスタンスがなくなったら再生成
	if (dustArray.size() < MAX_DUST_NUM) {
		for (int i = 0; i < (MAX_DUST_NUM - dustArray.size()); i++) {
			int size = GetRandomSize();
			VECTOR3 position = GetRandomPosition();
			Dust* d = new Dust(size, position);
			dustArray.push_back(d);
		}
	}
	
	//ポーズ
	if (GameDevice()->m_pDI->CheckJoy(KD_TRG, 9)) {
		state = sPose;
		isPlay = false;
	}
}

void PlayScene::UpdateFinish()
{
	timer = frm * (1.0f / 60.0f);
	if (timer >= 4) { //リザルトへ
		std::list<Player*> players = ObjectManager::FindGameObjects<Player>();
		int max=0;
		int winner = -1;
		for (Player* pl : players)
		{
			if (max < pl->GetScore()) { //一番スコアが高いプレイヤーの探索
				max = pl->GetScore();
				winner = pl->GetPlayerNum();
			}
			dc->SetScore(pl->GetScore(),pl->GetPlayerNum()); //スコアの保存
			for (int i = 0; i < 5; i++) {
				dc->SetPowerLevel(i, pl->GetPowerLv(i), pl->GetPlayerNum()); //各能力レベルの保存
			}
			//各ボーナス用変数のセット
			dc->SetMoveDistance(pl->GetMoveDistance(),pl->GetPlayerNum());
			dc->SetJumpCount(pl->GetJumpCount(), pl->GetPlayerNum());
			dc->SetKnockOutCount(pl->GetKnockOutCount(), pl->GetPlayerNum());
			dc->SetItemCount(pl->GetItemCount(), pl->GetPlayerNum());
			dc->SetCleanReafCount(pl->GetCleanReafCount(), pl->GetPlayerNum());
			dc->SetBlowCount(pl->GetBlowCount(), pl->GetPlayerNum());
		}
		
		SceneManager::ChangeScene("ResultScene");
	}
}

VECTOR3 PlayScene::GetRandomPosition()
{
	int x = Random(-40,40);		// -40 ~ 40
	int z = Random(-40, 40);	// -40 ~ 40
	int y = Random(10, 15);		// 10~15
	return VECTOR3(x, y, z);
}

int PlayScene::GetRandomSize()
{
	int size = Random(0, 20);
	if (size > 19) {
		return 2;
	}
	else if (size > 15) {
		return 1;
	}
	else {
		return 0;
	}
}

void PlayScene::DustDestroyed(Dust* dust)
{
	if (dust->GetNum() == 3) { //透明化アイテム
		VECTOR3 pos = VECTOR3(GetRandomPosition());
		new Dust(3, pos);
		return;
	}
	// 配列内から該当の Dust を削除
	auto it = std::find(dustArray.begin(), dustArray.end(), dust);
	if (it != dustArray.end())
	{
		dustArray.erase(it); // 配列から削除
	}
}

void PlayScene::SetFinalRank()
{
	int i = 0, j = 0, max = -1;
	int arr[MAXPLAYER];
	arr[0] = p1->GetScore();
	arr[1] = p2->GetScore();
	arr[2] = p3->GetScore();
	arr[3] = p4->GetScore();

	for (i = 0; i < MAXPLAYER; i++) {
		rank[i] = 1;
		for (j = 0; j < MAXPLAYER; j++) {
			if (arr[j] > arr[i]) { //自分より大きいスコアがある場合、順位を1つ下げる
				rank[i]++; 
			}
		}
	}
}
