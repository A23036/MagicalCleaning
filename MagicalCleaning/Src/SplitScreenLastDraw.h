#pragma once
#include "Object3D.h"
#include "Direct3D.h"
#include "Player.h"
#include "Stage.h"
#include "EasingCalc.h"

/// ��ʕ����N���X
/// </summary>
class SplitScreenLastDraw : public Object3D
{
public:
	SplitScreenLastDraw();
	~SplitScreenLastDraw();

	void Start() override;
	void Update() override;
	void Draw() override;

	void Transition();
	
	bool GetIsTransFinish() { return isTransFinish; };
	void SetState(int state);
	void SetGameTime(int time);
	void SetIsTransFinish(bool isTrans);

	void SetPlayers(vector<Player*> players);

	void LoadMap();

private:
	D3D11_VIEWPORT vpSingle;

	EasingCalc* ec;
	Stage* st;

	CSprite* sprite;
	CSpriteImage* frameUiImage;	//��ʘgUI
	CSpriteImage* playUiImage;	//�v���C���UI

	vector<CSpriteImage*> mapImages;	//�}�b�v�C���[�W�f�[�^�z��

	vector<Player*> players;

	int gameState;
	int gameTime;

	int fadeFrm;
	int transFrm;

	bool isTransFinish;

	enum GameState {
		sTransition = 0, //��ʃg�����W�V����
		sReady,		//�J�n�O�J�E���g�_�E��
		sPose,		//�|�[�Y��
		sGamePlay,	//�Q�[���v���C��
		sFinish,	//�Q�[���I�����o��
	};
};