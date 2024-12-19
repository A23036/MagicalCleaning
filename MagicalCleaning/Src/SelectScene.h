#pragma once
#include "SceneBase.h"
#include "Animator.h"
#include "CsvReader.h"
#include "Player.h"
#include "SelectDisplay.h"

class SelectScene : public SceneBase
{
public:
	SelectScene();
	~SelectScene();
	void Update() override;
	void Draw() override;

	void CsvLoad(); //CSVファイルからデータの設定
private:
	Player* players[6]; //プレイヤーのリスト
	SelectDisplay* sd;
	CsvReader* csv;
	CSprite* sprite;
	CSpriteImage* selectBackImage;	//セレクト画面背景イメージ

	VECTOR3 initPos[6];		//キャラクター初期位置
	float ScrollSpeed;		//背景スクロール速度
	float scrollX;			//背景画像位置X
};
