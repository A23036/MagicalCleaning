#pragma once
#include "SceneBase.h"
#include "PlayDisplay.h"
#include "SplitScreenLastDraw.h"
#include "DataCarrier.h"
#include "CsvReader.h"
#include "Dust.h"

class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;

	void CsvLoad();
	void DustDestroyed(Dust* dust);

private:
	PlayDisplay* pd;
	SplitScreenLastDraw* ssld;
	DataCarrier* dc;
	CsvReader* csv;

	int timer;		//�Q�[���o�ߎ���(�b)
	int frm;		//�Q�[���o�ߎ���(�t���[��)
	int GameTime;	//�Q�[������
	bool isPlay;	//�Q�[�����t���O

	static const int MAX_DUST_NUM = 20; //�ő吶���S�~��
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