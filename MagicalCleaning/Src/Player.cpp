#include "Player.h"
#include "../Libs/Imgui/imgui.h"
#include <dinput.h>
#include "CsvReader.h"
#include "Stage.h"
#include "Camera.h"
#include "Broom.h"
#include "Leaf.h"
#include "SlashEffect.h"
#include "PowerUpEffect.h"
#include "LeafEffect.h"
#include "JumpEffect.h"
#include "Animator.h"
#include "DataCarrier.h"
#include "MagicCircleEffect.h"
#include "TeleportCircleEffect.h"
#include "PlayDisplay.h"

//定数定義
namespace {
	static const float GRAVITY = 0.005f;	//重力
	static const float JUMP_POWER = 0.1f;	//ジャンプ力
	static const float MOVE_SPEED = 0.05f;	//基本移動速度
}

// ---------------------------------------------------------------------------
// コンストラクタ1
// 
// セレクト画面/リザルト画面で使用するプレイヤーの処理
// ---------------------------------------------------------------------------
Player::Player(VECTOR3 pos, VECTOR3 rot, int num)
{
	animator = new Animator(); // インスタンスを作成

	mesh = new CFbxMesh();

	std::string f = "Data/Player/Color" + std::to_string(num);
	mesh->Load((f + "/witch.mesh").c_str());

	mesh->LoadAnimation(aStandBy, (f + "/standby.anmx").c_str(), true);
	

	animator->SetModel(mesh);	// このモデルでアニメーションする
	animator->Play(aStandBy);	// 待機アニメーション
	animator->SetPlaySpeed(1.0f);

	transform.position = pos;
	transform.rotation = rot;

	state = sStandby;
	prevState = sStandby;
}

// ---------------------------------------------------------------------------
// コンストラクタ2
// 
// プレイ画面で使用するプレイヤーの処理
// ---------------------------------------------------------------------------
Player::Player(int num,int color) : playerNum(num),color(color)
{
	ObjectManager::SetDrawOrder(this, -100);

	//CSVからデータの設定
	CsvLoad();

	//初期化処理
	Init();

	// プレイヤーの持つ箒の生成
	child = new Broom(this,color);

	animator = new Animator();
	mesh = new CFbxMesh();

	//セレクト画面で設定したプレイヤーカラーのメッシュをロード
	std::string f = "Data/Player/Color" + std::to_string(color);
	mesh->Load((f + "/witch.mesh").c_str());

	//アニメーションデータのロード
	mesh->LoadAnimation(aStandBy, (f + "/standby.anmx").c_str(), true);
	mesh->LoadAnimation(aIdle, _T("Data/Player/idle.anmx"), true);
	mesh->LoadAnimation(aRun, _T("Data/Player/run.anmx"), true);
	mesh->LoadAnimation(aJump, _T("Data/Player/jump.anmx"), false);
	mesh->LoadAnimation(aJump2, _T("Data/Player/jump2.anmx"), false);
	mesh->LoadAnimation(aFly, _T("Data/Player/fly.anmx"), true);
	mesh->LoadAnimation(aFall, _T("Data/Player/fall.anmx"), true);
	mesh->LoadAnimation(aAttack1, _T("Data/Player/attack1.anmx"), false);
	mesh->LoadAnimation(aAttack2, _T("Data/Player/attack2.anmx"), false);
	mesh->LoadAnimation(aAttack3, _T("Data/Player/attack3.anmx"), false);
	mesh->LoadAnimation(aComboAttack, _T("Data/Player/comboAttack.anmx"), true);
	mesh->LoadAnimation(aChargeReady, _T("Data/Player/chargeReady.anmx"), false);
	mesh->LoadAnimation(aCharge, _T("Data/Player/charge.anmx"), true);
	mesh->LoadAnimation(aBlow, _T("Data/Player/blow.anmx"), true);

	animator->SetModel(mesh);	// このモデルでアニメーションする
	animator->Play(aIdle);		// 初期アニメーション
	animator->SetPlaySpeed(1.0f);	// 初期アニメーションスピード

	transform.position = VECTOR3(0, 0, 0);
	
	//プレイヤーに応じた初期回転量の設定
	transform.rotation = VECTOR3(0, playerNum * 90 * DegToRad, 0);

}

// ---------------------------------------------------------------------------
// デストラクタ
// ---------------------------------------------------------------------------
Player::~Player()
{
	SAFE_DELETE(mesh);
	SAFE_DELETE(animator);
	SAFE_DELETE(csv);
}

// ---------------------------------------------------------------------------
// 開始時に呼ばれる関数
// 
// オブジェクトを生成後、最初にUpdate()の前に呼ばれる
// ---------------------------------------------------------------------------
void Player::Start()
{
	//自身以外のプレイヤーをリストに格納
	for (int i = 0; i < MAXPLAYER; i++) {
		if (i == playerNum) {
			continue;
		}
		std::string s = "Player";
		s = s + std::to_string(i + 1);
		Player* pl = ObjectManager::FindGameObjectWithTag<Player>(s);
		otherPlayers.push_back(pl);
	}
	
	dc = ObjectManager::FindGameObject<DataCarrier>();
	st = ObjectManager::FindGameObject<Stage>();
	cm = ObjectManager::FindGameObject<Camera>();
	pd = ObjectManager::FindGameObject<PlayDisplay>();
}

// ---------------------------------------------------------------------------
// 各変数の初期化処理
// ---------------------------------------------------------------------------
void Player::Init()
{
	deltaTime = 0.0f;

	state = sWait;
	prevState = sWait;

	speedY = 0;
	score = 0;
	leaf = 0;
	mp = 0;
	weight = 0;
	jumpCount = 0;
	atkNum = 0;
	chargeSpeed = 1.0f;
	chargeTime = 0.0f;
	invisibleTime = 0.0f;
	teleportPos = VECTOR3(0, 0, 0);
	teleportFrm = 0;
	fastAtkSpeed = 8;
	damageTime = 0;
	itemNum = -1;

	isDash = false;
	isFly = false;
	isBlow = false;
	isTeleporting = false;

	canTeleport = false;
	setTeleport = false;
	canFly		= false;
	canSpeedAtk = false;
	canRangeAtk = false;
	canFastCharge = false;

	finishAtkAnim = false;
	atkComboFlg = false;
	isMagicReady = false;
	isInvisible = false;
	isDamageCool = false;

	blowVec = VECTOR3(0, 0, 0);

	comboWaitFrm = 20;

	msNum = 0;
	jnNum = 0;
	asNum = 0;
	arNum = 0;
	cwNum = 0;

	moveSpeed = MoveSpeedT[msNum];
	jumpNum = JumpNumT[jnNum];
	atkSpeed = AtkSpeedT[asNum];
	atkRange = AtkRangeT[arNum];
	carWeight = CarWeightT[cwNum];

	moveDistance = 0.0f;
	jumpCountAll = 0;
	knockOutCount = 0;
	itemCount = 0;
	cleanReafCount = 0;
	blowCount = 0;

	selectPower = 0;
	anmFrame = 0;
}

// ---------------------------------------------------------------------------
// プレイヤーの更新処理
// 
// 各プレイヤーの状態に応じた処理を行う
// ---------------------------------------------------------------------------
void Player::Update()
{
	deltaTime = 60 * SceneManager::DeltaTime();
	prevPos = transform.position;    // -- 2024.12.2

	// 箒の位置情報更新
	if (state != sStandby) { //キャラセレクト画面では持たない

		MATRIX4X4 bone;
		if (state == sAttack2) {
			bone = mesh->GetFrameMatrices(18);// プレイヤーの原点からの左手の位置(18は左手)
		}
		else {
			bone = mesh->GetFrameMatrices(0);// プレイヤーの原点からの右手の位置(0は右手)
		}
		if (isDash || isFly) {
			bone = mesh->GetFrameMatrices(2);//プレイヤーのルート位置
		}
		child->SetPos(bone);
	}
	
	if (dc->GetIsPlay()) {
		animator->Update();
		if (state == sWait) {
			state = sOnGround;
			prevState = sOnGround;
		}
		state = prevState;
	}
	else if(state != sStandby && state != sWait)
	{
		state = sStop;
	}
	else {
		animator->Update();
	}

	switch (state) {
	case sStandby:
	case sStop:
		return;
	case sOnGround:
		UpdateOnGround();
		break;
	case sJump:
		UpdateJump();
		break;
	case sAttack1:
	case sAttack2:
	case sAttack3:
		UpdateAttack();
		break;
	case sCharge:
		UpdateCharge();
		break;
	case sBlow:
		UpdateBlow();
		break;
	case sTeleport:
		UpdateTeleport();
		return;
	default:
		break;
	}
	
	auto di = GameDevice()->m_pDI;
	if ((di->CheckKey(KD_TRG, DIK_UP) && playerNum == 0) || di->CheckJoy(KD_TRG, 2, playerNum)) { //能力強化
		switch (selectPower) {
		case pMS:
			if (mp >= MoveSpeedC[msNum] && msNum < PowerLvNum - 1)
			{
				mp -= MoveSpeedC[msNum];
				msNum++;
				new PowerUpEffect(this,transform.position,selectPower);
				GameDevice()->m_pDI->PlayJoyEffect(2, 1, playerNum); // 振動の再生
				GameDevice()->powerUpSE->Play();
				if (msNum == PowerLvNum - 1) {
					canTeleport = true;
				}
				else {
					moveSpeed = MoveSpeedT[msNum];
				}
			}
			break;
		case pJN:
			if (mp >= JumpNumC[jnNum] && jnNum < PowerLvNum - 1)
			{
				mp -= JumpNumC[jnNum];
				jnNum++;
				new PowerUpEffect(this, transform.position, selectPower);
				GameDevice()->m_pDI->PlayJoyEffect(2, 1, playerNum); // 振動の再生
				GameDevice()->powerUpSE->Play();
				if (jnNum == PowerLvNum - 1) {
					canFly = true;
				}
				else {
					jumpNum = JumpNumT[jnNum];
				}
			}
			break;
		case pAS:
			if (mp >= AtkSpeedC[asNum] && asNum < PowerLvNum - 1)
			{
				mp -= AtkSpeedC[asNum];
				asNum++;
				new PowerUpEffect(this, transform.position, selectPower);
				GameDevice()->m_pDI->PlayJoyEffect(2, 1, playerNum); // 振動の再生
				GameDevice()->powerUpSE->Play();
				if (asNum == PowerLvNum - 1) {
					canSpeedAtk = true;
				}
				else {
					atkSpeed = AtkSpeedT[asNum];
				}
			}
			break;
		case pAR:
			if (mp >= AtkRangeC[arNum] && arNum < PowerLvNum - 1)
			{
				mp -= AtkRangeC[arNum];
				arNum++;
				new PowerUpEffect(this, transform.position, selectPower);
				GameDevice()->m_pDI->PlayJoyEffect(2, 1, playerNum); // 振動の再生
				GameDevice()->powerUpSE->Play();
				if (arNum == PowerLvNum - 1) {
					canRangeAtk = true;
				}
				else {
					atkRange = AtkRangeT[arNum];
				}
			}
			break;
		case pCW:
			if (mp >= CarWeightC[cwNum] && cwNum < PowerLvNum - 1)
			{
				mp -= CarWeightC[cwNum];
				cwNum++;
				new PowerUpEffect(this, transform.position, selectPower);
				GameDevice()->m_pDI->PlayJoyEffect(2, 1, playerNum); // 振動の再生
				GameDevice()->powerUpSE->Play();
				if (cwNum == PowerLvNum - 1) {
					canFastCharge = true;
				}
				else {
					carWeight = CarWeightT[cwNum];
				}
			}
			break;
		}
	}
	
	
	if ((di->CheckKey(KD_TRG, DIK_LEFT) && playerNum == 0) || di->CheckJoy(KD_TRG, 4, playerNum)) { //強化能力変更
		selectPower--;
		if (selectPower < 0) {
			selectPower = 4;
		}
		GameDevice()->powerSelectSE->Play();
	}
	if ((di->CheckKey(KD_TRG, DIK_RIGHT) && playerNum == 0) || di->CheckJoy(KD_TRG, 5, playerNum)) { //強化能力変更
		selectPower++;
		if (selectPower > 4) {
			selectPower = 0;
		}
		GameDevice()->powerSelectSE->Play();
	}

	auto* pdi = GameDevice()->m_pDI;
	DIJOYSTATE2 joyState = pdi->GetJoyState(playerNum);
	
	//アイテム使用処理
	if (joyState.rgbButtons[11] & 0x80) {
		if (itemNum != -1) { //アイテム取得状態
			UseItem(itemNum);
			itemNum = -1;
		}
	}
	
	//テレポート処理
	// テレポート先が設置されていて吹っ飛ばされていないとき
	if (((di->CheckKey(KD_TRG, DIK_F) && playerNum == 0) 
		|| joyState.rgbButtons[10] & 0x80) && setTeleport && state != sBlow) {
		state = sTeleport;
		finishAtkAnim = false;
		isTeleporting = true;
		if (mcEffect != nullptr) {
			mcEffect->SetIsFinish();
		}
	}

	//無敵時間処理
	if (isDamageCool) {
		damageTime++;
		if (damageTime * (1.0f / 60.0f) > DamageCoolTime) {
			damageTime = 0;
			isDamageCool = false;
			mesh->m_vDiffuse = VECTOR4(1, 1, 1, 1.0f);
		}
		else { //透明度を変えて点滅する
			if (damageTime % 12 > 6) {
				mesh->m_vDiffuse = VECTOR4(1, 1, 1, 0.3f);
			}
			else {
				mesh->m_vDiffuse = VECTOR4(1, 1, 1, 0.7f);
			}
		}
	}

	//場外処理
	if (transform.position.y < -30.0f || fabs(transform.position.x) > 60.0f || fabs(transform.position.z) > 60.0f){
		transform.position = VECTOR3(0, 20, 0);
		SetIsBlow();
		SetPlayerState(sBlow);
		animator->MergePlay(aBlow, 0);
		animator->SetPlaySpeed(2.0f);
		ResetLeaf();
	}

	//当たり判定処理 // -- 2024.12.2
	//ステージ
	st->MapCol()->IsCollisionMoveGravity(prevPos, transform.position);

	//吹っ飛び
	transform.position += blowVec;

	//透明化
	if (isInvisible) {
		mesh->m_vDiffuse = VECTOR4(1, 1, 1, 0.2f);
		invisibleTime++;
		if (invisibleTime * (1.0f / 60.0f) > InvisibleTime[dc->GetRank(playerNum) - 1]) {
			invisibleTime = 0;
			isInvisible = false;
			mesh->m_vDiffuse = VECTOR4(1, 1, 1, 1);
		}
	}

	// Leafにめり込まないようにする
	std::list<Leaf*> leaves = ObjectManager::FindGameObjects<Leaf>();
	
	for (Leaf* leaf : leaves) {
		SphereCollider tCol = leaf->Collider(leaf->GetNum());
		SphereCollider pCol = Collider();
		VECTOR3 pushVec = pCol.center - tCol.center;
		float rSum = pCol.radius + tCol.radius;
		if (pushVec.LengthSquare() < rSum * rSum) { // 球の当たり判定
			// 当たってる
			// 押し出す方向はpushVec
			// 押し出す長さを求める
			float pushLen = rSum - pushVec.Length();
			pushVec.y = 0.0f; // y軸方向の押し返しを無効にする(地面に埋まったり浮いたりするため)
			pushVec = XMVector3Normalize(pushVec); // pushVecの長さを１にする
			transform.position += pushVec * pushLen;
		}
	}
	
	// playerにめり込まないようにする
	for (Player* player : otherPlayers) {
		SphereCollider tCol = player->Collider();
		SphereCollider pCol = Collider();
		VECTOR3 pushVec = pCol.center - tCol.center;
		float rSum = pCol.radius + tCol.radius;
		if (pushVec.LengthSquare() < rSum * rSum) { // 球の当たり判定
			// 当たってる
			// 押し出す方向はpushVec
			// 押し出す長さを求める
			float pushLen = rSum - pushVec.Length();
			pushVec.y = 0.0f; // y軸方向の押し返しを無効にする(地面に埋まったり浮いたりするため)
			pushVec = XMVector3Normalize(pushVec); // pushVecの長さを１にする
			transform.position += pushVec * pushLen;
		}
	}
	
	prevState = state;
}
	
// ---------------------------------------------------------------------------
// プレイヤー描画処理
// 
// 画面ごとのプレイヤーの表示非表示を状態によって切り替える
// ---------------------------------------------------------------------------
void Player::Draw()
{
	if (ObjectManager::DrawCounter() != playerNum) {
		if (isInvisible) { //透明のプレイヤーを描画しない
			return;
		}
	}
	Object3D::Draw(); // 継承元の関数を呼ぶ
}

// ---------------------------------------------------------------------------
// CSV読み込み処理
// 
// プレイヤー処理に使用する定数のCSV読み込みを行う
// ---------------------------------------------------------------------------
void Player::CsvLoad()
{
	// csvからデータ読み込み
	CsvPlayerDataLoader dataLoader("data/csv/Paramater.csv");
	dataLoader.Load();
	playerParams = dataLoader.GetPlayerParams();

	//読み込んだ構造体からデータの代入
	DamageCoolTime = playerParams.DamageCoolTime;
	TeleportTime = playerParams.TeleportTime;

	for (int i = 0; i < MAXPLAYER; i++) {
		InvisibleTime[i] = playerParams.InvisibleTime[i];
	}

	for (int i = 0; i < PowerLvNum; i++) {
		MoveSpeedT[i] = playerParams.MoveSpeedT[i];
		JumpNumT[i]   = playerParams.JumpNumT[i];
		AtkSpeedT[i]  = playerParams.AtkSpeedT[i];
		AtkRangeT[i]  = playerParams.AtkRangeT[i];
		CarWeightT[i] = playerParams.CarWeightT[i];

		MoveSpeedC[i] = playerParams.MoveSpeedC[i];
		JumpNumC[i]   = playerParams.JumpNumC[i];
		AtkSpeedC[i]  = playerParams.AtkSpeedC[i];
		AtkRangeC[i]  = playerParams.AtkRangeC[i];
		CarWeightC[i] = playerParams.CarWeightC[i];
	}
}

SphereCollider Player::Collider()
{
	SphereCollider col;
	col.center = transform.position + VECTOR3(0, 0.58f, 0);
	col.radius = 0.58f;
	return col;
}

int Player::GetPowerCost(int selectPower)
{ 
	switch (selectPower) {
	case pMS:
		return MoveSpeedC[msNum];
	case pJN:
		return JumpNumC[jnNum];
	case pAS:
		return AtkSpeedC[asNum];
	case pAR:
		return AtkRangeC[arNum];
	case pCW:
		return CarWeightC[cwNum];
	default:
		return 0;
	}
}

int Player::GetPowerLv(int selectPower)
{
	switch (selectPower) {
	case pMS:
		return msNum;
	case pJN:
		return jnNum;
	case pAS:
		return asNum;
	case pAR:
		return arNum;
	case pCW:
		return cwNum;
	default:
		return 0;
	}
}
int Player::GetMaxPowerLv(int selectPower)
{
	switch (selectPower) {
	case pMS:
		return PowerLvNum;
	case pJN:
		return PowerLvNum;
	case pAS:
		return PowerLvNum;
	case pAR:
		return PowerLvNum;
	case pCW:
		return PowerLvNum;
	default:
		return 0;
	}
}

float Player::GetInvisibleTime()
{
	return  InvisibleTime[dc->GetRank(playerNum) - 1];
}

void Player::SetPlayerState(int state)
{
	this->state = state;
}

void Player::SetPlayerPrevState(int state)
{
	prevState = state;
}

void Player::SetBlowVec(VECTOR3 vec)
{
	blowVec = vec;
}

void Player::SetSpeedY(float y)
{
	speedY = y;
}

void Player::SetIsBlow()
{
	blowCount++;
	isBlow = true;
	isDash = false; 
	isFly = false;
	finishAtkAnim = false;
	GameDevice()->m_pDI->PlayJoyEffect(1, 1, playerNum); // 振動の再生

	if (mcEffect != nullptr) {
		mcEffect->SetIsFinish();
	}
	if (isTeleporting) {
		teleportFrm = 0;
		mesh->m_vDiffuse = VECTOR4(1, 1, 1, 1);
	}
}

void Player::ResetLeaf()
{
	leaf = 0;
}

void Player::AddLeaf(int n)
{
	if (leaf + n > carWeight) { //所持上限までしか加算しない
		leaf = carWeight;
	}
	else {
		leaf += n;
	}
}

void Player::AddMP(int n)
{
	mp += n;
}

void Player::AddWeight(int n)
{
	weight += n;

}

void Player::AddScore(int n)
{
	score += n;
}

void Player::AddCleanReaf()
{
	cleanReafCount++;
}

void Player::UseItem(int num)
{
	switch (num) {
	case 0:
		GameDevice()->itemSE->Play();
		invisibleTime = 0;
		isInvisible = true;
		itemCount++;
		break;
	default:
		break;
	}
}

void Player::UpdateOnGround()
{
	auto di = GameDevice()->m_pDI;

	int x = di->GetJoyState(playerNum).lX;	// 右:1000 / 左:-1000
	int y = di->GetJoyState(playerNum).lY;	// 下:1000 / 上:-1000
	
	if (st->MapCol()->IsCollisionMoveGravity(prevPos, transform.position) == clFall) {
		state = sJump;
		return;
	}

	// 2024.10.26 プレイヤー操作をコントローラーに対応↓
	
	// コントローラースティックの入力を取得
	// 正規化して -1.0〜1.0 の範囲にする
	float ix = di->GetJoyState(playerNum).lX / 1000.0f;
	float iy = di->GetJoyState(playerNum).lY / 1000.0f;
	
	// キーボード操作を取得(1Pのみ動かせる)
	if (di->CheckKey(KD_DAT, DIK_W) && playerNum == 0) {
		iy = -1.0f; // 前進
	}
	if (di->CheckKey(KD_DAT, DIK_S) && playerNum == 0) {
		iy = 1.0f;  // 後退
	}
	if (di->CheckKey(KD_DAT, DIK_A) && playerNum == 0) {
		ix = -1.0f; // 左移動
	}
	if (di->CheckKey(KD_DAT, DIK_D) && playerNum == 0) {
		ix = 1.0f;  // 右移動
	}

	// スティックが入力されているか確認
	if (fabs(ix) > 0.1f || fabs(iy) > 0.1f) {
		// 入力に基づいて移動方向を設定
		VECTOR3 inputDirection = VECTOR3(ix, 0, -iy); // Z軸方向は反転

		// カメラのY軸回転を取得
		float cameraYRotation = cm->GetRotY(playerNum);

		// カメラの回転に基づく移動ベクトルの計算
		MATRIX4X4 cameraRotY = XMMatrixRotationY(cameraYRotation);
		VECTOR3 moveDirection = XMVector3TransformNormal(inputDirection, cameraRotY);
		if (sqrt(ix * ix + iy * iy) > 1.0f) {
			moveDirection = XMVector3Normalize(moveDirection) * MOVE_SPEED; // 斜め移動が直進より早くならないよう補正
		}
		else {
			moveDirection = XMVector3Normalize(moveDirection) * MOVE_SPEED * sqrt(ix * ix + iy * iy); // 傾き具合に応じた速度
		}
		moveDistance += moveDirection.Length(); //移動距離の加算
		
		// 移動の適用
		transform.position += moveDirection * moveSpeed * deltaTime;

		// プレイヤーの回転をカメラ基準で方向を向かせる
		transform.rotation.y = cameraYRotation + atan2(ix, -iy); // カメラの回転に対してスティックの方向に合わせる

		if (sqrt(ix * ix + iy * iy) * moveSpeed > 2.0) {
			// 飛行アニメーションを再生
			animator->MergePlay(aFly, 0);
			animator->SetPlaySpeed(1.0f);
			isDash = true;
		}
		else {
			//走行速度に応じたアニメーションスピードを設定
			animator->SetPlaySpeed(sqrt(ix * ix + iy * iy) * moveSpeed);

			// 走行アニメーションを再生
			animator->MergePlay(aRun, 0);
			isDash = false;
		}
	}
	else {
		// スティックが傾いていない場合は待機アニメーションを再生
		animator->SetPlaySpeed(1.0f);
		animator->MergePlay(aIdle,0);
		isDash = false;
	}
	
	// 2024.10.26 プレイヤー操作をコントローラーに対応↑
	if ((di->CheckKey(KD_TRG, DIK_SPACE) && playerNum == 0) || di->CheckJoy(KD_TRG, 1, playerNum) ) { //ジャンプ
		speedY = JUMP_POWER;
		if (!isDash) {
			animator->MergePlay(aJump, 0);
			animator->SetPlaySpeed(1.0f);
		}
		jumpCount++;
		jumpCountAll++;
		state = sJump;
		GameDevice()->jumpSE->Play();
		GameDevice()->m_pDI->PlayJoyEffect(2, 1, playerNum); // 振動の再生
	}
	if ((di->CheckKey(KD_TRG, DIK_N) && playerNum == 0) || di->CheckJoy(KD_TRG, 0, playerNum)) { //攻撃
		if (canSpeedAtk) {
			animator->MergePlay(aComboAttack, 0);
			animator->SetPlaySpeed(8.0f);
		}
		else {
			animator->MergePlay(aAttack1, 0);
			animator->SetPlaySpeed(1.0f * atkSpeed);
		}
		state = sAttack1;
		isDash = false;
		anmFrame = 0;
		atkNum = 1;
	}
	if ((di->CheckKey(KD_TRG, DIK_M) && playerNum == 0) || di->CheckJoy(KD_TRG, 3, playerNum)) { //MP変換
		animator->MergePlay(aChargeReady);
		animator->SetPlaySpeed(1.0f);
		state = sCharge;
		isDash = false;
		chargeSpeed = 1.0f;
		mcEffect = new MagicCircleEffect(transform.position, color);
		GameDevice()->magicCircleSE->Play();
	}
	if (((di->CheckKey(KD_TRG, DIK_R) && playerNum == 0) || di->CheckJoy(KD_TRG, 7, playerNum)) && canTeleport) { //テレポート魔法陣設置
		setTeleport = true;
		teleportPos = transform.position;
		if (tpEffect != nullptr) {
			tpEffect->SetIsFinish();
		}
		tpEffect = new TeleportCircleEffect(transform.position, color);
	}
}


void Player::UpdateJump()
{
	transform.position.y += speedY * deltaTime;

	//飛行可能かつジャンプキーを入力しっぱなしで降下しているとき
	if (canFly && GameDevice()->m_pDI->CheckJoy(KD_DAT, 1, playerNum) && speedY < 0) {
		speedY = -0.01;	// 重力
		isFly = true;
		if (animator->PlayingID() == aRun || animator->PlayingID() == aFall) {
			animator->MergePlay(aFly);
		}
	}
	else {
		if (animator->PlayingID() == aFly && !isDash) {
			animator->MergePlay(aFall);
		}
		speedY -= GRAVITY * deltaTime;	// 重力
		isFly = false;
	}

	auto di = GameDevice()->m_pDI;
	// コントローラースティックの入力を取得
	// 正規化して -1.0〜1.0 の範囲にする
	float ix = di->GetJoyState(playerNum).lX / 1000.0f;
	float iy = di->GetJoyState(playerNum).lY / 1000.0f;

	// キーボード操作を取得(1Pのみ動かせる)
	if (di->CheckKey(KD_DAT, DIK_W) && playerNum == 0) {
		iy = -1.0f; // 前進
	}
	if (di->CheckKey(KD_DAT, DIK_S) && playerNum == 0) {
		iy = 1.0f;  // 後退
	}
	if (di->CheckKey(KD_DAT, DIK_A) && playerNum == 0) {
		ix = -1.0f; // 左移動
	}
	if (di->CheckKey(KD_DAT, DIK_D) && playerNum == 0) {
		ix = 1.0f;  // 右移動
	}

	// スティックが入力されているか確認
	if (fabs(ix) > 0.1f || fabs(iy) > 0.1f) {
		// 入力に基づいて移動方向を設定
		VECTOR3 inputDirection = VECTOR3(ix, 0, -iy); // Z軸方向は反転

		// カメラのY軸回転を取得
		float cameraYRotation = cm->GetRotY(playerNum);

		// カメラの回転に基づく移動ベクトルの計算
		MATRIX4X4 cameraRotY = XMMatrixRotationY(cameraYRotation);
		VECTOR3 moveDirection = XMVector3TransformNormal(inputDirection, cameraRotY);
		if (sqrt(ix * ix + iy * iy) > 1.0f) {
			moveDirection = XMVector3Normalize(moveDirection) * MOVE_SPEED; // 斜め移動が直進より早くならないよう補正
		}
		else {
			moveDirection = XMVector3Normalize(moveDirection) * MOVE_SPEED * sqrt(ix * ix + iy * iy); // 傾き具合に応じた速度
		}
		moveDistance += moveDirection.Length(); //移動距離の加算

		// 移動の適用
		transform.position += moveDirection * moveSpeed * deltaTime;

		// プレイヤーの回転をカメラ基準で方向を向かせる
		transform.rotation.y = cameraYRotation + atan2(ix, -iy); // カメラの回転に対してスティックの方向に合わせる
	}

	if (di->CheckJoy(KD_TRG, 1, playerNum) && jumpCount <= jumpNum) { //ジャンプ可能回数が残っているとき
		speedY = JUMP_POWER;
		isDash = false;
		if (jumpCount % 2 == 0) {
			animator->MergePlay(aJump); //ジャンプアニメーション1
		}
		else {
			animator->MergePlay(aJump2);//ジャンプアニメーション2
		}
		animator->SetPlaySpeed(1.0f);
		new JumpEffect(transform.position);
		GameDevice()->jumpSE->Play();
		jumpCount++;
		jumpCountAll++;
	}


	if (animator->Finished()) {
		if (isFly) {
			animator->MergePlay(aFly);
		}
		else{
			animator->MergePlay(aFall);
		}
		
	}

	if (st->MapCol()->IsCollisionMoveGravity(prevPos, transform.position) != clFall) {
		// ジャンプ終了
		isFly = false;
		state = sOnGround;
		jumpCount = 0;
		speedY = 0;
		blowVec = VECTOR3(0, 0, 0);
	}
}

void Player::UpdateAttack()
{	
	anmFrame++;

	if ((!finishAtkAnim && animator->CurrentFrame() >= 20.0f / atkSpeed) //攻撃のヒットしたタイミング
		|| (canSpeedAtk && anmFrame % fastAtkSpeed == 0)) { //連続攻撃の場合
		
		if (asNum > 6) {
			GameDevice()->attack3SE->Play();
		}
		else if (asNum > 3) {
			GameDevice()->attack2SE->Play();
		}
		else {
			GameDevice()->attack1SE->Play();
		}
		//攻撃判定処理
		CheckAtkCoillision();
	}
	
	auto di = GameDevice()->m_pDI;
	if (canSpeedAtk) {
		// 攻撃中の方向転換処理
		auto di = GameDevice()->m_pDI;
		float ix = di->GetJoyState(playerNum).lX / 1000.0f;
		float iy = di->GetJoyState(playerNum).lY / 1000.0f;

		// スティックが入力されているか確認
		if (fabs(ix) > 0.1f || fabs(iy) > 0.1f) {
			// カメラのY軸回転を取得
			float cameraYRotation = cm->GetRotY(playerNum);
			// プレイヤーの回転をカメラ基準で決める
			transform.rotation.y = cameraYRotation + atan2(ix, -iy); // カメラの回転に対してスティックの方向に合わせる
		}
	}
	else {
		if (di->CheckKey(KD_TRG, DIK_N) || di->CheckJoy(KD_TRG, 0, playerNum)) { //攻撃中の先行入力受付
			atkComboFlg = true;
			anmFrame = 0;
		}

		if (anmFrame >= comboWaitFrm) { //連続入力後一定時間で連続攻撃フラグ解除
			atkComboFlg = false;
		}
	}

	if (animator->Finished() || (canSpeedAtk && di->CheckJoy(KD_UTRG, 0, playerNum)))
	{
		if (canSpeedAtk) {
			animator->SetPlaySpeed(1.0f);
			state = sOnGround;
			anmFrame = 0;
		}
		//攻撃アニメーションの終了
		finishAtkAnim = false;
		animator->SetPlaySpeed(1.0f);
		if (atkComboFlg && atkNum < 3) { //連続攻撃判定/連続攻撃が4回目でない
			if (atkNum == 1) {
				animator->SetPlaySpeed(1.0f * atkSpeed);
				animator->MergePlay(aAttack2, 0);
				state = sAttack2;
			}
			else if (atkNum == 2) {
				animator->SetPlaySpeed(1.2f * atkSpeed);
				animator->MergePlay(aAttack3, 0);
				state = sAttack3;
			}
			anmFrame = 0;
			atkNum++;

			// 攻撃中の方向転換処理
			auto di = GameDevice()->m_pDI;
			float ix = di->GetJoyState(playerNum).lX / 1000.0f;
			float iy = di->GetJoyState(playerNum).lY / 1000.0f;

			// スティックが入力されているか確認
			if (fabs(ix) > 0.1f || fabs(iy) > 0.1f) {
				// カメラのY軸回転を取得
				float cameraYRotation = cm->GetRotY(playerNum);
				// プレイヤーの回転をカメラ基準で決める
				transform.rotation.y = cameraYRotation + atan2(ix, -iy); // カメラの回転に対してスティックの方向に合わせる
			}
		}
		else {
			//地上状態へ
			state = sOnGround;
		}
		atkComboFlg = false;
	}
	
}


void Player::UpdateCharge()
{
	auto di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_UTRG, DIK_M) || di->CheckJoy(KD_UTRG, 3, playerNum)) {
		animator->SetPlaySpeed(1.0f);
		state = sOnGround;
		isMagicReady = false;
		chargeTime = 0.0f;
		mcEffect->SetIsFinish();
		return;
	}
	
	if (animator->PlayingID() == aChargeReady && animator->Finished()) {
		animator->MergePlay(aCharge);
		isMagicReady = true;
	}

	if (isMagicReady) {
		if (chargeSpeed <= (chargeTime * (1.0f / 60.0f)) && leaf > 0) {
			new LeafEffect(transform.position,VECTOR3(0.5f, 0.5f, 0.5f),1);
			GameDevice()->chargeSE->Play();
			mp++;		//MP加算
			score++;	//スコア加算
			leaf--; //葉っぱを減らす
			GameDevice()->m_pDI->PlayJoyEffect(2, 1, playerNum); // 振動の再生
			if (chargeSpeed > 0.1f) {
				chargeSpeed -= 0.1f;
			}
			chargeTime = 0;
		}
		if (canFastCharge) {
			animator->SetPlaySpeed(2.0f);
			chargeTime += 2.0f;
		}
		else {
			animator->SetPlaySpeed(1.0f);
			chargeTime++;
		}
	}
}

void Player::UpdateBlow()
{
	transform.position.y += speedY * deltaTime;
	speedY -= GRAVITY * deltaTime;	// 重力

	if (isBlow && speedY <= -0.2f) { //吹っ飛びアニメーションの解除(落下し始めてから少したってから)
		animator->MergePlay(aFall, 0); //落下アニメーション
		animator->SetPlaySpeed(1.0f);
		isBlow = false;
	}

	auto di = GameDevice()->m_pDI;
	// コントローラースティックの入力を取得
	// 正規化して -1.0〜1.0 の範囲にする
	float ix = di->GetJoyState(playerNum).lX / 1000.0f;
	float iy = di->GetJoyState(playerNum).lY / 1000.0f;

	// キーボード操作を取得(1Pのみ動かせる)
	if (di->CheckKey(KD_DAT, DIK_W) && playerNum == 0) {
		iy = -1.0f; // 前進
	}
	if (di->CheckKey(KD_DAT, DIK_S) && playerNum == 0) {
		iy = 1.0f;  // 後退
	}
	if (di->CheckKey(KD_DAT, DIK_A) && playerNum == 0) {
		ix = -1.0f; // 左移動
	}
	if (di->CheckKey(KD_DAT, DIK_D) && playerNum == 0) {
		ix = 1.0f;  // 右移動
	}

	// スティックが入力されているか確認
	if (fabs(ix) > 0.1f || fabs(iy) > 0.1f) {
		// 入力に基づいて移動方向を設定
		VECTOR3 inputDirection = VECTOR3(ix, 0, -iy); // Z軸方向は反転

		// カメラのY軸回転を取得
		float cameraYRotation = cm->GetRotY(playerNum);

		// カメラの回転に基づく移動ベクトルの計算
		MATRIX4X4 cameraRotY = XMMatrixRotationY(cameraYRotation);
		VECTOR3 moveDirection = XMVector3TransformNormal(inputDirection, cameraRotY);
		if (sqrt(ix * ix + iy * iy) > 1.0f) {
			moveDirection = XMVector3Normalize(moveDirection) * MOVE_SPEED; // 斜め移動が直進より早くならないよう補正
		}
		else {
			moveDirection = XMVector3Normalize(moveDirection) * MOVE_SPEED * sqrt(ix * ix + iy * iy); // 傾き具合に応じた速度
		}

		// 移動の適用
		transform.position += moveDirection * moveSpeed * deltaTime;

		// プレイヤーの回転をカメラ基準で方向を向かせる
		transform.rotation.y = cameraYRotation + atan2(ix, -iy); // カメラの回転に対してスティックの方向に合わせる

	}

	if (st->MapCol()->IsCollisionMoveGravity(prevPos, transform.position) != clFall) {
		// 吹っ飛び終了
		isFly = false;
		state = sOnGround;
		jumpCount = 0;
		speedY = 0;
		blowVec = VECTOR3(0, 0, 0);
		isDamageCool = true; //無敵時間の開始
	}
}

void Player::UpdateTeleport()
{
	if (teleportFrm * (1.0f / 60.0f) > TeleportTime) {
		mesh->m_vDiffuse = VECTOR4(1, 1, 1, 1);
		transform.position = teleportPos;
		state = sOnGround;
		prevState = sOnGround;
		teleportFrm = 0;
		isTeleporting = false;
		isFly = false;
		return;
	}

	if (teleportFrm % 20 > 10) {
		mesh->m_vDiffuse = VECTOR4(1, 1, 1, 0.5f);
	}
	else {
		mesh->m_vDiffuse = VECTOR4(1, 1, 1, 1);
	}
	teleportFrm++;
}

void Player::CheckAtkCoillision()
{
	// ゴミに攻撃を当てる
	std::list<Leaf*> dusts = ObjectManager::FindGameObjects<Leaf>();

	finishAtkAnim = true;

	// 攻撃判定発生位置格納用配列
	vector<VECTOR3> attackPositions;

	int attackNum = 1; //攻撃判定数
	// 範囲攻撃が可能な時、攻撃判定を3つ生成
	if (canRangeAtk) {
		attackNum = 3;
	}

	// 攻撃判定の発生位置を計算
	for (int i = 0; i < attackNum; i++) {
		VECTOR3 forward;
		// プレイヤーのY軸回転行列(範囲攻撃の時、プレイヤーの前方と斜め前2個所に攻撃発生)
		MATRIX4X4 rotY;
		if (i > 0) {
			rotY = XMMatrixRotationY(transform.rotation.y + pow(-1,i)* 1.25f);
			// 斜め前の攻撃はやや前方に
			forward = VECTOR3(0, 0, 1.8f * atkRange); // 前方ベクトル
		}
		else {
			forward = VECTOR3(0, 0, 1.2f * atkRange); // 前方ベクトル
			rotY = XMMatrixRotationY(transform.rotation.y);
		}
		forward = forward * rotY; // プレイヤーの向いてる方への移動量
		VECTOR3 pos = forward + transform.position;

		// 攻撃判定位置情報を格納
		attackPositions.push_back(pos);

		//エフェクトの再生
		new SlashEffect(pos, atkRange);
	}

	// 攻撃判定数分だけ当たり判定を計算
	for (VECTOR3 atkPos : attackPositions) {
		for (Leaf* d : dusts) {
			SphereCollider dCol = d->Collider(d->GetNum()); //ゴミの判定球
			SphereCollider atkCol = Collider();			//攻撃判定の球

			//攻撃判定の球を作る
			atkCol.center = atkPos;
			atkCol.radius = 1.0f * atkRange;
			VECTOR3 pushVec = atkCol.center - dCol.center;
			float rSum = atkCol.radius + dCol.radius;
			if (pushVec.LengthSquare() < rSum * rSum) { // 球の当たり判定
				// 当たってる
				GameDevice()->m_pDI->PlayJoyEffect(2, 1, playerNum); // 振動の再生
				d->AddDamage(this, 1); //ダメージを与える
				if (d->GetNum() == 3) { //透明化
					itemNum = 0;
				}
			}
		}

		//他プレイヤーへの攻撃判定
		for (Player* p : otherPlayers) {
			SphereCollider pCol = p->Collider(); //他プレイヤーの判定球
			SphereCollider atkCol = Collider();		//攻撃判定の球
			
			//攻撃判定の球を作る
			atkCol.center = atkPos;
			atkCol.radius = 1.0f * atkRange;
			VECTOR3 pushVec = pCol.center - atkCol.center;

			float rSum = atkCol.radius + pCol.radius;
			if (pushVec.LengthSquare() < rSum * rSum && !p->GetIsDamage()) { // 球の当たり判定
				// 当たってる
				GameDevice()->m_pDI->PlayJoyEffect(0, 1, playerNum); // 振動の再生
				GameDevice()->blowSE->Play();
				GameDevice()->blowCheerSE->Play();
				pushVec = XMVector3Normalize(pushVec);
				pushVec *= 0.2f;
				pushVec.y = 0.201f;
				p->SetBlowVec(pushVec);
				p->SetSpeedY(pushVec.y);
				p->SetPlayerState(sBlow);
				p->SetPlayerPrevState(sBlow);
				p->SetIsBlow();
				p->animator->MergePlay(aBlow, 0);
				p->animator->SetPlaySpeed(2.0f);
				AddLeaf(p->GetLeaf());
				p->ResetLeaf();
				new LeafEffect(p->Position(), VECTOR3(1, 1, 1), p->GetLeaf());
				pd->SetBlowPlayer(playerNum,p->GetPlayerNum());
				knockOutCount++;
			}
		}
	}

}
