#include "Player.h"
#include "../Libs/Imgui/imgui.h"
#include <dinput.h>
#include "CsvReader.h"
#include "Stage.h"
#include "Camera.h"
#include "Dust.h"
#include "DustBox.h"

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
	CsvLoad(); // csvからデータの設定

	dc = ObjectManager::FindGameObject<DataCarrier>();

	// プレイヤーの持つ箒の生成
	child = new Broom(this,playerNum);

	animator = new Animator(); // インスタンスを作成

	mesh = new CFbxMesh();

	std::string f = "Data/Player/Color" + std::to_string(playerNum);
	mesh->Load((f + "/witch.mesh").c_str());

	mesh->LoadAnimation(aIdle, (f + "/idle.anmx").c_str(), true);
	mesh->LoadAnimation(aRun, (f + "/run.anmx").c_str(), true);
	//mesh->LoadAnimation(aWalk, (f + "/walk.anmx").c_str(), true);
	mesh->LoadAnimation(aJump, (f + "/jump.anmx").c_str(), false);
	mesh->LoadAnimation(aJump2, (f + "/jump2.anmx").c_str(), false);
	mesh->LoadAnimation(aFly, (f + "/fly.anmx").c_str(), false);
	mesh->LoadAnimation(aAttack1, (f + "/attack1.anmx").c_str(), false);
	mesh->LoadAnimation(aAttack2, (f + "/attack2.anmx").c_str(), false);
	//mesh->LoadAnimation(aAttack3, (f + "/attack3.anmx").c_str(), false);

	animator->SetModel(mesh); // このモデルでアニメーションする
	animator->Play(aIdle);
	animator->SetPlaySpeed(1.0f);
	
	transform.position = VECTOR3(0, 0, 0);
	transform.rotation = VECTOR3(0, 0, 0);
	state = sOnGround;
	curState = sOnGround;
	speedY = 0;
	mp = 0;
	weight = 0;
	jumpCount = 0;
	isFly = false;
	finishAtkAnim = true;

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
}

Player::~Player()
{
	SAFE_DELETE(mesh);
	SAFE_DELETE(animator);
}

void Player::Update()
{
	// 箒の位置情報更新
	if (state != sStandby) { //キャラセレクト画面では持たない

		MATRIX4X4 bone;
		if (state == sAttack2) {
			bone = mesh->GetFrameMatrices(18);// プレイヤーの原点からの左手の位置(18は左手)
		}
		else {
			bone = mesh->GetFrameMatrices(0);// プレイヤーの原点からの右手の位置(0は右手)
		}
		if (animator->PlayingID() == aFly) {
			bone = mesh->GetFrameMatrices(2);//プレイヤーのルート位置
		}

		child->SetPos(bone);
	}
	
	
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(100, 60));
	ImGui::Begin("isFly");
	if (isFly) {
		ImGui::Text("Fly");
	}
	else
	{
		ImGui::Text("notFly");
	}
	ImGui::End();
	
	/**
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
	case sStop:
		ImGui::Text("sStop");
		break;
	}
	ImGui::End();
	*/

	if (dc->GetIsPlay()) {
		animator->Update();
		state = curState;
	}
	else
	{
		state = sStop;
	}

	switch (state) {
	case sStandby:
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
		UpdateAttack1();
		break;
	case sAttack3:
		UpdateAttack1();
		break;
	case sStop:
		return;
	default:
		break;
	}

	if (GameDevice()->m_pDI->CheckJoy(KD_TRG, 3, playerNum)) { //能力強化
		switch (selectPower) {
		case pMS:
			if (mp >= MoveSpeedC[msNum] && msNum < MsTableNum - 1)
			{
				mp -= MoveSpeedC[msNum];
				msNum++;
				moveSpeed = MoveSpeedT[msNum];
			}
			break;
		case pJN:
			if (mp >= JumpNumC[jnNum] && jnNum < JnTableNum - 1)
			{
				mp -= JumpNumC[jnNum];
				jnNum++;
				jumpNum = JumpNumT[jnNum];
			}
			break;
		case pAS:
			if (mp >= AtkSpeedC[asNum] && asNum < AsTableNum - 1)
			{
				mp -= AtkSpeedC[asNum];
				asNum++;
				atkSpeed = AtkSpeedT[asNum];
			}
			break;
		case pAR:
			if (mp >= AtkRangeC[arNum] && arNum < ArTableNum - 1)
			{
				mp -= AtkRangeC[arNum];
				arNum++;
				atkRange = AtkRangeT[arNum];
			}
			break;
		case pCW:
			if (mp >= CarWeightC[cwNum] && cwNum < CwTableNum - 1)
			{
				mp -= CarWeightC[cwNum];
				cwNum++;
				carWeight = CarWeightT[cwNum];
			}
			break;
		}
	}

	
	if (GameDevice()->m_pDI->CheckJoy(KD_TRG, 4, playerNum)) { //強化能力変更
		selectPower--;
		if (selectPower < 0) {
			selectPower = 4;
		}
	}
	if (GameDevice()->m_pDI->CheckJoy(KD_TRG, 5, playerNum)) { //強化能力変更
		selectPower++;
		if (selectPower > 4) {
			selectPower = 0;
		}
	}



	//当たり判定処理
	Stage* st = ObjectManager::FindGameObject<Stage>();
	VECTOR3 push;
	if (st->HitSphere(Collider(), &push)) {
		transform.position += push;
	}

	// ImGuiウィンドウの位置とサイズを設定
	/*
	ImGui::SetNextWindowPos(ImVec2(0, 60));
	ImGui::SetNextWindowSize(ImVec2(100, 100));
	ImGui::Begin("PlayerPos");
	ImGui::InputFloat("X", &transform.position.x);
	ImGui::InputFloat("Y", &transform.position.y);
	ImGui::InputFloat("Z", &transform.position.z);
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

	// playerにめり込まないようにする
	// 自分の座標は、transform.position
	// playerの座標を知る
	std::list<Player*> players = ObjectManager::FindGameObjects<Player>();

	for (Player* player : players) {
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

	curState = state;
}
	
void Player::Draw()
{
	Object3D::Draw(); // 継承元の関数を呼ぶ
	/*
	MATRIX4X4 tip = XMMatrixRotationRollPitchYawFromVector(VECTOR3(-33, 82, 0) * DegToRad);
	VECTOR3 tipPos = VECTOR3(0, 0, 1.2f) * tip;
	VECTOR3 tipPos = VECTOR3(0.9966, 0.6536, 0.140);
	MATRIX4X4 mat = transform.matrix();// 世界(ワールド)の中で、プレイヤーの位置と向き
	MATRIX4X4 bone = mesh->GetFrameMatrices(34); // プレイヤーの原点からの手首の位置(34は手首)
	VECTOR3 start = VECTOR3(0, 0, 0) * bone * mat;
	VECTOR3 end = tipPos * bone * mat;
	
	CSprite spr;
	spr.DrawLine3D(start, end, RGB(255, 0, 0));
	*/
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
	col.center = transform.position + VECTOR3(0, 0.6f, 0);
	col.radius = 0.58f;
	return col;
}

void Player::SetPlayerState(int state)
{
	this->state = state;
}

void Player::SetPlayerCurState(int state)
{
	curState = state;
}

void Player::AddMP(int n)
{
	mp += n;
}

void Player::AddWeight(int n)
{
	weight += n;

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
	Stage* st = ObjectManager::FindGameObject<Stage>();
	if (!(st->IsLandBlock(transform.position))) {
		// 空中
		state = sJump;
		return; //この下の処理は行わない
	}
	 
	// 2024.10.26 プレイヤー操作をコントローラーに対応↓
	
	// コントローラースティックの入力を取得
	// 正規化して -1.0～1.0 の範囲にする
	float ix = di->GetJoyState(playerNum).lX / 1000.0f;
	float iy = di->GetJoyState(playerNum).lY / 1000.0f;
	
	// スティックが入力されているか確認
	if (fabs(ix) > 0.1f || fabs(iy) > 0.1f) {
		// 入力に基づいて移動方向を設定
		VECTOR3 inputDirection = VECTOR3(ix, 0, -iy); // Z軸方向は反転

		// カメラのY軸回転を取得
		Camera* camera = ObjectManager::FindGameObject<Camera>();
		float cameraYRotation = camera->GetRotY(playerNum);

		// カメラの回転に基づく移動ベクトルの計算
		MATRIX4X4 cameraRotY = XMMatrixRotationY(cameraYRotation);
		VECTOR3 moveDirection = XMVector3TransformNormal(inputDirection, cameraRotY);
		moveDirection = XMVector3Normalize(moveDirection) * MOVE_SPEED * sqrt(ix * ix + iy * iy); // 傾き具合に応じた速度

		// 移動の適用
		transform.position += moveDirection * moveSpeed;

		// プレイヤーの回転をカメラ基準で方向を向かせる
		transform.rotation.y = cameraYRotation + atan2(ix, -iy); // カメラの回転に対してスティックの方向に合わせる

		if (sqrt(ix * ix + iy * iy) * moveSpeed > 2.0) {
			// 飛行アニメーションを再生
			animator->MergePlay(aFly, 0);
			animator->SetPlaySpeed(1.0f);
			isFly = true;
		}
		else {
			//走行速度に応じたアニメーションスピードを設定
			animator->SetPlaySpeed(sqrt(ix * ix + iy * iy) * moveSpeed);

			// 走行アニメーションを再生
			animator->MergePlay(aRun, 0);
			isFly = false;
		}
	}
	else {
		// スティックが傾いていない場合は待機アニメーションを再生
		animator->SetPlaySpeed(1.0f);
		animator->MergePlay(aIdle,0);
	}
	// 2024.10.26 プレイヤー操作をコントローラーに対応↑

	if ((di->CheckKey(KD_TRG, DIK_SPACE) || di->CheckJoy(KD_TRG, 2, playerNum) )) { //ジャンプ
		speedY = JUMP_POWER;
		animator->MergePlay(aJump,0);
		animator->SetPlaySpeed(1.0f);
		jumpCount++;
		state = sJump;
	}
	if (di->CheckKey(KD_TRG, DIK_N) || di->CheckJoy(KD_TRG, 0, playerNum)) { //攻撃ボタン
		animator->MergePlay(aAttack1,0);
		animator->SetPlaySpeed(1.0f * atkSpeed);
		transform.rotation.y += 15 * DegToRad; //正面方向に回転させる
		state = sAttack1;
	}
	if (di->CheckKey(KD_TRG, DIK_M) || di->CheckJoy(KD_TRG, 1, playerNum)) { //攻撃ボタン
		animator->MergePlay(aAttack2,0);
		animator->SetPlaySpeed(1.0f * atkSpeed);
		transform.rotation.y += 10 * DegToRad; //正面方向に回転させる
		state = sAttack2;
	}
}


void Player::UpdateJump()
{
	Stage* st = ObjectManager::FindGameObject<Stage>();
	transform.position.y += speedY;
	speedY -= GRAVITY;	// 重力

	auto di = GameDevice()->m_pDI;
	// コントローラースティックの入力を取得
	// 正規化して -1.0～1.0 の範囲にする
	float ix = di->GetJoyState(playerNum).lX / 1000.0f;
	float iy = di->GetJoyState(playerNum).lY / 1000.0f;

	// スティックが入力されているか確認
	if (fabs(ix) > 0.1f || fabs(iy) > 0.1f) {
		// 入力に基づいて移動方向を設定
		VECTOR3 inputDirection = VECTOR3(ix, 0, -iy); // Z軸方向は反転

		// カメラのY軸回転を取得
		Camera* camera = ObjectManager::FindGameObject<Camera>();
		float cameraYRotation = camera->GetRotY(playerNum);

		// カメラの回転に基づく移動ベクトルの計算
		MATRIX4X4 cameraRotY = XMMatrixRotationY(cameraYRotation);
		VECTOR3 moveDirection = XMVector3TransformNormal(inputDirection, cameraRotY);
		moveDirection = XMVector3Normalize(moveDirection) * MOVE_SPEED* sqrt(ix * ix + iy * iy); // 傾き具合に応じた速度

		// 移動の適用
		transform.position += moveDirection * moveSpeed;

		// プレイヤーの回転をカメラ基準で方向を向かせる
		transform.rotation.y = cameraYRotation + atan2(ix, -iy); // カメラの回転に対してスティックの方向に合わせる

	}

	if (di->CheckJoy(KD_TRG, 2, playerNum) && jumpCount <= jumpNum) {
		speedY = JUMP_POWER;
		if (jumpCount % 2 == 0) {
			animator->MergePlay(aJump);
		}
		else {
			animator->MergePlay(aJump2);
		}
		animator->SetPlaySpeed(1.0f);
		jumpCount++;
	}

	if (st->IsLandBlock(transform.position)) {
		// ジャンプ終了
		state = sOnGround;
		jumpCount = 0;
	}
}

void Player::UpdateAttack1()
{	
	// ゴミに攻撃を当てる
	std::list<Dust*> dusts = ObjectManager::FindGameObjects<Dust>();
	
	if (!finishAtkAnim && animator->CurrentFrame() <= 20.0f/atkSpeed) { //攻撃のヒットしたタイミング
		finishAtkAnim = true;
		for (Dust* d : dusts) {
			SphereCollider dCol = d->Collider(d->GetNum()); //ゴミの判定球
			SphereCollider atkCol = Collider();			//攻撃判定の球
			VECTOR3 forward = VECTOR3(0, 0, 1.0f); // 回転してない時の移動量
			MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Yの回転行列
			forward = forward * rotY; // キャラの向いてる方への移動量
			atkCol.center = transform.position + forward; //攻撃判定の球を作る
			atkCol.radius = 1.0f;
			VECTOR3 pushVec = atkCol.center - dCol.center;
			float rSum = atkCol.radius + dCol.radius;
			if (pushVec.LengthSquare() < rSum * rSum) { // 球の当たり判定
				// 当たってる
				d->AddDamage(this,1); //ダメージを与える
			}
		}
	}
	
	if (animator->Finished())
	{
		//攻撃アニメーションの終了
		finishAtkAnim = false;
		animator->SetPlaySpeed(1.0f);
		transform.rotation.y -= 15 * DegToRad; //回転を戻す
		state = sOnGround;
	}

}

void Player::UpdateAttack2()
{
	/*
	// ゴミに攻撃を当てる
	std::list<Dust*> dusts = ObjectManager::FindGameObjects<Dust>();
	VECTOR3 tipPos = VECTOR3(0.9966, 0.6536, 0.140);
	MATRIX4X4 mat = transform.matrix();// 世界(ワールド)の中で、プレイヤーの位置と向き
	MATRIX4X4 bone = mesh->GetFrameMatrices(34); // プレイヤーの原点からの手首の位置(34は手首)
	VECTOR3 start = VECTOR3(0, 0, 0) * bone * mat;
	VECTOR3 end = tipPos * bone * mat;

	for (Dust* d : dusts)
	{
		if (d->HitLineToMesh(start, end)) {
			d->AddDamage(1); //ゴミに当てた
		}
		if (animator->Finished())
		{
			//攻撃アニメーションの終了
			state = sOnGround;
		}
	}*/

	// ゴミに攻撃を当てる
	std::list<Dust*> dusts = ObjectManager::FindGameObjects<Dust>();

	if (!finishAtkAnim && animator->CurrentFrame() <= 20.0f/atkSpeed) { //攻撃のヒットしたタイミング
		finishAtkAnim = true;
		for (Dust* d : dusts) {
			SphereCollider dCol = d->Collider(d->GetNum()); //ゴミの判定球
			SphereCollider atkCol = Collider();			//攻撃判定の球
			VECTOR3 forward = VECTOR3(0, 0, 1.0f); // 回転してない時の移動量
			MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Yの回転行列
			forward = forward * rotY; // キャラの向いてる方への移動量
			atkCol.center = transform.position + forward; //攻撃判定の球を作る
			atkCol.radius = 1.0f;
			VECTOR3 pushVec = atkCol.center - dCol.center;
			float rSum = atkCol.radius + dCol.radius;
			if (pushVec.LengthSquare() < rSum * rSum) { // 球の当たり判定
				// 当たってる
				d->AddDamage(this,1); //ダメージを与える
			}
		}
	}

	if (animator->Finished())
	{
		//攻撃アニメーションの終了
		finishAtkAnim = false;
		animator->SetPlaySpeed(1.0f);
		state = sOnGround;
	}
}

void Player::UpdateAttack3()
{
}

//　プレイヤーの持つ箒
Broom::Broom(Object3D* parentModel, int num)
{
	parent = parentModel;

	mesh = new CFbxMesh();

	std::string f = "Data/Player/Color" + std::to_string(num) + "/weapon";
	mesh->Load((f + "/broom.mesh").c_str());

	transform.position = VECTOR3(0, 0, 0);
	transform.rotation = VECTOR3(0, 0, -70);
}

Broom::~Broom()
{
}

void Broom::Update()
{
	Player* pl = dynamic_cast<Player*>(parent);

	// 状態ごとの角度変更
	switch (pl->GetPlayerState()) {
	case sOnGround:
		if (!pl->GetIsFly()) {
			transform.position = VECTOR3(0, 0, 0);
			transform.rotation = VECTOR3(0, 0, -70);
		}
		else {
			transform.position = VECTOR3(7*0.01, -10*0.01, -35*0.01);
			transform.rotation = VECTOR3(77, 0, 90);
		}
		break;

	case sJump:
		transform.position = VECTOR3(0, 0, 0);
		transform.rotation = VECTOR3(0, 0, -70);
		break;
	case sAttack1:
	case sAttack2:
		transform.position = VECTOR3(0, 0, 0);
		transform.rotation = VECTOR3(0, 0, 0);
		break;
	}
}

void Broom::Draw()
{
	mesh->Render(transform.matrix() * mat * parent->Matrix());
}

void Broom::SetPos(MATRIX4X4 m)
{
	mat = m;
}
