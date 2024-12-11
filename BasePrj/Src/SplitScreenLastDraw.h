#pragma once
#include "Object3D.h"
#include "Direct3D.h"
#include "Player.h"

/// ��ʕ����N���X
/// </summary>
class SplitScreenLastDraw : public Object3D
{
public:
	SplitScreenLastDraw();
	~SplitScreenLastDraw();

	void Update() override;
	void Draw() override;

	void SetState(int state);
	void SetGameTime(int time);

	void SetPlayers(list<Player*> players);

private:
	D3D11_VIEWPORT vpSingle;

	CSprite* sprite;
	CSpriteImage* frameUiImage;

	CSpriteImage* mapImage1;	//�}�b�v�C���[�W1
	CSpriteImage* mapImage2;	//�}�b�v�C���[�W2

	list<Player*> players;

	int gameState;
	int gameTime;

	enum GameState {
		sReady = 0,	//�J�n�O�J�E���g�_�E��
		sPose,		//�|�[�Y��
		sGamePlay,	//�Q�[���v���C��
		sFinish,	//�Q�[���I�����o��
	};
};