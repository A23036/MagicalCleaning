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
	Player* p0;
	Player* p1;
	Player* p2;
	Player* p3;
	Player* p4;
	Player* p5;

	SelectDisplay* sd;
	
	CsvReader* csv;
	
	CSprite* sprite;
	CSpriteImage* selectBackImage;	//セレクト画面背景イメージ
	float ScrollSpeed;				//背景スクロール速度
	float offX;						//背景画像位置X
};
