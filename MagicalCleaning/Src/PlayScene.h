#pragma once
#include "SceneBase.h"
#include "PlayDisplay.h"
#include "SplitScreenLastDraw.h"
#include "DataCarrier.h"
#include "CsvReader.h"
#include "Dust.h"
#include "Stage.h"
#include "Sky.h"
#include "Camera.h"

class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;

	void CsvLoad();
	void DustDestroyed(Dust* dust);
	void CalcRank();

private:
	PlayDisplay* pd;
	SplitScreenLastDraw* ssld;
	DataCarrier* dc;
	CsvReader* csv;
	Stage* st;
	Sky* sky;
	Camera* cm;

	list<Player*> players;

	Player* p1;
	Player* p2;
	Player* p3;
	Player* p4;

	int timer;		//�Q�[���o�ߎ���(�b)
	int frm;		//�Q�[���o�ߎ���(�t���[��)
	int GameTime;	//�Q�[������
	bool isPlay;	//�Q�[�����t���O
	int rank[MAXPLAYER];	//����

	static const int MAX_DUST_NUM = 30; //�ő吶���S�~��
	std::vector<Dust*> dustArray;

	void UpdateReady();
	void UpdatePose();
	void UpdateGamePlay();
	void UpdateFinish();
	VECTOR3 GetRandomPosition();
	int GetRandomSize();

	enum GameState {
		sReady = 0,	//�J�n�O�J�E���g�_�E��
		sPose,		//�|�[�Y��
		sGamePlay,	//�Q�[���v���C��
		sFinish,	//�Q�[���I�����o��
	};
	
	GameState state; //�Q�[����ԕϐ�

};