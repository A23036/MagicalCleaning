#include "Player.h"
#include "../Libs/Imgui/imgui.h"
#include <dinput.h>
#include "CsvReader.h"
#include "Stage.h"
#include "Camera.h"
#include "Dust.h"
#include "DustBox.h"
#include "SlashEffect.h"
#include "PowerUpEffect.h"
#include "LeafEffect.h"
#include "JumpEffect.h"

Player::Player()
{
}

Player::Player(VECTOR3 pos, VECTOR3 rot, int num)//セレクトシーン/リザルトシーンで使用
{
	animator = new Animator(); // インスタンスを作成

	mesh = new CFbxMesh();

	std::string f = "Data/Player/Color" + std::to_string(num);
	mesh->Load((f + "/witch.mesh").c_str());

	mesh->LoadAnimation(aIdle, (f + "/standby.anmx").c_str(), true);
	

	animator->SetModel(mesh); // このモデルでアニメーションする
	animator->Play(aIdle);
	animator->SetPlaySpeed(1.0f);

	transform.position = pos;
	transform.rotation = rot;

	state = sStandby;
}


Player::Player(int num) : playerNum(num) // プレイシーンで使用
{
	ObjectManager::SetDrawOrder(this, -100);

	CsvLoad(); // csvからデータの設定

	// プレイヤーの持つ箒の生成
	child = new Broom(this,playerNum);

	animator = new Animator(); // インスタンスを作成

	mesh = new CFbxMesh();

	transform.position = VECTOR3(0, 0, 0);
	transform.rotation = VECTOR3(0, 0, 0);
	deltaTime = 0.0f;

	state = sOnGround;
	prevState = sOnGround;

	speedY = 0;
	score = 0;
	leaf = 0;
	mp = 0;
	weight = 0;
	jumpCount = 0;
	chargeSpeed = 0;
	chargeFrm = 0;
	invisibleTime = 0;

	isDash = false;
	isFly = false;
	isBlow = false;
	canFly = false;
	finishAtkAnim = false;
	atkComboFlg = false;
	isMagicReady = false;
	isInvisible = false;

	blowVec = VECTOR3(0, 0, 0);

	comboWaitFrm = 20;

	msNum = 0;
	jnNum = 0;
	asNum = 0;
	arNum = 0;
	cwNum = 0;

	moveSpeed	= MoveSpeedT[msNum];
	jumpNum		= JumpNumT[jnNum];
	atkSpeed	= AtkSpeedT[asNum];
	atkRange	= AtkRangeT[arNum];
	carWeight	= CarWeightT[cwNum];

	selectPower = 0;
	anmFrame = 0;
}

Player::~Player()
{
	SAFE_DELETE(mesh);
	SAFE_DELETE(animator);
	SAFE_DELETE(csv);
}

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

	std::string f = "Data/Player/Color" + std::to_string(color);
	mesh->Load((f + "/witch.mesh").c_str());

	mesh->LoadAnimation(aIdle, (f + "/idle.anmx").c_str(), true);
	mesh->LoadAnimation(aRun, (f + "/run.anmx").c_str(), true);
	mesh->LoadAnimation(aJump, (f + "/jump.anmx").c_str(), false);
	mesh->LoadAnimation(aJump2, (f + "/jump2.anmx").c_str(), false);
	mesh->LoadAnimation(aFly, (f + "/fly.anmx").c_str(), true);
	mesh->LoadAnimation(aFall, (f + "/fall.anmx").c_str(), true);
	mesh->LoadAnimation(aAttack1, (f + "/attack1.anmx").c_str(), false);
	mesh->LoadAnimation(aAttack2, (f + "/attack2.anmx").c_str(), false);
	mesh->LoadAnimation(aAttack3, (f + "/attack3.anmx").c_str(), false);
	mesh->LoadAnimation(aChargeReady, (f + "/chargeReady.anmx").c_str(), false);
	mesh->LoadAnimation(aCharge, (f + "/charge.anmx").c_str(), true);
	mesh->LoadAnimation(aBlow, (f + "/blow.anmx").c_str(), true);

	animator->SetModel(mesh); // このモデルでアニメーションする
	animator->Play(aIdle);
	animator->SetPlaySpeed(1.0f);
}

void Player::Update()
{
	deltaTime = 60 * SceneManager::DeltaTime();
	posOld = transform.position;    // -- 2024.12.2

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
	
	/*
	ImGui::SetNextWindowPos(ImVec2(600, 0));
	ImGui::SetNextWindowSize(ImVec2(100, 120));
	ImGui::Begin("Player");
	if (ImGui::Button("DataLoad")) {
		CsvLoad();
	}
	ImGui::End();
	
	
	ImGui::SetNextWindowPos(ImVec2(0, 50));
	ImGui::SetNextWindowSize(ImVec2(100, 160));
	ImGui::Begin("state");
	switch (state) {
	case sStandby:
		ImGui::Text("sStandby");
		break;
	case sOnGround:
		ImGui::Text("sOnGround");
		break;
	case sJump:
		ImGui::Text("sJump");
		break;
	case sAttack1:
		ImGui::Text("sAttack1");
		break;
	case sAttack2:
		ImGui::Text("sAttack2");
		break;
	case sAttack3:
		ImGui::Text("sAttack3");
		break;
	case sStop:
		ImGui::Text("sStop");
		break;
	}
	ImGui::End();
	*/

	if (dc->GetIsPlay()) {
		animator->Update();
		state = prevState;
	}
	else
	{
		state = sStop;
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
		UpdateAttack1();
		break;
	case sAttack2:
		UpdateAttack2();
		break;
	case sAttack3:
		UpdateAttack3();
		break;
	case sCharge:
		UpdateCharge();
		break;
	case sBlow:
		UpdateBlow();
		break;
	default:
		break;
	}
	
	auto di = GameDevice()->m_pDI;
	if ((di->CheckKey(KD_TRG, DIK_UP) && playerNum == 0) || di->CheckJoy(KD_TRG, 2, playerNum)) { //能力強化
		switch (selectPower) {
		case pMS:
			if (mp >= MoveSpeedC[msNum] && msNum < MsTableNum - 1)
			{
				mp -= MoveSpeedC[msNum];
				msNum++;
				moveSpeed = MoveSpeedT[msNum];
				new PowerUpEffect(this,transform.position,selectPower);
			}
			break;
		case pJN:
			if (mp >= JumpNumC[jnNum] && jnNum < JnTableNum - 1)
			{
				mp -= JumpNumC[jnNum];
				jnNum++;
				new PowerUpEffect(this, transform.position, selectPower);
				if (jnNum == JnTableNum - 1) {
					canFly = true;
				}
				else {
					jumpNum = JumpNumT[jnNum];
				}
			}
			break;
		case pAS:
			if (mp >= AtkSpeedC[asNum] && asNum < AsTableNum - 1)
			{
				mp -= AtkSpeedC[asNum];
				asNum++;
				atkSpeed = AtkSpeedT[asNum];
				new PowerUpEffect(this, transform.position, selectPower);
			}
			break;
		case pAR:
			if (mp >= AtkRangeC[arNum] && arNum < ArTableNum - 1)
			{
				mp -= AtkRangeC[arNum];
				arNum++;
				atkRange = AtkRangeT[arNum];
				new PowerUpEffect(this, transform.position, selectPower);
			}
			break;
		case pCW:
			if (mp >= CarWeightC[cwNum] && cwNum < CwTableNum - 1)
			{
				mp -= CarWeightC[cwNum];
				cwNum++;
				carWeight = CarWeightT[cwNum];
				new PowerUpEffect(this, transform.position, selectPower);
			}
			break;
		}
	}
	
	
	if ((di->CheckKey(KD_TRG, DIK_LEFT) && playerNum == 0) || di->CheckJoy(KD_TRG, 4, playerNum)) { //強化能力変更
		selectPower--;
		if (selectPower < 0) {
			selectPower = 4;
		}
	}
	if ((di->CheckKey(KD_TRG, DIK_RIGHT) && playerNum == 0) || di->CheckJoy(KD_TRG, 5, playerNum)) { //強化能力変更
		selectPower++;
		if (selectPower > 4) {
			selectPower = 0;
		}
	}



	//当たり判定処理
	//VECTOR3 push;
	
	//if (st->HitSphereToMesh(Collider(), &push)) {
	//	transform.position += push;
	//}

	//場外処理
	if (transform.position.y <= -30.0f) {
		transform.position = VECTOR3(0, 20, 0);
		ResetLeaf();
	}

	//当たり判定処理 // -- 2024.12.2
	//ステージ
	st->MapCol()->IsCollisionMoveGravity(posOld, transform.position);

	//吹っ飛び
	transform.position += blowVec;

	//透明化
	if (isInvisible) {
		mesh->m_vDiffuse = VECTOR4(1, 1, 1, 0.2f);
		invisibleTime++;
		if (invisibleTime * (1.0f / 60.0f) > InvisibleTime) {
			invisibleTime = 0;
			isInvisible = false;
			mesh->m_vDiffuse = VECTOR4(1, 1, 1, 1);
		}
	}

	// ImGuiウィンドウの位置とサイズを設定
	/*
	ImGui::SetNextWindowPos(ImVec2(0, 60));
	ImGui::SetNextWindowSize(ImVec2(120, 400));
	ImGui::Begin("PlayerPos");
	ImGui::InputFloat("X", &transform.position.x);
	ImGui::InputFloat("Y", &transform.position.y);
	ImGui::InputFloat("Z", &transform.position.z);
	ImGui::InputFloat("speedY", &speedY);
	ImGui::End();
	
	// 入力ボタン確認
	ImGui::SetNextWindowPos(ImVec2(0, 220));
	ImGui::SetNextWindowSize(ImVec2(200, 120));
	ImGui::Begin("CheckJoy");
	auto di = GameDevice()->m_pDI;
	auto* pdi = GameDevice()->m_pDI;
	DIJOYSTATE2 joyState = pdi->GetJoyState(playerNum);
	for (int i = 0; i < 32; i++)
	{
		// 各ボタンの押下状態を確認
		if (joyState.rgbButtons[i] & 0x80) // ボタン i が押されている場合
		{
			ImGui::Text("%dP : Button %d is pressed", playerNum,i); // ボタン番号を表示
		}
	}
	
	float rx = di->GetJoyState().lRx;
	float ry = di->GetJoyState().lRy;
	float rz = di->GetJoyState().lRz;
	ImGui::End();
	*/
	// Dustにめり込まないようにする
	// 自分の座標は、transform.position
	// Dustの座標を知る
	
	std::list<Dust*> dusts = ObjectManager::FindGameObjects<Dust>();
	
	for (Dust* dust : dusts) {
		SphereCollider tCol = dust->Collider(dust->GetNum());
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
	
	// DustBoxにめり込まないようにする
	// 自分の座標は、transform.position
	// DustBoxの座標を知る
	/*
	std::list<DustBox*> boxs = ObjectManager::FindGameObjects<DustBox>();

	for (DustBox* box : boxs) {
		SphereCollider tCol = box->Collider();
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
	*/
	// playerにめり込まないようにする
	// 自分の座標は、transform.position
	// playerの座標を知る
	
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
	
void Player::Draw()
{
	if (ObjectManager::DrawCounter() != playerNum) { //透明のプレイヤーを描画しない
		if (isInvisible) {
			return;
		}
	}
	Object3D::Draw(); // 継承元の関数を呼ぶ
}

void Player::CsvLoad()
{
	// csvからデータ読み込み
	csv = new CsvReader("data/csv/Paramater.csv");
	if (csv->GetLines() < 1) {
		MessageBox(NULL, "Paramater.csvが読めません", "エラー", MB_OK);
	}

	for (int i = 1; i < csv->GetLines(); i++) { //CSVファイルから設定の読み込み
		if (csv->GetString(i, 0) == "Player") {
			if (csv->GetString(i, 1) == "Gravity") {		// 重力
				GRAVITY = csv->GetFloat(i, 3);
			}
			if (csv->GetString(i, 1) == "JumpPower") {		// ジャンプ力
				JUMP_POWER = csv->GetFloat(i, 3);
			}
			if (csv->GetString(i, 1) == "MoveSpeed") {		// 移動速度
				MOVE_SPEED = csv->GetFloat(i, 3);
			}
			if (csv->GetString(i, 1) == "InvisibleTime") {		// 透明化時間
				InvisibleTime = csv->GetInt(i, 3);
			}
			if (csv->GetString(i, 1) == "MoveSpeedT") {		// 移動速度テーブル
				for (int j = 0; j < MsTableNum; j++) {
					MoveSpeedT[j] = csv->GetFloat(i, 3+j);
				}
			}
			if (csv->GetString(i, 1) == "JumpNumT") {		// ジャンプ回数テーブル
				for (int j = 0; j < JnTableNum; j++) {
					JumpNumT[j] = csv->GetInt(i, 3 + j);
				}
			}
			if (csv->GetString(i, 1) == "AtkSpeedT") {		// 攻撃速度テーブル
				for (int j = 0; j < AsTableNum; j++) {
					AtkSpeedT[j] = csv->GetFloat(i, 3 + j);
				}
			}
			if (csv->GetString(i, 1) == "AtkRangeT") {		// 攻撃範囲テーブル
				for (int j = 0; j < ArTableNum; j++) {
					AtkRangeT[j] = csv->GetFloat(i, 3 + j);
				}
			}
			if (csv->GetString(i, 1) == "CarWeightT") {		// 運搬可能重量テーブル
				for (int j = 0; j < CwTableNum; j++) {
					CarWeightT[j] = csv->GetInt(i, 3 + j);
				}
			}
			if (csv->GetString(i, 1) == "MoveSpeedC") {		// 移動速度コストテーブル
				for (int j = 0; j < MsTableNum; j++) {
					MoveSpeedC[j] = csv->GetInt(i, 3 + j);
				}
			}
			if (csv->GetString(i, 1) == "JumpNumC") {		// ジャンプ回数コストテーブル
				for (int j = 0; j < JnTableNum; j++) {
					JumpNumC[j] = csv->GetInt(i, 3 + j);
				}
			}
			if (csv->GetString(i, 1) == "AtkSpeedC") {		// 攻撃速度コストテーブル
				for (int j = 0; j < AsTableNum; j++) {
					AtkSpeedC[j] = csv->GetInt(i, 3 + j);
				}
			}
			if (csv->GetString(i, 1) == "AtkRangeC") {		// 攻撃範囲コストテーブル
				for (int j = 0; j < ArTableNum; j++) {
					AtkRangeC[j] = csv->GetInt(i, 3 + j);
				}
			}
			if (csv->GetString(i, 1) == "CarWeightC") {		// 運搬可能重量コストテーブル
				for (int j = 0; j < CwTableNum; j++) {
					CarWeightC[j] = csv->GetInt(i, 3 + j);
				}
			}
		}
		if (csv->GetString(i, 0) == "Dust") {
			break;
		}
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
		return MsTableNum;
	case pJN:
		return JnTableNum;
	case pAS:
		return AsTableNum;
	case pAR:
		return ArTableNum;
	case pCW:
		return CwTableNum;
	default:
		return 0;
	}
}

void Player::SetPlayerColor(int color)
{
	this->color = color;
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

void Player::SetIsBlow(bool isBlow)
{
	isDash = false; 
	isFly = false;
	finishAtkAnim = false;
	if (mcEffect != nullptr) {
		mcEffect->SetIsFinish();
	}
	this->isBlow = isBlow;
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

void Player::UpdateOnGround()
{
	auto di = GameDevice()->m_pDI;

	int x = di->GetJoyState(playerNum).lX;	// 右:1000 / 左:-1000
	int y = di->GetJoyState(playerNum).lY;	// 下:1000 / 上:-1000
	/*
	ImGui::SetNextWindowPos(ImVec2(0, 300));
	ImGui::SetNextWindowSize(ImVec2(200, 200));
	ImGui::Begin("Joystick");
	ImGui::InputInt("IX", &x);
	ImGui::InputInt("IY", &y);
	ImGui::End();
	*/
	
	if (st->MapCol()->IsCollisionMoveGravity(posOld, transform.position) == clFall) {
		state = sJump;
		return;
	}

	/*
	VECTOR3 start = transform.position;
	start.y += 1.0f;
	VECTOR3 end = transform.position;
	if (!HitLineToMesh(start, end)) {
		state = sJump;
		return;
	}
	*/   

	// 2024.10.26 プレイヤー操作をコントローラーに対応↓
	
	// コントローラースティックの入力を取得
	// 正規化して -1.0～1.0 の範囲にする
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
		state = sJump;
	}
	if ((di->CheckKey(KD_TRG, DIK_N) && playerNum == 0) || di->CheckJoy(KD_TRG, 0, playerNum)) { //攻撃
		animator->MergePlay(aAttack1,0);
		animator->SetPlaySpeed(1.0f * atkSpeed);
		transform.rotation.y += 15 * DegToRad; //正面方向に回転させる
		state = sAttack1;
		isDash = false;
		anmFrame = 0;
	}
	if ((di->CheckKey(KD_TRG, DIK_M) && playerNum == 0) || di->CheckJoy(KD_TRG, 3, playerNum)) { //MP変換
		animator->MergePlay(aChargeReady);
		animator->SetPlaySpeed(1.0f);
		state = sCharge;
		isDash = false;
		chargeSpeed = 1.0f; //MP変換速度初期値
		mcEffect = new MagicCircleEffect(transform.position, playerNum);
	}
}


void Player::UpdateJump()
{
	transform.position.y += speedY * deltaTime;
	if (canFly && GameDevice()->m_pDI->CheckJoy(KD_DAT, 7, playerNum)) {
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
	// 正規化して -1.0～1.0 の範囲にする
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

	if (di->CheckJoy(KD_TRG, 1, playerNum) && jumpCount <= jumpNum) {
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
		jumpCount++;
	}


	if (animator->Finished()) {
		if (isFly) {
			animator->MergePlay(aFly);
		}
		else
		{
			animator->MergePlay(aFall);
		}
		
	}

	if (st->MapCol()->IsCollisionMoveGravity(posOld, transform.position) != clFall) {
		// ジャンプ終了
		isFly = false;
		state = sOnGround;
		jumpCount = 0;
		speedY = 0;
		blowVec = VECTOR3(0, 0, 0);
	}
}

void Player::UpdateAttack1()
{	
	anmFrame++;

	// ゴミに攻撃を当てる
	std::list<Dust*> dusts = ObjectManager::FindGameObjects<Dust>();
	
	if (!finishAtkAnim && animator->CurrentFrame() >= 20.0f/atkSpeed) { //攻撃のヒットしたタイミング
		finishAtkAnim = true;
		//エフェクトの再生
		VECTOR3 forward = VECTOR3(0, 0, 1.1f*atkRange); // 回転してない時の移動量
		MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Yの回転行列
		forward = forward * rotY; // キャラの向いてる方への移動量
		VECTOR3 pos = forward + transform.position;
		new SlashEffect(pos,atkRange);

		for (Dust* d : dusts) {
			SphereCollider dCol = d->Collider(d->GetNum()); //ゴミの判定球
			SphereCollider atkCol = Collider();			//攻撃判定の球
			
			atkCol.center = transform.position + forward; //攻撃判定の球を作る
			atkCol.radius = 1.0f * atkRange;
			VECTOR3 pushVec = atkCol.center - dCol.center;
			float rSum = atkCol.radius + dCol.radius;
			if (pushVec.LengthSquare() < rSum * rSum) { // 球の当たり判定
				// 当たってる
				d->AddDamage(this,1); //ダメージを与える
				if (d->GetNum() == 3) { //透明化
					invisibleTime = 0;
					isInvisible = true;
				}
			}
		}
		//他プレイヤーへの攻撃判定
		for (Player* p : otherPlayers) {
			SphereCollider pCol = p->Collider(); //他プレイヤーの判定球
			SphereCollider atkCol = Collider();		//攻撃判定の球
			atkCol.center = transform.position + forward; //攻撃判定の球を作る
			atkCol.radius = 1.0f * atkRange;
			VECTOR3 pushVec = pCol.center - atkCol.center;
			
			float rSum = atkCol.radius + pCol.radius;
			if (pushVec.LengthSquare() < rSum * rSum) { // 球の当たり判定
				// 当たってる
				pushVec = XMVector3Normalize(pushVec);
				pushVec *= 0.1f;
				pushVec.y = 0.201f;
				p->SetBlowVec(pushVec);
				p->SetSpeedY(pushVec.y);
				p->SetPlayerState(sBlow);
				p->SetPlayerPrevState(sBlow);
				p->SetIsBlow(true);
				p->animator->MergePlay(aBlow, 0);
				p->animator->SetPlaySpeed(2.0f);
				AddLeaf(p->GetLeaf());
				p->ResetLeaf();
				new LeafEffect(p->Position(), VECTOR3(1, 1, 1), p->GetLeaf());
			}
		}
	}
	
	auto di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_TRG, DIK_N) || di->CheckJoy(KD_TRG, 0, playerNum)) { //攻撃中の先行入力受付
		atkComboFlg = true;
		anmFrame = 0;
	}

	if (anmFrame >= comboWaitFrm) { //連続入力後一定時間で連続攻撃フラグ解除
		atkComboFlg = false;
	}

	if (animator->Finished())
	{
		//攻撃アニメーションの終了
		finishAtkAnim = false;
		animator->SetPlaySpeed(1.0f);
		transform.rotation.y -= 15 * DegToRad; //回転を戻す
		if (atkComboFlg) { //連続攻撃
			animator->MergePlay(aAttack2, 0);
			animator->SetPlaySpeed(1.0f * atkSpeed);
			transform.rotation.y += 15 * DegToRad; //正面方向に回転させる
			state = sAttack2;
			anmFrame = 0;
		}
		else {
			//地上状態へ
			state = sOnGround;
		}
		atkComboFlg = false;
	}
	
}

void Player::UpdateAttack2()
{
	anmFrame++;

	// ゴミに攻撃を当てる
	std::list<Dust*> dusts = ObjectManager::FindGameObjects<Dust>();

	if (!finishAtkAnim && animator->CurrentFrame() >= 20.0f / atkSpeed) { //攻撃のヒットしたタイミング
		finishAtkAnim = true;
		//エフェクトの再生
		VECTOR3 forward = VECTOR3(0, 0, 1.1f * atkRange); // 回転してない時の移動量
		MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Yの回転行列
		forward = forward * rotY; // キャラの向いてる方への移動量
		VECTOR3 pos = forward + transform.position;
		new SlashEffect(pos, atkRange);

		for (Dust* d : dusts) {
			SphereCollider dCol = d->Collider(d->GetNum()); //ゴミの判定球
			SphereCollider atkCol = Collider();			//攻撃判定の球

			atkCol.center = transform.position + forward; //攻撃判定の球を作る
			atkCol.radius = 1.0f * atkRange;
			VECTOR3 pushVec = atkCol.center - dCol.center;
			float rSum = atkCol.radius + dCol.radius;
			if (pushVec.LengthSquare() < rSum * rSum) { // 球の当たり判定
				// 当たってる
				d->AddDamage(this, 1); //ダメージを与える
				if (d->GetNum() == 3) { //透明化
					invisibleTime = 0;
					isInvisible = true;
				}
			}
		}
		//他プレイヤーへの攻撃判定
		for (Player* p : otherPlayers) {
			SphereCollider pCol = p->Collider(); //他プレイヤーの判定球
			SphereCollider atkCol = Collider();		//攻撃判定の球
			atkCol.center = transform.position + forward; //攻撃判定の球を作る
			atkCol.radius = 1.0f * atkRange;
			VECTOR3 pushVec = pCol.center - atkCol.center;

			float rSum = atkCol.radius + pCol.radius;
			if (pushVec.LengthSquare() < rSum * rSum) { // 球の当たり判定
				// 当たってる
				pushVec = XMVector3Normalize(pushVec);
				pushVec *= 0.1f;
				pushVec.y = 0.201f;
				p->SetBlowVec(pushVec);
				p->SetSpeedY(pushVec.y);
				p->SetPlayerState(sBlow);
				p->SetPlayerPrevState(sBlow);
				p->SetIsBlow(true);
				p->animator->MergePlay(aBlow, 0);
				p->animator->SetPlaySpeed(2.0f);
				AddLeaf(p->GetLeaf());
				p->ResetLeaf();
				new LeafEffect(p->Position(), VECTOR3(1, 1, 1), p->GetLeaf());
			}
		}
	}

	auto di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_TRG, DIK_N) || di->CheckJoy(KD_TRG, 0, playerNum)) { //攻撃中の先行入力受付
		atkComboFlg = true;
		anmFrame = 0;
	}

	if (anmFrame >= comboWaitFrm) { //連続入力後一定時間で連続攻撃フラグ解除
		atkComboFlg = false;
	}

	if (animator->Finished())
	{
		//攻撃アニメーションの終了
		finishAtkAnim = false;
		animator->SetPlaySpeed(1.0f);
		transform.rotation.y -= 15 * DegToRad; //回転を戻す
		if (atkComboFlg) { //連続攻撃
			animator->MergePlay(aAttack3, 0);
			animator->SetPlaySpeed(1.0f * atkSpeed);
			//transform.rotation.y += 15 * DegToRad; //正面方向に回転させる
			state = sAttack3;
			anmFrame = 0;
		}
		else {
			//地上状態へ
			state = sOnGround;
		}
		atkComboFlg = false;
	}
}

void Player::UpdateAttack3()
{
	// ゴミに攻撃を当てる
	std::list<Dust*> dusts = ObjectManager::FindGameObjects<Dust>();

	if (!finishAtkAnim && animator->CurrentFrame() >= 30.0f / atkSpeed) { //攻撃のヒットしたタイミング
		finishAtkAnim = true;
		//エフェクトの再生
		VECTOR3 forward = VECTOR3(0, 0, 1.1f * atkRange); // 回転してない時の移動量
		MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Yの回転行列
		forward = forward * rotY; // キャラの向いてる方への移動量
		VECTOR3 pos = forward + transform.position;
		new SlashEffect(pos, atkRange);

		for (Dust* d : dusts) {
			SphereCollider dCol = d->Collider(d->GetNum()); //ゴミの判定球
			SphereCollider atkCol = Collider();			//攻撃判定の球

			atkCol.center = transform.position + forward; //攻撃判定の球を作る
			atkCol.radius = 1.0f * atkRange;
			VECTOR3 pushVec = atkCol.center - dCol.center;
			float rSum = atkCol.radius + dCol.radius;
			if (pushVec.LengthSquare() < rSum * rSum) { // 球の当たり判定
				// 当たってる
				d->AddDamage(this, 1); //ダメージを与える
				if (d->GetNum() == 3) { //透明化
					invisibleTime = 0;
					isInvisible = true;
				}
			}
		}
		//他プレイヤーへの攻撃判定
		for (Player* p : otherPlayers) {
			SphereCollider pCol = p->Collider(); //他プレイヤーの判定球
			SphereCollider atkCol = Collider();		//攻撃判定の球
			atkCol.center = transform.position + forward; //攻撃判定の球を作る
			atkCol.radius = 1.0f * atkRange;
			VECTOR3 pushVec = pCol.center - atkCol.center;

			float rSum = atkCol.radius + pCol.radius;
			if (pushVec.LengthSquare() < rSum * rSum) { // 球の当たり判定
				// 当たってる
				pushVec = XMVector3Normalize(pushVec);
				pushVec *= 0.1f;
				pushVec.y = 0.201f;
				p->SetBlowVec(pushVec);
				p->SetSpeedY(pushVec.y);
				p->SetPlayerState(sBlow);
				p->SetPlayerPrevState(sBlow);
				p->SetIsBlow(true);
				p->animator->MergePlay(aBlow, 0);
				p->animator->SetPlaySpeed(2.0f);
				AddLeaf(p->GetLeaf());
				p->ResetLeaf();
				new LeafEffect(p->Position(), VECTOR3(1, 1, 1), p->GetLeaf());
			}
		}
	}

	if (animator->Finished())
	{
		//攻撃アニメーションの終了
		finishAtkAnim = false;
		animator->SetPlaySpeed(1.0f);
		//transform.rotation.y -= 15 * DegToRad; //回転を戻す
		state = sOnGround;
		atkComboFlg = false;
	}
}

void Player::UpdateCharge()
{
	auto di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_UTRG, DIK_M) || di->CheckJoy(KD_UTRG, 3, playerNum)) {
		state = sOnGround;
		isMagicReady = false;
		chargeFrm = 0;
		mcEffect->SetIsFinish();
		return;
	}
	
	if (animator->PlayingID() == aChargeReady && animator->Finished()) {
		animator->SetPlaySpeed(1.0f);
		animator->MergePlay(aCharge);
		isMagicReady = true;
	}

	if (isMagicReady) {

		if (chargeSpeed <= (chargeFrm * (1.0f / 60.0f)) && leaf > 0) {
			new LeafEffect(transform.position,VECTOR3(0.5f, 0.5f, 0.5f),1);
			mp++;		//MP加算
			score++;	//スコア加算
			leaf--; //葉っぱを減らす
			if (chargeSpeed > 0.1f) {
				chargeSpeed -= 0.1f;
			}
			chargeFrm = 0;
		}

		chargeFrm++;
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
	// 正規化して -1.0～1.0 の範囲にする
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

	if (st->MapCol()->IsCollisionMoveGravity(posOld, transform.position) != clFall) {
		// 吹っ飛び終了
		isFly = false;
		state = sOnGround;
		jumpCount = 0;
		speedY = 0;
		blowVec = VECTOR3(0, 0, 0);
	}
}

//　プレイヤーの持つ箒
Broom::Broom(Object3D* parentModel, int num)
{
	ObjectManager::SetDrawOrder(this, -100);
	parent = parentModel;

	mesh = new CFbxMesh();

	std::string f = "Data/Player/Color" + std::to_string(num) + "/weapon";
	mesh->Load((f + "/broom.mesh").c_str());

	transform.position = VECTOR3(0, 0, 0);
	transform.rotation = VECTOR3(0, 0, -70 * DegToRad);
}

Broom::~Broom()
{
	SAFE_DELETE(mesh);
}

void Broom::Update()
{
	Player* pl = dynamic_cast<Player*>(parent);

	// 状態ごとの角度変更
	switch (pl->GetPlayerState()) {
	case sOnGround:
	case sJump:
		if (pl->GetIsDash() || pl->GetIsFly()) {
			transform.position = VECTOR3(7 * 0.01, -10 * 0.01, -35 * 0.01);
			transform.rotation = VECTOR3(77 * DegToRad, 75 * DegToRad, 90 * DegToRad);
		}
		else {
			transform.position = VECTOR3(0, 0, 0);
			transform.rotation = VECTOR3(0, 0, -70 * DegToRad);
		}
		break;
	case sBlow:
		transform.position = VECTOR3(0, 0, 0);
		transform.rotation = VECTOR3(0, 0, -70 * DegToRad);
		break;
	case sAttack1:
	case sAttack2:
	case sAttack3:
		transform.position = VECTOR3(0, 0, 0);
		transform.rotation = VECTOR3(0, 0, 0);
		break;
	case sCharge:
		transform.position = VECTOR3(0, 0, 0);
		transform.rotation = VECTOR3(45 * DegToRad, 0, 0);
		break;
	}
	if (pl->GetIsInvisible()) {
		mesh->m_vDiffuse = VECTOR4(1, 1, 1, 0.2f);
	}
	else {
		mesh->m_vDiffuse = VECTOR4(1, 1, 1, 1);
	}
}

void Broom::Draw()
{
	Player* pl = dynamic_cast<Player*>(parent);
	if (ObjectManager::DrawCounter() != pl->GetPlayerNum()) {
		if (pl->GetIsInvisible()) {
			return;
		}
	}
	mesh->Render(transform.matrix() * mat * parent->Matrix());
}

void Broom::SetPos(MATRIX4X4 m)
{
	mat = m;
}
