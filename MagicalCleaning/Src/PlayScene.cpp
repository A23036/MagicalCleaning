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
// �R���X�g���N�^
// ---------------------------------------------------------------------------
PlayScene::PlayScene()
{
	//DataCarrier�̃|�C���^���擾
	dc = ObjectManager::FindGameObject<DataCarrier>();

	//����������
	Init();

	pd = new PlayDisplay();
	st = new Stage();
	sky = new Sky();
	cm = new Camera();
	
	ssld = ObjectManager::FindGameObject<SplitScreenLastDraw>();
	ssld->SetPlayers(players);
}

// ---------------------------------------------------------------------------
// �f�X�g���N�^
// ---------------------------------------------------------------------------
PlayScene::~PlayScene()
{
	SAFE_DELETE(csv);
}

// ---------------------------------------------------------------------------
// �e�ϐ��̏���������
// ---------------------------------------------------------------------------
void PlayScene::Init()
{
	//�萔��CSV�ǂݍ���
	CsvLoad();

	//�e�v���C���[�𐶐��A�|�C���^��z��Ɋi�[
	p1 = new Player(0, dc->GetColor(0));
	p2 = new Player(1, dc->GetColor(1));
	p3 = new Player(2, dc->GetColor(2));
	p4 = new Player(3, dc->GetColor(3));
	players.push_back(p1);
	players.push_back(p2);
	players.push_back(p3);
	players.push_back(p4);

	//�e�v���C���[�ʒu�̏������A�^�O�̐ݒ�
	for (int i = 0; i < MAXPLAYER; i++) {
		string str = "Player" + to_string(i+1);
		players[i]->SetPosition(InitPos[i].x, InitPos[i].y, InitPos[i].z);
		players[i]->SetTag(str);
	}

	//�A�C�e�����[�t�̏�������(�X�e�[�W����)
	Leaf* leaf = new Leaf(3, VECTOR3(0, 1, 0));

	timer = 0;
	frm = 0;
	isPlay = false;
	state = sTransition;
	//state = sReady;
}

// ---------------------------------------------------------------------------
// �v���C��ʂ̍X�V����
// 
// �o�ߎ��ԂȂǂɂ��Q�[����Ԃ̑J�ځA�e�v���C���[�̏��ʌv�Z�Ȃǂ��s��
// ---------------------------------------------------------------------------
void PlayScene::Update()
{
	//T�L�[�̓��͂Ń^�C�g����ʂ֑J��
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_T)) {
		SceneManager::ChangeScene("TitleScene");
		GameDevice()->playBGM->Stop();
		GameDevice()->spurtBGM->Stop();
		frm = 0;
		timer = 0;
		isPlay = false;
	}

	//�Q�[����Ԃ��Ƃ̏���
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

	//�|�[�Y���łȂ���Όo�ߎ��Ԃ����Z
	if (state != sPose) {
		frm++;
	}

	//SplitScreenLastDraw�ŕ\�����鎞�Ԃ̃Z�b�g
	ssld->SetState(state);
	ssld->SetGameTime(timer);

	//DataCarrier�փQ�[�����̓o�^
	dc->SetGameState(state);
	dc->SetGameTime(timer);
	dc->SetIsPlay(isPlay);

	SetFinalRank();		//���ʌv�Z
	dc->SetRank(rank);	//���ʃf�[�^���Z�b�g
}

// ---------------------------------------------------------------------------
// �v���C��ʂ̍X�V�����T�u�֐�
// 
// �g�����W�V�������̏������s��
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
// �v���C��ʂ̍X�V�����T�u�֐�
// 
// �J�E���g�_�E�����̏����A���[�t�̏����������s��
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

	// �z��̏�������Leaf�̐���
	if (leafArray.empty())
	{
		//�e�v���C���[�̃X�^�[�g�ʒu�O���ɌŒ�o��
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
// �v���C��ʂ̍X�V�����T�u�֐�
// 
// �|�[�Y���̏���
// ---------------------------------------------------------------------------
void PlayScene::UpdatePose()
{
	if (GameDevice()->m_pDI->CheckJoy(KD_TRG, 9, 0)) {
		state = sGamePlay;
		isPlay = true;
	}
}

// ---------------------------------------------------------------------------
// �v���C��ʂ̍X�V�����T�u�֐�
// 
// �Q�[�����̏����A���[�t�̍Đ������s��
// ---------------------------------------------------------------------------
void PlayScene::UpdateGamePlay()
{
	timer = GameTime - int(frm * (1.0f / 60.0f));
	
	if (timer <= 0){ //�Q�[���I��
		GameDevice()->whistleSE->Play();
		frm = 0;
		state = sFinish;
		isPlay = false;
	}

	// leaf�̃C���X�^���X���Ȃ��Ȃ�����Đ���
	if (leafArray.size() < MaxLeafNum) {
		for (int i = 0; i < (MaxLeafNum - leafArray.size()); i++) {
			int size = GetRandomSize();
			VECTOR3 position = GetRandomPosition();
			Leaf* d = new Leaf(size, position);
			leafArray.push_back(d);
		}
	}
	
	//�|�[�Y
	if (GameDevice()->m_pDI->CheckJoy(KD_TRG, 9)) {
		state = sPose;
		isPlay = false;
	}
}

// ---------------------------------------------------------------------------
// �v���C��ʂ̍X�V�����T�u�֐�
// 
// �Q�[���I�����̏����A���ʌv�Z�ƃ{�[�i�X�p�ϐ��̊i�[���s��
// ---------------------------------------------------------------------------
void PlayScene::UpdateFinish()
{
	timer = frm * (1.0f / 60.0f);
	if (timer >= 4) { //���U���g��
		std::list<Player*> players = ObjectManager::FindGameObjects<Player>();
		int max = 0;
		int winner = -1;
		for (Player* pl : players)
		{
			if (max < pl->GetScore()) { //��ԃX�R�A�������v���C���[�̒T��
				max = pl->GetScore();
				winner = pl->GetPlayerNum();
			}
			dc->SetScore(pl->GetScore(),pl->GetPlayerNum()); //�X�R�A�̕ۑ�
			for (int i = 0; i < 5; i++) {
				dc->SetPowerLevel(i, pl->GetPowerLv(i), pl->GetPlayerNum()); //�e�\�̓��x���̕ۑ�
			}
			//�e�{�[�i�X�p�ϐ��̃Z�b�g
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
// ���[�t�̃����_���Ȕz�u��Ԃ��֐�
// 
// CSV�t�@�C������擾�������W�����ƂɃ��[�t�̃����_���Ȉʒu��Ԃ�
// ---------------------------------------------------------------------------
VECTOR3 PlayScene::GetRandomPosition()
{
	int x = Random(LeafSpawnRangeMin, LeafSpawnRangeMax);
	int z = Random(LeafSpawnRangeMin, LeafSpawnRangeMax);
	int y = Random(LeafSpawnHeightMin, LeafSpawnHeightMax);
	return VECTOR3(x, y, z);
}

// ---------------------------------------------------------------------------
// ���[�t�̃T�C�Y�������_���ɕԂ��֐�
// 
// CSV�t�@�C������擾�����m�������ƂɃ��[�t�̃����_���ȃT�C�Y��Ԃ�
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
// ���[�t�̏������̏���
// 
// CSV�t�@�C������擾�����m�������ƂɃ��[�t�̃����_���ȃT�C�Y��Ԃ�
// ---------------------------------------------------------------------------
void PlayScene::LeafDestroyed(Leaf* leaf)
{
	if (leaf->GetNum() == 3) { //�������A�C�e��
		VECTOR3 pos = VECTOR3(GetRandomPosition());
		new Leaf(3, pos);
		return;
	}
	// �z�������Y����Leaf���폜
	auto it = std::find(leafArray.begin(), leafArray.end(), leaf);
	if (it != leafArray.end())
	{
		leafArray.erase(it); // �z�񂩂�폜
	}
}

// ---------------------------------------------------------------------------
// �v���C���[�̃X�R�A���珇�ʂ̌v�Z���s���֐�
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
			if (arr[j] > arr[i]) { //�������傫���X�R�A������ꍇ�A���ʂ�1������
				rank[i]++; 
			}
		}
	}
}

// ---------------------------------------------------------------------------
// CSV�ǂݍ��ݏ���
// 
// �v���C��ʂł̏����Ɏg�p����萔��CSV�ǂݍ��݂��s��
// ---------------------------------------------------------------------------
void PlayScene::CsvLoad()
{
	CsvPlayDataLoader dataLoader("data/csv/SceneParam.csv");
	dataLoader.Load();
	playParams = dataLoader.GetPlayParams();

	//�ǂݍ��񂾍\���̂���f�[�^�̑��
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