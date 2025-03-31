#include "PlayScene.h"
#include "Stage.h"
#include "Camera.h"
#include "DataCarrier.h"
#include "PlayDisplay.h"
#include "SplitScreenLastDraw.h"
#include "Player.h"
#include "Leaf.h"
#include "Sky.h"
#include "PlayDisplay.h"
#include "CsvReader.h"

// ---------------------------------------------------------------------------
// コンストラクタ
// ---------------------------------------------------------------------------
PlayScene::PlayScene()
{
	//DataCarrierのポインタを取得
	dc = ObjectManager::FindGameObject<DataCarrier>();

	//初期化処理
	Init();

	pd = new PlayDisplay();
	st = new Stage();
	sky = new Sky();
	cm = new Camera();
	
	ssld = ObjectManager::FindGameObject<SplitScreenLastDraw>();
	ssld->SetPlayers(players);
}

// ---------------------------------------------------------------------------
// デストラクタ
// ---------------------------------------------------------------------------
PlayScene::~PlayScene()
{
	SAFE_DELETE(csv);
}

// ---------------------------------------------------------------------------
// 各変数の初期化処理
// ---------------------------------------------------------------------------
void PlayScene::Init()
{
	//定数のCSV読み込み
	CsvLoad();

	//各プレイヤーを生成、ポインタを配列に格納
	p1 = new Player(0, dc->GetColor(0));
	p2 = new Player(1, dc->GetColor(1));
	p3 = new Player(2, dc->GetColor(2));
	p4 = new Player(3, dc->GetColor(3));
	players.push_back(p1);
	players.push_back(p2);
	players.push_back(p3);
	players.push_back(p4);

	//各プレイヤー位置の初期化、タグの設定
	for (int i = 0; i < MAXPLAYER; i++) {
		string str = "Player" + to_string(i+1);
		players[i]->SetPosition(InitPos[i].x, InitPos[i].y, InitPos[i].z);
		players[i]->SetTag(str);
	}

	//アイテムリーフの初期生成(ステージ中央)
	Leaf* leaf = new Leaf(3, VECTOR3(0, 1, 0));

	timer = 0;
	frm = 0;
	isPlay = false;
	state = sTransition;
	//state = sReady;
}

// ---------------------------------------------------------------------------
// プレイ画面の更新処理
// 
// 経過時間などによるゲーム状態の遷移、各プレイヤーの順位計算などを行う
// ---------------------------------------------------------------------------
void PlayScene::Update()
{
	//Tキーの入力でタイトル画面へ遷移
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_T)) {
		SceneManager::ChangeScene("TitleScene");
		GameDevice()->playBGM->Stop();
		GameDevice()->spurtBGM->Stop();
		frm = 0;
		timer = 0;
		isPlay = false;
	}

	//ゲーム状態ごとの処理
	switch (state) {
	case sTransition:
		UpdateTransition();
		if (state == sReady) {
			return;
		}
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

	//ポーズ中でなければ経過時間を加算
	if (state != sPose) {
		frm++;
	}

	//SplitScreenLastDrawで表示する時間のセット
	ssld->SetState(state);
	ssld->SetGameTime(timer);

	//DataCarrierへゲーム情報の登録
	dc->SetGameState(state);
	dc->SetGameTime(timer);
	dc->SetIsPlay(isPlay);

	SetFinalRank();		//順位計算
	dc->SetRank(rank);	//順位データをセット
}

// ---------------------------------------------------------------------------
// プレイ画面の更新処理サブ関数
// 
// トランジション中の処理を行う
// ---------------------------------------------------------------------------
void PlayScene::UpdateTransition()
{
	if (ssld->GetIsTransFinish()) {
		frm = 0;
		timer = 0;
		ssld->SetIsTransFinish(false);
		state = sReady;
	}
}

// ---------------------------------------------------------------------------
// プレイ画面の更新処理サブ関数
// 
// カウントダウン中の処理、リーフの初期生成を行う
// ---------------------------------------------------------------------------
void PlayScene::UpdateReady()
{
	timer = frm * (1.0f / 60.0f);
	if (timer >= 4) {
		frm = 0;
		timer = GameTime;
		state = sGamePlay;
		isPlay = true;
	}

	// 配列の初期化とLeafの生成
	if (leafArray.empty())
	{
		//各プレイヤーのスタート位置前方に固定出現
		leafArray.push_back(new Leaf(1, VECTOR3(0, 10, -40)));
		leafArray.push_back(new Leaf(1, VECTOR3(-40, 10, 0)));
		leafArray.push_back(new Leaf(1, VECTOR3(0, 10, 40)));
		leafArray.push_back(new Leaf(1, VECTOR3(40, 10, 0)));

		for (int i = 4; i < MaxLeafNum; ++i)
		{
			int size = GetRandomSize();
			VECTOR3 position = GetRandomPosition();
			leafArray.push_back(new Leaf(size, position));
		}
	}
}

// ---------------------------------------------------------------------------
// プレイ画面の更新処理サブ関数
// 
// ポーズ中の処理
// ---------------------------------------------------------------------------
void PlayScene::UpdatePose()
{
	if (GameDevice()->m_pDI->CheckJoy(KD_TRG, 9, 0)) {
		state = sGamePlay;
		isPlay = true;
	}
}

// ---------------------------------------------------------------------------
// プレイ画面の更新処理サブ関数
// 
// ゲーム中の処理、リーフの再生成を行う
// ---------------------------------------------------------------------------
void PlayScene::UpdateGamePlay()
{
	timer = GameTime - int(frm * (1.0f / 60.0f));
	
	if (timer <= 0){ //ゲーム終了
		GameDevice()->whistleSE->Play();
		frm = 0;
		state = sFinish;
		isPlay = false;
	}

	// leafのインスタンスがなくなったら再生成
	if (leafArray.size() < MaxLeafNum) {
		for (int i = 0; i < (MaxLeafNum - leafArray.size()); i++) {
			int size = GetRandomSize();
			VECTOR3 position = GetRandomPosition();
			Leaf* d = new Leaf(size, position);
			leafArray.push_back(d);
		}
	}
	
	//ポーズ
	if (GameDevice()->m_pDI->CheckJoy(KD_TRG, 9)) {
		state = sPose;
		isPlay = false;
	}
}

// ---------------------------------------------------------------------------
// プレイ画面の更新処理サブ関数
// 
// ゲーム終了時の処理、順位計算とボーナス用変数の格納を行う
// ---------------------------------------------------------------------------
void PlayScene::UpdateFinish()
{
	timer = frm * (1.0f / 60.0f);
	if (timer >= 4) { //リザルトへ
		std::list<Player*> players = ObjectManager::FindGameObjects<Player>();
		int max = 0;
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
		
		GameDevice()->playBGM->Stop();
		GameDevice()->spurtBGM->Stop();
		SceneManager::ChangeScene("ResultScene");
	}
}

// ---------------------------------------------------------------------------
// リーフのランダムな配置を返す関数
// 
// CSVファイルから取得した座標をもとにリーフのランダムな位置を返す
// ---------------------------------------------------------------------------
VECTOR3 PlayScene::GetRandomPosition()
{
	int x = Random(LeafSpawnRangeMin, LeafSpawnRangeMax);
	int z = Random(LeafSpawnRangeMin, LeafSpawnRangeMax);
	int y = Random(LeafSpawnHeightMin, LeafSpawnHeightMax);
	return VECTOR3(x, y, z);
}

// ---------------------------------------------------------------------------
// リーフのサイズをランダムに返す関数
// 
// CSVファイルから取得した確率をもとにリーフのランダムなサイズを返す
// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
// リーフの消去時の処理
// 
// CSVファイルから取得した確率をもとにリーフのランダムなサイズを返す
// ---------------------------------------------------------------------------
void PlayScene::LeafDestroyed(Leaf* leaf)
{
	if (leaf->GetNum() == 3) { //透明化アイテム
		VECTOR3 pos = VECTOR3(GetRandomPosition());
		new Leaf(3, pos);
		return;
	}
	// 配列内から該当のLeafを削除
	auto it = std::find(leafArray.begin(), leafArray.end(), leaf);
	if (it != leafArray.end())
	{
		leafArray.erase(it); // 配列から削除
	}
}

// ---------------------------------------------------------------------------
// プレイヤーのスコアから順位の計算を行う関数
// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
// CSV読み込み処理
// 
// プレイ画面での処理に使用する定数のCSV読み込みを行う
// ---------------------------------------------------------------------------
void PlayScene::CsvLoad()
{
	CsvPlayDataLoader dataLoader("data/csv/SceneParam.csv");
	dataLoader.Load();
	playParams = dataLoader.GetPlayParams();

	//読み込んだ構造体からデータの代入
	GameTime = playParams.GameTime;
	MaxLeafNum = playParams.MaxLeafNum;
	LeafSpawnRangeMin = playParams.LeafSpawnRangeMin;	
	LeafSpawnRangeMax = playParams.LeafSpawnRangeMax;
	LeafSpawnHeightMin = playParams.LeafSpawnHeightMin;
	LeafSpawnHeightMax = playParams.LeafSpawnHeightMax;

	for (int i = 0; i < MAXPLAYER; i++) {
		InitPos[i] = playParams.InitPos[i];
	}
}