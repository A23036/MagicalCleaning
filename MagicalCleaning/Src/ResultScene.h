#pragma once
#include "SceneBase.h"
#include "DataCarrier.h"
#include "ResultDisplay.h"
 
class ResultScene : public SceneBase
{
public:
	ResultScene();
	~ResultScene();
	void Update() override;
	void Draw() override;
private:
	DataCarrier* dc;
	ResultDisplay* rd;
	CSprite* sprite;
	CSpriteImage* resultBackImage;	//リザルト画面背景イメージ
};
