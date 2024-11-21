#pragma once
#include "SceneBase.h"
#include "PlayDisplay.h"
#include "SplitScreenLastDraw.h"
#include "DataCarrier.h"
#include "CsvReader.h"

class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;

	void CsvLoad();

private:
	PlayDisplay* pd;
	SplitScreenLastDraw* ssld;
	DataCarrier* dc;
	CsvReader* csv;

	int timer;		//�Q�[���o�ߎ���(�b)
	int frm;		//�Q�[���o�ߎ���(�t���[��)
	int GameTime;	//�Q�[������
	bool isPlay;	//�Q�[�����t���O

	void UpdateReady();
	void UpdatePose();
	void UpdateGamePlay();
	void UpdateFinish();

	enum GameState {
		sReady = 0,	//�J�n�O�J�E���g�_�E��
		sPose,		//�|�[�Y��
		sGamePlay,	//�Q�[���v���C��
		sFinish,	//�Q�[���I�����o��
	};
	
	GameState state; //�Q�[����ԕϐ�

};