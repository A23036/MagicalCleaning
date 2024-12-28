#pragma once
// Player.h
#include "Object3D.h"
#include "CsvReader.h"
#include "Animator.h"
#include "DataCarrier.h"
#include "Stage.h"
#include "Camera.h"
#include "MagicCircleEffect.h"
#include "TeleportCircleEffect.h"

using namespace std;

enum State { //プレイヤー状態
	sStandby = 0,
	sOnGround,
	sJump,
	sAttack1,
	sAttack2,
	sAttack3,
	sCharge,
	sBlow,
	sTeleport,
	sStop,
	sWait,
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
	Player(int num, int color);
	~Player();
	void Start() override;
	void Update() override;
	void Draw() override;

	void CsvLoad(); //CSVファイルからデータの設定

	SphereCollider Collider() override;

	int GetColor() { return color; };
	int GetLeaf() { return leaf; };
	int GetMP() { return mp; };
	int GetWeight() { return weight; };
	int GetCarWeight() { return carWeight; };
	int GetScore() { return score; };
	int GetPlayerNum() { return playerNum; };
	int GetPlayerState() { return state; };
	int GetSelectPower() { return selectPower; };
	int GetPowerCost(int selectPower);
	int GetPowerLv(int selectPower);
	int GetMaxPowerLv(int selectPower);
	bool GetIsSetTeleport() { return setTeleport; };
	VECTOR2 GetTeleportPos() { return VECTOR2(tereportPos.x, tereportPos.z); };
	bool GetIsInvisible() { return isInvisible; };
	bool GetIsDash() { return isDash; };
	bool GetIsFly() { return isFly; };
	float GetMoveDistance() { return moveDistance; };
	int GetJumpCount() { return jumpCount; };
	int GetKnockOutCount() { return knockOutCount; };
	int GetItemCount() { return itemCount; };
	int GetCleanReafCount() { return cleanReafCount; };
	int GetBlowCount() { return blowCount; };
	void SetPlayerState(int state);
	void SetPlayerPrevState(int state);
	void SetBlowVec(VECTOR3 vec);
	void SetSpeedY(float y);
	void SetIsBlow();
	void ResetLeaf();
	void AddLeaf(int n);
	void AddMP(int n);
	void AddWeight(int n);
	void AddScore(int n);
	void AddCleanReaf();

private:
	CsvReader* csv;
	DataCarrier* dc;
	Stage* st;
	Camera* cm;
	list<Player*> otherPlayers;
	MagicCircleEffect* mcEffect;
	TeleportCircleEffect* tpEffect;

	void UpdateOnGround();	//地上処理
	void UpdateJump();		//空中処理
	void UpdateAttack();	//攻撃中処理
	void UpdateCharge();	//MP変換中処理
	void UpdateBlow();		//吹っ飛ばされ中処理
	void UpdateTeleport();	//テレポート処理

	float deltaTime;
	float GRAVITY;		//重力加速度
	float JUMP_POWER;	//ジャンプ力
	float MOVE_SPEED;	//基本移動速度

	int playerNum;		//プレイヤー番号
	int color;			//プレイヤーカラー
	int score;			//スコア
	int leaf;			//葉っぱ所持数
	int mp;				//MP所持数
	int weight;			//運搬重量
	int jumpCount;		//現在ジャンプ回数
	bool isDash;		//ダッシュ中判定
	bool isFly;			//飛行中判定
	bool isBlow;		//吹っ飛び中判定

	//各能力最終進化
	bool canTeleport;	//テレポート可能判定
	bool setTeleport;	//テレポート場所設置済み判定
	VECTOR3 tereportPos;//テレポート場所
	bool canFly;		//飛行可能判定
	bool canSpeedAtk;	//連続攻撃可能判定
	int fastAtkSpeed;	//連続攻撃の攻撃速度(○fに一回)
	bool canRangeAtk;	//遠距離攻撃可能判定
	bool canFastCharge; //高速変換可能判定

	bool atkComboFlg;	//連続攻撃判定
	int	 comboWaitFrm;	//コンボ先行入力フレーム
	bool isMagicReady;	//MP変換予備動作完了フラグ
	float chargeSpeed;	//MP変換スピード
	float chargeTime;	//MP変換経過時間
	VECTOR3 blowVec;	//吹っ飛ばしベクトル
	bool isInvisible;	//透明フラグ
	int InvisibleTime;	//透明化時間
	int invisibleTime;	//透明化経過時間

	bool finishAtkAnim;
	int atkNum;			//現在の攻撃の連続回数
	
	bool isTeleporting;	//テレポート中判定
	float TeleportTime;	//テレポート時間
	int teleportFrm;	//テレポート経過時間

	float	moveSpeed;	//移動速度
	int		jumpNum;	//ジャンプ回数
	float	atkSpeed;	//攻撃速度
	float	atkRange;	//攻撃範囲
	int		carWeight;	//運搬可能重量

	int msNum;	//能力レベル:移動速度
	int jnNum;	//能力レベル:ジャンプ回数
	int asNum;	//能力レベル:攻撃速度
	int arNum;	//能力レベル:攻撃範囲
	int cwNum;	//能力レベル:運搬可能重量

	static const int MsTableNum = 10;	//移動速度テーブル数
	static const int JnTableNum = 10;	//ジャンプ回数テーブル数
	static const int AsTableNum = 10;	//攻撃速度テーブル数
	static const int ArTableNum = 10;	//攻撃範囲テーブル数
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

	float moveDistance;	//移動距離
	int jumpCountAll;	//累計ジャンプ回数
	int knockOutCount;	//吹っ飛ばした敵の数
	int itemCount;		//アイテムを使用した回数
	int cleanReafCount;	//リーフを掃除した数
	int blowCount;		//吹っ飛ばされた回数

	Animator* animator; // 部品のインスタンスをポインター型で準備
	enum AnimID {
		aStandBy = 0,
		aIdle,
		aRun,
		aJump,
		aJump2,
		aFly,
		aFall,
		aAttack1,
		aAttack2,
		aAttack3,
		aComboAttack,
		aChargeReady,
		aCharge,
		aBlow,
	};

	int state;		//プレイヤー状態
	int prevState;	//直前のプレイヤー状態

	VECTOR3 posOld;	//直前のプレイヤー位置

	int selectPower; //選択中能力

	int anmFrame; // アニメーションのフレームを数える

	Broom* child; //自身の持っている箒のポインタ
};


