#pragma once
#include "SceneBase.h"
#include "PlayDisplay.h"
#include "SplitScreenLastDraw.h"
#include "DataCarrier.h"

class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;

private:
	PlayDisplay* pd;
	SplitScreenLastDraw* ssld;
	DataCarrier* dc;

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