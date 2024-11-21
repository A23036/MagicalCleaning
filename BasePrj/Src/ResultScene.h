#pragma once
#include "SceneBase.h"
#include "DataCarrier.h"

class ResultScene : public SceneBase
{
public:
	ResultScene();
	~ResultScene();
	void Update() override;
	void Draw() override;
private:
	DataCarrier* dc;
};
