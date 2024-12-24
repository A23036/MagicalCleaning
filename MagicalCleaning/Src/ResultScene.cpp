#include "ResultScene.h"
#include <algorithm>

ResultScene::ResultScene()
{
	dc = ObjectManager::FindGameObject<DataCarrier>();
	rd = new ResultDisplay();

	sprite = new CSprite();
	resultBackImage = new CSpriteImage(_T("data/Image/Result/BackImage.png"));

	bonus1 = -1;
	bonus2 = -1;

	SetRandomBonus(); //ボーナス2つをランダムに決定

	//ボーナスを受け取るプレイヤーの設定
	SetBonusPlayer(bonus1,bonusPlayers1); 
	SetBonusPlayer(bonus2,bonusPlayers2);
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
	if (di->CheckKey(KD_TRG, DIK_T) || di->CheckJoy(KD_TRG, DIJ_B)) {
		SceneManager::ChangeScene("TitleScene");
	}
}

void ResultScene::Draw()
{
	sprite->Draw(resultBackImage, 0, 0, 0, 0, 1280, 720, WINDOW_WIDTH, WINDOW_HEIGHT);

	GameDevice()->m_pFont->Draw(400, 15, _T("RESULTSCENE   Push [T]: TitleScene"), 50, RGB(255, 0, 0));

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
