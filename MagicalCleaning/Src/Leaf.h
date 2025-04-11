#pragma once
#include "Object3D.h"

//プロトタイプ宣言
class Player;
class Stage;
class LeafEffect;

// ---------------------------------------------------------------------------
// リーフ処理
// 
// プレイ画面でのリーフ処理を行うクラス
// ---------------------------------------------------------------------------
class Leaf : public Object3D {
public:
	Leaf(int number,VECTOR3 pos);
	virtual ~Leaf();
	void Start() override;
	void Update();
	void Draw() override;

	SphereCollider Collider(int n);

	//アクセス関数
	int GetNum() { return leafNum; };

	void AddDamage(Player* player,int damage);

private:
	Stage* st;

	VECTOR3 prevPos; //前フレームの位置

	int maxHp;		//最大hp
	int hp;			//現在hp
	int leafNum;	//リーフの番号
	float size;		//リーフのサイズ

	float maxScale; //最大サイズ(生成初期サイズ)
	
	//リーフID
	enum LeafID {
		Small = 0,
		Midium,
		Big,
		Item,
	};
};