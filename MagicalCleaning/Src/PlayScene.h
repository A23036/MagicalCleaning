#pragma once
#include "SceneBase.h"
#include "CsvPlayDataLoader.h"
#include "Object3D.h"

//�v���g�^�C�v�錾
class PlayDisplay;
class SplitScreenLastDraw;
class DataCarrier;
class CsvReader;
class Player;
class Leaf;
class Stage;
class Sky;
class Camera;
class PlayDisplay;

using namespace std;

// ---------------------------------------------------------------------------
// �v���C��ʏ���
// 
// �Q�[����Ԃ̊Ǘ��Ȃǂ��s��
// ---------------------------------------------------------------------------
class PlayScene : public SceneBase
{
public:
	PlayScene();
	virtual ~PlayScene();
	void Init();
	void Update() override;

	void LeafDestroyed(Leaf* leaf);
	void SetFinalRank();

	void UpdateTransition();
	void UpdateReady();
	void UpdatePose();
	void UpdateGamePlay();
	void UpdateFinish();
	VECTOR3 GetRandomPosition();
	int GetRandomSize();

	void CsvLoad();

private:
	//�e��N���X�̃|�C���^
	PlayDisplay* pd;
	SplitScreenLastDraw* ssld;
	DataCarrier* dc;
	CsvReader* csv;
	Stage* st;
	Sky* sky;
	Camera* cm;

	//�e�v���C���[���i�[�����z��
	vector<Player*> players;
	
	//�X�e�[�W��̃��[�t���i�[�����z��
	vector<Leaf*> leafArray;

	//�e�v���C���[�̃|�C���^
	Player* p1;
	Player* p2;
	Player* p3;
	Player* p4;

	//CSV�t�@�C������ǂݍ��񂾃f�[�^���i�[����\����
	PlayParams playParams;

	//CSV�t�@�C������ǂݍ��܂��f�[�^
	int GameTime;					//�Q�[������
	int MaxLeafNum;					//�ő吶�����[�t��
	int LeafSpawnRangeMin;			//���[�t�o���ŏ��͈�
	int LeafSpawnRangeMax;			//���[�t�o���ő�͈�
	int LeafSpawnHeightMin;			//���[�t�o���ŏ�����
	int LeafSpawnHeightMax;			//���[�t�o���ő卂��
	VECTOR3 InitPos[MAXPLAYER];		//�e�v���C���[�̏����ʒu

	//�Q�[���v���C�Ɏg�p����ϐ�
	int  timer;				//�Q�[���o�ߎ���(�b)
	int  frm;				//�Q�[���o�ߎ���(�t���[��)
	int  rank[MAXPLAYER];	//����
	bool isPlay;			//�Q�[�����t���O

	enum GameState {
		sTransition = 0,	//��ʃg�����W�V����
		sReady,				//�J�n�O�J�E���g�_�E��
		sPose,				//�|�[�Y��
		sGamePlay,			//�Q�[���v���C��
		sFinish,			//�Q�[���I�����o��
	};
	
	GameState state;	//�Q�[����ԕϐ�

};