#pragma once
#include "SceneBase.h"
#include "DataCarrier.h"
#include "ResultDisplay.h"
#include "Player.h"
 
class ResultScene : public SceneBase
{
public:
	ResultScene();
	~ResultScene();
	void Update() override;
	void Draw() override;

	void SetRandomBonus();
	void SetBonusPlayer(int bonus,std::vector<int> &players);

	void SetFinalRank();

private:
	DataCarrier* dc;
	ResultDisplay* rd;
	CSprite* sprite;
	CSpriteImage* resultBackImage;	//リザルト画面背景イメージ

	Player* p1;
	Player* p2;
	Player* p3;
	Player* p4;

	int bonus1;
	int bonus2;

	std::vector<int> bonusPlayers1;
	std::vector<int> bonusPlayers2;

	enum Bonus {
		bMove = 0,
		bJump,
		bKnock,
		bItem,
		bClean,
		bBlow,
	};
};
