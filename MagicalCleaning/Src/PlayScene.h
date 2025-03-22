#pragma once
#include "SceneBase.h"
#include "Player.h"

class PlayDisplay;
class SplitScreenLastDraw;
class DataCarrier;
class CsvReader;
class Leaf;
class Stage;
class Sky;
class Camera;
class PlayDisplay;

using namespace std;

class PlayScene : public SceneBase
{
public:
	PlayScene();
	virtual ~PlayScene();
	void Update() override;
	void Draw() override;

	void CsvLoad();
	void LeafDestroyed(Leaf* dust);
	void SetFinalRank();

	void UpdateTransition();
	void UpdateReady();
	void UpdatePose();
	void UpdateGamePlay();
	void UpdateFinish();
	VECTOR3 GetRandomPosition();
	int GetRandomSize();

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

	int timer;		//�Q�[���o�ߎ���(�b)
	int frm;		//�Q�[���o�ߎ���(�t���[��)
	int GameTime;	//�Q�[������
	bool isPlay;	//�Q�[�����t���O
	int rank[MAXPLAYER];	//����

	static const int MAX_LEAF_NUM = 50; //�ő吶�����[�t��

	enum GameState {
		sTransition = 0, //��ʃg�����W�V����
		sReady,		//�J�n�O�J�E���g�_�E��
		sPose,		//�|�[�Y��
		sGamePlay,	//�Q�[���v���C��
		sFinish,	//�Q�[���I�����o��
	};
	
	GameState state; //�Q�[����ԕϐ�

};