#pragma once
#include "SceneBase.h"
#include "Animator.h"
#include "CsvReader.h"

class SelectScene : public SceneBase
{
public:
	SelectScene();
	~SelectScene();
	void Update() override;
	void Draw() override;

	void CsvLoad(); //CSVファイルからデータの設定
private:
	CsvReader* csv;
	CSprite* sprite;
	CSpriteImage* selectBackImage;	//セレクト画面背景イメージ
	float ScrollSpeed;				//背景スクロール速度
	float offX;						//背景画像位置X
};
