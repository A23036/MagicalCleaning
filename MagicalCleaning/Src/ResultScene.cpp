#include "ResultScene.h"
#include "PlayScene.h"
#include <algorithm>

ResultScene::ResultScene()
{
	dc = ObjectManager::FindGameObject<DataCarrier>();
	
	sprite = new CSprite();
	resultBackImage = new CSpriteImage(_T("data/Image/Result/BackImage.png"));

	bonus1 = -1;
	bonus2 = -1;

	SetRandomBonus(); //ボーナス2つをランダムに決定

	//ボーナスを受け取るプレイヤーの設定
	SetBonusPlayer(bonus1,bonusPlayers1);
	//ボーナス分スコアを加算
	for (int i = 0; i < bonusPlayers1.size(); i++) {
		dc->SetScore(dc->GetScore(bonusPlayers1[i]) + 20, bonusPlayers1[i]);
	}
	//二つ目のボーナス
	SetBonusPlayer(bonus2,bonusPlayers2);
	for (int i = 0; i < bonusPlayers2.size(); i++) {
		dc->SetScore(dc->GetScore(bonusPlayers2[i]) + 20, bonusPlayers2[i]);
	}

	//ボーナス加算分を考慮した最終的な順位の計算
	SetFinalRank();

	//キャラクターの配置
	p1 = new Player(VECTOR3(-4, -1, 0), VECTOR3(0, 180 * DegToRad, 0), dc->GetColor(0));
	p1->SetScale(2.0f, 2.0f, 2.0f);
	p2 = new Player(VECTOR3(-1.3f, -1, 0), VECTOR3(0, 180 * DegToRad, 0), dc->GetColor(1));
	p2->SetScale(2.0f, 2.0f, 2.0f);
	p3 = new Player(VECTOR3(1.3f, -1, 0), VECTOR3(0, 180 * DegToRad, 0), dc->GetColor(2));
	p3->SetScale(2.0f, 2.0f, 2.0f);
	p4 = new Player(VECTOR3(4, -1, 0), VECTOR3(0, 180 * DegToRad, 0), dc->GetColor(3));
	p4->SetScale(2.0f, 2.0f, 2.0f);

	//UI表示用クラスの生成
	rd = new ResultDisplay();
	//ボーナスを受け取るプレイヤーの設定
	rd->SetBonusID(bonus1, bonus2);
	rd->SetBonusPlayer(bonusPlayers1, bonusPlayers2);

}

ResultScene::~ResultScene()
{
	SAFE_DELETE(sprite);
	SAFE_DELETE(resultBackImage);
}

void ResultScene::Update()
{
	auto di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_TRG, DIK_T) || di->CheckJoy(KD_TRG, 2, 0)) {
		if (rd->GetIsFinish()) {
			SceneManager::ChangeScene("TitleScene");
			GameDevice()->resultBGM->Stop();
		}
	}
}

void ResultScene::Draw()
{
	sprite->Draw(resultBackImage, 0, 0, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);

	GameDevice()->m_mView = XMMatrixLookAtLH(
		VECTOR3(0, 0, -10), // カメラ位置
		VECTOR3(0, 0, 0), // 注視点
		VECTOR3(0, 1, 0)); // 上ベクトル
}

void ResultScene::SetRandomBonus()
{
	int blowCount[MAXPLAYER];
	// 各プレイヤーの吹っ飛ばし回数を配列に格納
	for (int i = 0; i < MAXPLAYER; i++) {
		blowCount[i] = dc->GetBlowCount(i);
	}

	// 吹っ飛ばし回数を昇順にソート
	std::sort(blowCount, blowCount + MAXPLAYER);

	// 最小値と2番目に小さい値を取得
	int min1 = blowCount[0];
	int min2 = blowCount[1];

	// 最小値と2番目に小さい値の差が大きかった時、ボーナスとして選出を確定
	if (min2 - min1 > 5) {
		bonus1 = bBlow;
		bonus2 = Random(0, 4);
	}
	else {
		// ボーナス2つをランダムに決定
		bonus1 = Random(0, 5);
		bonus2 = Random(0, 5);

		//bonus2はbonus1と異なるものを選ぶ
		while (bonus1 == bonus2) {
			bonus2 = Random(0, 5);
		}
	}
}

void ResultScene::SetBonusPlayer(int bonus, std::vector<int> &players)
{
	float max = -1.0f;

	players.clear(); // 関数呼び出し時に配列をクリア

	for (int i = 0; i < MAXPLAYER; i++) {
		switch (bonus){
		case bMove:
			if (dc->GetMoveDistance(i) >= max) { 
				if (max == dc->GetMoveDistance(i)) { //同数はプレイヤーを追加
					players.push_back(i);
				}
				else { //最大値が更新されたとき
					max = dc->GetMoveDistance(i);
					players.clear(); //配列をクリア
					players.push_back(i);
				}
			}
			break;
		case bJump:
			if (dc->GetJumpCount(i) >= max) {
				if (max == dc->GetJumpCount(i)) {
					players.push_back(i);
				}
				else {
					max = dc->GetJumpCount(i);
					players.clear();
					players.push_back(i);
				}
			}
			break;
		case bKnock:
			if (dc->GetKnockOutCount(i) >= max) {
				if (max == dc->GetKnockOutCount(i)) {
					players.push_back(i);
				}
				else {
					max = dc->GetKnockOutCount(i);
					players.clear();
					players.push_back(i);
				}
			}
			break;
		case bItem:
			if (dc->GetItemCount(i) >= max) {
				if (max == dc->GetItemCount(i)) {
					players.push_back(i);
				}
				else {
					max = dc->GetItemCount(i);
					players.clear();
					players.push_back(i);
				}
			}
			break;
		case bClean:
			if (dc->GetCleanReaf(i) >= max) {
				if (max == dc->GetCleanReaf(i)) {
					players.push_back(i);
				}
				else {
					max = dc->GetCleanReaf(i);
					players.clear();
					players.push_back(i);
				}
			}
			break;
		case bBlow:
			if (dc->GetBlowCount(i) >= max) {
				if (max == dc->GetBlowCount(i)) {
					players.push_back(i);
				}
				else {
					max = dc->GetBlowCount(i);
					players.clear();
					players.push_back(i);
				}
			}
			break;
		default:
			break;
		}
	}
}

void ResultScene::SetFinalRank()
{
	int i = 0, j = 0, max = -1;
	int arr[MAXPLAYER],rank[MAXPLAYER];
	for (int i = 0; i < MAXPLAYER; i++) {
		arr[i] = dc->GetScore(i);
	}

	for (i = 0; i < MAXPLAYER; i++) {
		rank[i] = 1;
		for (j = 0; j < MAXPLAYER; j++) {
			if (arr[j] > arr[i]) { //自分より大きいスコアがある場合、順位を1つ下げる
				rank[i]++;
			}
		}
	}

	for (int i = 0; i < MAXPLAYER; i++) {
		dc->SetRank(rank);
	}
}
