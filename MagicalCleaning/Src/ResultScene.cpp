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

	SetRandomBonus(); //�{�[�i�X2�������_���Ɍ���

	//�{�[�i�X���󂯎��v���C���[�̐ݒ�
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
	// �e�v���C���[�̐�����΂��񐔂�z��Ɋi�[
	for (int i = 0; i < MAXPLAYER; i++) {
		blowCount[i] = dc->GetBlowCount(i);
	}

	// ������΂��񐔂������Ƀ\�[�g
	std::sort(blowCount, blowCount + MAXPLAYER);

	// �ŏ��l��2�Ԗڂɏ������l���擾
	int min1 = blowCount[0];
	int min2 = blowCount[1];

	// �ŏ��l��2�Ԗڂɏ������l�̍����傫���������A�{�[�i�X�Ƃ��đI�o���m��
	if (min2 - min1 > 5) {
		bonus1 = bBlow;
		bonus2 = Random(0, 4);
	}
	else {
		// �{�[�i�X2�������_���Ɍ���
		bonus1 = Random(0, 5);
		bonus2 = Random(0, 5);

		//bonus2��bonus1�ƈقȂ���̂�I��
		while (bonus1 == bonus2) {
			bonus2 = Random(0, 5);
		}
	}
}

void ResultScene::SetBonusPlayer(int bonus, std::vector<int> &players)
{
	float max = -1.0f;

	players.clear(); // �֐��Ăяo�����ɔz����N���A

	for (int i = 0; i < MAXPLAYER; i++) {
		switch (bonus){
		case bMove:
			if (dc->GetMoveDistance(i) >= max) { 
				if (max == dc->GetMoveDistance(i)) { //�����̓v���C���[��ǉ�
					players.push_back(i);
				}
				else { //�ő�l���X�V���ꂽ�Ƃ�
					max = dc->GetMoveDistance(i);
					players.clear(); //�z����N���A
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
