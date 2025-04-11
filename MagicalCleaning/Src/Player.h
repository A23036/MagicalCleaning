#pragma once
#include "Object3D.h"
#include "CsvPlayerDataLoader.h"

//プロトタイプ宣言
class CsvReader;
class Broom;
class Animator;
class DataCarrier;
class Stage;
class Camera;
class MagicCircleEffect;
class TeleportCircleEffect;
class PlayDisplay;

using namespace std;

//プレイヤー状態
enum State {
	sStandby = 0,	//待機(セレクト画面、リザルト画面専用状態)
	sOnGround,		//地上
	sJump,			//ジャンプ
	sAttack1,		//攻撃1
	sAttack2,		//攻撃2
	sAttack3,		//攻撃3
	sCharge,		//詠唱
	sBlow,			//吹っ飛ばされ中
	sTeleport,		//テレポート中
	sStop,			//ポーズ中
	sWait,			//開始待機中
};

//選択中強化能力
enum SelectPowerID {
	pMS = 0,	//移動速度
	pJN,		//ジャンプ回数
	pAS,		//攻撃速度
	pAR,		//攻撃範囲
	pCW,		//テモチリーフ
};

// ---------------------------------------------------------------------------
// プレイヤー処理
// 
// セレクト画面、プレイ画面、リザルト画面でのプレイヤー処理を行うクラス
// ---------------------------------------------------------------------------
class Player : public Object3D {
public:
	Player(VECTOR3 pos, VECTOR3 rot, int num);
	Player(int num, int color);
	virtual ~Player();
	void Start() override;
	void Init();

	void Update() override;
	void Draw() override;

	void CsvLoad(); //CSVファイルからデータの設定

	SphereCollider Collider() override;

	//アクセス関数
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
	VECTOR2 GetTeleportPos() { return VECTOR2(teleportPos.x, teleportPos.z); };
	bool GetIsInvisible() { return isInvisible; };
	float GetCurInvisibleTime() { return  invisibleTime; };
	float GetInvisibleTime();
	bool GetIsDamage() { return isDamageCool; };
	bool GetIsDash() { return isDash; };
	bool GetIsFly() { return isFly; };
	float GetMoveDistance() { return moveDistance; };
	int GetJumpCount() { return jumpCountAll; };
	int GetKnockOutCount() { return knockOutCount; };
	int GetItemCount() { return itemCount; };
	int GetCleanReafCount() { return cleanReafCount; };
	int GetBlowCount() { return blowCount; };
	int GetItemNum() { return itemNum; };
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
	void UseItem(int num);

private:
	//各種クラスのポインタ
	Animator* animator;
	CsvReader* csv;
	DataCarrier* dc;
	Stage* st;
	Camera* cm;
	list<Player*> otherPlayers;
	MagicCircleEffect* mcEffect;
	TeleportCircleEffect* tpEffect;
	PlayDisplay* pd;

	//自身の持っている箒
	Broom* child;

	//状態ごとの更新処理
	void UpdateOnGround();	//地上処理
	void UpdateJump();		//空中処理
	void UpdateAttack();	//攻撃中処理
	void UpdateCharge();	//MP変換中処理
	void UpdateBlow();		//吹っ飛ばされ中処理
	void UpdateTeleport();	//テレポート処理

	void CheckAtkCoillision(); //攻撃ヒット判定処理

	float deltaTime;

	//プレイヤー基礎変数
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
	VECTOR3 teleportPos;//テレポート場所
	bool canFly;		//飛行可能判定
	bool canSpeedAtk;	//連続攻撃可能判定
	int  fastAtkSpeed;	//連続攻撃の攻撃速度(○fに一回)
	bool canRangeAtk;	//範囲攻撃可能判定
	bool canFastCharge; //高速変換可能判定

	bool atkComboFlg;	//連続攻撃判定
	int	 comboWaitFrm;	//コンボ先行入力フレーム
	bool isMagicReady;	//MP変換予備動作完了フラグ
	float chargeSpeed;	//MP変換スピード
	float chargeTime;	//MP変換経過時間
	VECTOR3 blowVec;	//吹っ飛ばしベクトル
	int  itemNum;		//アイテム番号(-1:未所持,0:ステルス)
	bool isInvisible;	//透明フラグ

	float invisibleTime;//透明化経過時間

	int  damageTime;	//無敵経過時間
	bool isDamageCool;	//無敵フラグ

	bool finishAtkAnim;
	int  atkNum;		//現在の攻撃の連続回数
	
	bool isTeleporting;	//テレポート中判定
	int  teleportFrm;	//テレポート経過時間

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

	float speedY; // Y方向の速度

	//ボーナス計算用変数
	float moveDistance;	//移動距離
	int jumpCountAll;	//累計ジャンプ回数
	int knockOutCount;	//吹っ飛ばした敵の数
	int itemCount;		//アイテムを使用した回数
	int cleanReafCount;	//リーフを掃除した数
	int blowCount;		//吹っ飛ばされた回数

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

	VECTOR3 prevPos; //直前のプレイヤー位置

	int selectPower; //選択中能力

	int anmFrame; // アニメーションのフレームを数える

	//CSVファイルから読み込んだデータを格納する構造体
	PlayerParams playerParams;

	//CSVファイルから読み込まれるデータ
	float InvisibleTime[MAXPLAYER];	//透明化時間
	float DamageCoolTime;		//無敵時間
	float TeleportTime;			//テレポート時間

	static const int PowerLvNum = 10;	//能力レベル段階数

	float MoveSpeedT[PowerLvNum];	//移動速度テーブル
	int	  JumpNumT[PowerLvNum];		//ジャンプ回数テーブル
	float AtkSpeedT[PowerLvNum];	//攻撃速度テーブル
	float AtkRangeT[PowerLvNum];	//攻撃範囲テーブル
	int   CarWeightT[PowerLvNum];	//運搬可能重量テーブル

	int MoveSpeedC[PowerLvNum];	//移動速度コストテーブル
	int	JumpNumC[PowerLvNum];	//ジャンプ回数コストテーブル
	int AtkSpeedC[PowerLvNum];	//攻撃速度コストテーブル
	int AtkRangeC[PowerLvNum];	//攻撃範囲コストテーブル
	int CarWeightC[PowerLvNum];	//運搬可能重量コストテーブル
};