#pragma once
// Player.h
#include "Object3D.h"
#include "CsvReader.h"
#include "Animator.h"
#include "DataCarrier.h"
#include "Stage.h"
#include "Camera.h"

enum State { //プレイヤー状態
	sStandby = 0,
	sOnGround,
	sJump,
	sAttack1,
	sAttack2,
	sAttack3,
	sStop,
};

enum SelectPowerID { //選択中強化能力
	pMS = 0,
	pJN,
	pAS,
	pAR,
	pCW,
};

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
	Player(VECTOR3 pos, VECTOR3 rot, int num);
	Player(int num);
	~Player();
	void Start() override;
	void Update() override;
	void Draw() override;

	void CsvLoad(); //CSVファイルからデータの設定

	SphereCollider Collider() override;

	int GetMP() { return mp; };
	int GetWeight() { return weight; };
	int GetScore() { return score; };
	int GetPlayerNum() { return playerNum; };
	int GetPlayerState() { return state; };
	int GetSelectPower() { return selectPower; };
	int GetPowerCost(int selectPower);
	bool GetIsDash() { return isDash; };
	bool GetIsFly() { return isFly; };
	void SetPlayerState(int state);
	void SetPlayerCurState(int state);
	void AddMP(int n);
	void AddWeight(int n);
	void AddScore(int n);

private:
	CsvReader* csv;
	DataCarrier* dc;
	Stage* st;
	Camera* cm;
	list<Player*> otherPlayers;

	void UpdateOnGround();	//地上処理
	void UpdateJump();		//空中処理
	void UpdateAttack1();	//攻撃1中処理
	void UpdateAttack2();	//攻撃2中処理
	void UpdateAttack3();	//攻撃3中処理

	float deltaTime;
	float GRAVITY;		//重力加速度
	float JUMP_POWER;	//ジャンプ力
	float MOVE_SPEED;	//基本移動速度

	int playerNum;		//プレイヤー番号
	int score;
	int mp;				//MP
	int weight;			//運搬重量
	int jumpCount;		//現在ジャンプ回数
	bool isDash;		//ダッシュ中判定
	bool isFly;			//飛行中判定
	bool canFly;		//飛行可能判定
	bool atkComboFlg;	//連続攻撃判定
	int	 comboWaitFrm;	//コンボ先行入力フレームs

	bool finishAtkAnim;

	float	moveSpeed;	//移動速度
	int		jumpNum;	//ジャンプ回数
	float	atkSpeed;	//攻撃速度
	float	atkRange;	//攻撃範囲
	int		carWeight;	//運搬可能重量

	int msNum;
	int jnNum;
	int asNum;
	int arNum;
	int cwNum;

	static const int MsTableNum = 10;	//移動速度テーブル数
	static const int JnTableNum = 5;	//ジャンプ回数テーブル数
	static const int AsTableNum = 10;	//攻撃速度テーブル数
	static const int ArTableNum = 5;	//攻撃範囲テーブル数
	static const int CwTableNum = 10;	//運搬可能重量テーブル数

	float MoveSpeedT	[MsTableNum];	//移動速度テーブル
	int	  JumpNumT		[JnTableNum];	//ジャンプ回数テーブル
	float AtkSpeedT		[AsTableNum];	//攻撃速度テーブル
	float AtkRangeT		[ArTableNum];	//攻撃範囲テーブル
	int   CarWeightT	[CwTableNum];	//運搬可能重量テーブル
	
	int MoveSpeedC[MsTableNum];	//移動速度コストテーブル
	int	JumpNumC[JnTableNum];	//ジャンプ回数コストテーブル
	int AtkSpeedC[AsTableNum];	//攻撃速度コストテーブル
	int AtkRangeC[ArTableNum];	//攻撃範囲コストテーブル
	int CarWeightC[CwTableNum];	//運搬可能重量コストテーブル

	float speedY; // Y方向の速度

	Animator* animator; // 部品のインスタンスをポインター型で準備
	enum AnimID {
		aIdle = 0,
		aRun = 1,
		aJump,
		aJump2,
		aFly,
		aFall,
		aAttack1,
		aAttack2,
		aAttack3,
	};

	int state;		//プレイヤー状態
	int prevState;	//直前のプレイヤー状態

	VECTOR3 posOld;	//直前のプレイヤー位置

	int selectPower; //選択中能力

	int anmFrame; // アニメーションのフレームを数える

	Broom* child; //自身の持っている箒のポインタ
};


