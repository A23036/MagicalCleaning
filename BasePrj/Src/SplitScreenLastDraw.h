#pragma once
#include "Object3D.h"
#include "Direct3D.h"

/// 画面分割クラス
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

private:
	D3D11_VIEWPORT vpSingle;

	CSprite* sprite;
	CSpriteImage* frameUiImage;

	int gameState;
	int gameTime;

	enum GameState {
		sReady = 0,	//開始前カウントダウン
		sPose,		//ポーズ中
		sGamePlay,	//ゲームプレイ中
		sFinish,	//ゲーム終了演出中
	};
};