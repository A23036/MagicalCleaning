#pragma once
// Player.h
#include "Object3D.h"
#include "CsvReader.h"
#include "Animator.h"

class Broom : public Object3D {
public:
	Broom(Object3D* parentModel, int num);
	~Broom();
	void Update() override;
	void Draw() override;
	void SetPos(MATRIX4X4 m);
private:
	Object3D* parent;
	MATRIX4X4 mat;
};

class Player : public Object3D {
public:
	Player();
	Player(int num);
	~Player();
	void Update() override;
	void Draw() override;

	void CsvLoad(); //CSVファイルからデータの設定

	SphereCollider Collider() override;

	int GetMP() { return mp; };
	int GetWeight() { return weight; };
	int GetPlayerNum() { return playerNum; };
	int GetPlayerState() { return state; };
	void AddMP(int n);
	void AddWeight(int n);

private:
	CsvReader* csv;

	float GRAVITY;		//重力加速度
	float JUMP_POWER;	//ジャンプ力
	float MOVE_SPEED;	//基本移動速度

	int playerNum;		//プレイヤー番号
	int mp;
	int weight;

	bool finishAtkAnim;

	int moveSpeed;	//移動速度
	int	jumpNum;	//ジャンプ回数
	int atkSpeed;	//攻撃速度
	int atkRange;	//攻撃範囲
	int carWeight;	//運搬可能重量

	/*
	int moveSpeedT	[MAXPLAYER];	//移動速度テーブル
	int	jumpNumT	[MAXPLAYER];	//ジャンプ回数
	int atkSpeedT	[MAXPLAYER];	//攻撃速度
	int atkRangeT	[MAXPLAYER];	//攻撃範囲
	int carWeightT	[MAXPLAYER];	//運搬可能重量
	*/

	float speedY; // Y方向の速度

	Animator* animator; // 部品のインスタンスをポインター型で準備
	enum AnimID {
		aIdle = 0,
		aRun = 1,
		aWalk,
		aJump,
		aAttack1,
		aAttack2,
		aAttack3,
	};

	enum State {
		sOnGround = 0,
		sJump,
		sAttack1,
		sAttack2,
		sAttack3,
	};

	State state;

	void UpdateOnGround();	//地上処理
	void UpdateJump();		//空中処理
	void UpdateAttack1();	//攻撃1中処理
	void UpdateAttack2();	//攻撃2中処理
	void UpdateAttack3();	//攻撃3中処理

	int anmFrame; // アニメーションのフレームを数える

	Broom* child; //自身の持っている箒のポインタ
};


