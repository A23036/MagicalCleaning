#include "Player.h"
#include "../Libs/Imgui/imgui.h"
#include "Stage.h"
#include "Camera.h"
#include "Dust.h"
#include <dinput.h>

namespace { // このcpp以外では使えない
	static const float Gravity = 0.005f; // 重力加速度(正の値)
	static const float JumpPower = 0.1f;
	static const float RotationSpeed = 3.0f; // 回転速度(度)
	static const float MoveSpeed = 0.1f;
};

Player::Player()
{
	animator = new Animator(); // インスタンスを作成

	mesh = new CFbxMesh();
	//ToDo: モデル変更
	//mesh->Load("Data/Player/witch.mesh");
	//mesh->LoadAnimation(aIdle, "Data/Player/witch_idle.anmx", true);
	mesh->Load("Data/Player/PlayerChara.mesh");
	mesh->LoadAnimation(aIdle, "Data/Player/Idle.anmx", true);
	mesh->LoadAnimation(aRun, "Data/Player/Run.anmx", true);
	mesh->LoadAnimation(aAttack1, "Data/Player/attack1.anmx", false);
	mesh->LoadAnimation(aAttack2, "Data/Player/attack2.anmx", false);
	mesh->LoadAnimation(aAttack3, "Data/Player/attack3.anmx", false);

	animator->SetModel(mesh); // このモデルでアニメーションする
	animator->Play(aIdle);
	animator->SetPlaySpeed(1.0f);

	transform.position = VECTOR3(0, 0, 0);
	transform.rotation = VECTOR3(0, 0, 0);
	state = sOnGround;
	speedY = 0;
	MP = 0;
	doneAtk = false;

	moveSpeed	= 1;
	jumpNum		= 1;
	atkSpeed	= 1;
	atkRange	= 1;
	carWeight	= 1;
}

Player::~Player()
{
	if (mesh != nullptr) {
		delete mesh;
		mesh = nullptr;
	}
	if (animator != nullptr) {
		delete animator;
		animator = nullptr;
	}
}

void Player::Update()
{
	animator->Update(); // 毎フレーム、Updateを呼ぶ

	switch (state) {
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
	}

	//当たり判定処理
	
	Stage* st = ObjectManager::FindGameObject<Stage>();
	VECTOR3 push;
	if (st->HitSphere(Collider(), &push)) {
		transform.position += push;
	}

	// ImGuiウィンドウの位置とサイズを設定
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(100, 60));
	ImGui::Begin("State");
	switch (state) {
	case 0:
		ImGui::Text("sOnGround");
		break;
	case 1:
		ImGui::Text("sJump");
		break;
	case 2:
		ImGui::Text("sAttack");
		break;
	}
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(100, 0));
	ImGui::SetNextWindowSize(ImVec2(180, 60));
	ImGui::Begin("JumpSpeed");
	ImGui::InputFloat("speedY", &speedY);
	ImGui::End();
	
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
	DIJOYSTATE2 joyState = pdi->GetJoyState();
	for (int i = 0; i < 32; i++)
	{
		// 各ボタンの押下状態を確認
		if (joyState.rgbButtons[i] & 0x80) // ボタン i が押されている場合
		{
			ImGui::Text("Button %d is pressed", i); // ボタン番号を表示
		}
	}
	float rx = di->GetJoyState().lRx;
	float ry = di->GetJoyState().lRy;
	float rz = di->GetJoyState().lRz;
	ImGui::InputFloat("rX", &rx);
	ImGui::InputFloat("rY", &ry);
	ImGui::InputFloat("rZ", &rz);
	ImGui::End();

	// Dustにめり込まないようにする
	// 自分の座標は、transform.position
	// Dustの座標を知る
	std::list<Dust*> dusts =ObjectManager::FindGameObjects<Dust>();
	
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
	

	// Dancerにめり込まないようにする
	// 自分の座標は、transform.position
	// Dancerの座標を知る
	//std::list<Dancer*> dancers =
	//	ObjectManager::FindGameObjects<Dancer>();
	// ①イテレーター版
	//for (std::list<Dancer*>::iterator itr = dancers.begin();
	//				itr != dancers.end(); itr++) {
	//	Dancer* dancer = *itr;
	// ②イテレータの変更
	//for (auto itr = dancers.begin(); itr != dancers.end(); itr++) {
	//	Dancer* dancer = *itr;
	// ③for(:)で回す
	/*
	for (Dancer* dancer : dancers) {
		// 以上３バージョン
		SphereCollider tCol = dancer->Collider();
		SphereCollider pCol = Collider();
		VECTOR3 pushVec = pCol.center - tCol.center;
		float rSum = pCol.radius + tCol.radius;
		if (pushVec.LengthSquare() < rSum * rSum) { // 球の当たり判定
			// 当たってる
			// 押し出す方向はpushVec
			// 押し出す長さを求める
			float pushLen = rSum - pushVec.Length();
			pushVec = XMVector3Normalize(pushVec); // pushVecの長さを１にする
			transform.position += pushVec * pushLen;
		}
	}
	*/
}
	
void Player::Draw()
{
	Object3D::Draw(); // 継承元の関数を呼ぶ

	//MATRIX4X4 tip = XMMatrixRotationRollPitchYawFromVector(VECTOR3(-33, 82, 0) * DegToRad);
	//VECTOR3 tipPos = VECTOR3(0, 0, 1.2f) * tip;
	VECTOR3 tipPos = VECTOR3(0.9966, 0.6536, 0.140);
	MATRIX4X4 mat = transform.matrix();// 世界(ワールド)の中で、プレイヤーの位置と向き
	MATRIX4X4 bone = mesh->GetFrameMatrices(34); // プレイヤーの原点からの手首の位置(34は手首)
	VECTOR3 start = VECTOR3(0, 0, 0) * bone * mat;
	VECTOR3 end = tipPos * bone * mat;

	CSprite spr;
	spr.DrawLine3D(start, end, RGB(255, 0, 0));
}

SphereCollider Player::Collider()
{
	SphereCollider col;
	col.center = transform.position + VECTOR3(0, 0.6f, 0);
	col.radius = 0.55f;
	return col;
}

void Player::addMP(int n)
{
	MP += n;
}


void Player::UpdateOnGround()
{
	auto di = GameDevice()->m_pDI;
	//int ix = di->GetJoyState().lX;	// 右:1000 / 左:-1000
	//int iy = di->GetJoyState().lY;	// 下:1000 / 上:-1000

	//ImGui::SetNextWindowPos(ImVec2(0, 60));
	//ImGui::SetNextWindowSize(ImVec2(100, 100));
	//ImGui::Begin("Joystick");
	//ImGui::InputInt("IX", &ix);
	//ImGui::InputInt("IY", &iy);
	//ImGui::End();

	Stage* st = ObjectManager::FindGameObject<Stage>();
	if (!(st->IsLandBlock(transform.position))) {
		// 空中
		state = sJump;
		return; //この下の処理は行わない
	}
	 
	// 2024.10.26 プレイヤー操作をコントローラーに対応↓
	
	// コントローラースティックの入力を取得
	// 正規化して -1.0～1.0 の範囲にする
	float ix = di->GetJoyState().lX / 1000.0f; 
	float iy = di->GetJoyState().lY / 1000.0f;
	
	// スティックが入力されているか確認
	if (fabs(ix) > 0.1f || fabs(iy) > 0.1f) {
		// 入力に基づいて移動方向を設定
		VECTOR3 inputDirection = VECTOR3(ix, 0, -iy); // Z軸方向は反転

		// カメラのY軸回転を取得
		Camera* camera = ObjectManager::FindGameObject<Camera>();
		float cameraYRotation = camera->GetRotY(0);

		// カメラの回転に基づく移動ベクトルの計算
		MATRIX4X4 cameraRotY = XMMatrixRotationY(cameraYRotation);
		VECTOR3 moveDirection = XMVector3TransformNormal(inputDirection, cameraRotY);
		moveDirection = XMVector3Normalize(moveDirection) * MoveSpeed * sqrt(ix * ix + iy * iy); // 傾き具合に応じた速度

		// 移動の適用
		transform.position += moveDirection;

		// プレイヤーの回転をカメラ基準で方向を向かせる
		transform.rotation.y = cameraYRotation + atan2(ix, -iy); // カメラの回転に対してスティックの方向に合わせる

		// 走行アニメーションを再生
		animator->MergePlay(aRun);

		//走行速度に応じたアニメーションスピードを設定
		animator->SetPlaySpeed(sqrt(ix * ix + iy * iy));
	}
	else {
		// スティックが傾いていない場合は待機アニメーションを再生
		animator->MergePlay(aIdle);
	}
	// 2024.10.26 プレイヤー操作をコントローラーに対応↑

	if (di->CheckKey(KD_TRG, DIK_SPACE) || di->CheckJoy(KD_TRG, DIJ_A)) {
		speedY = JumpPower;
		state = sJump;
	}
	if (di->CheckKey(KD_TRG, DIK_N) || di->CheckJoy(KD_TRG, DIJ_X)) { //攻撃ボタン
		animator->MergePlay(aAttack1);
		animator->SetPlaySpeed(2.5);
		state = sAttack1;
	}
	if (di->CheckKey(KD_TRG, DIK_M)) { //攻撃ボタン
		animator->MergePlay(aAttack2);
		state = sAttack2;
	}

	
}


void Player::UpdateJump()
{
	Stage* st = ObjectManager::FindGameObject<Stage>();
	transform.position.y += speedY;
	speedY -= Gravity;	// 重力

	auto di = GameDevice()->m_pDI;
	// コントローラースティックの入力を取得
	// 正規化して -1.0～1.0 の範囲にする
	float ix = di->GetJoyState().lX / 1000.0f;
	float iy = di->GetJoyState().lY / 1000.0f;

	// スティックが入力されているか確認
	if (fabs(ix) > 0.1f || fabs(iy) > 0.1f) {
		// 入力に基づいて移動方向を設定
		VECTOR3 inputDirection = VECTOR3(ix, 0, -iy); // Z軸方向は反転

		// カメラのY軸回転を取得
		Camera* camera = ObjectManager::FindGameObject<Camera>();
		float cameraYRotation = camera->GetRotY(0);

		// カメラの回転に基づく移動ベクトルの計算
		MATRIX4X4 cameraRotY = XMMatrixRotationY(cameraYRotation);
		VECTOR3 moveDirection = XMVector3TransformNormal(inputDirection, cameraRotY);
		moveDirection = XMVector3Normalize(moveDirection) * MoveSpeed * sqrt(ix * ix + iy * iy); // 傾き具合に応じた速度

		// 移動の適用
		transform.position += moveDirection;

		// プレイヤーの回転をカメラ基準で方向を向かせる
		transform.rotation.y = cameraYRotation + atan2(ix, -iy); // カメラの回転に対してスティックの方向に合わせる

		// 走行アニメーションを再生
		animator->MergePlay(aRun);

		//走行速度に応じたアニメーションスピードを設定
		animator->SetPlaySpeed(sqrt(ix * ix + iy * iy));
	}
	else {
		// スティックが傾いていない場合は待機アニメーションを再生
		animator->MergePlay(aIdle);
	}

	if (st->IsLandBlock(transform.position)) {
		// ジャンプ終了
		state = sOnGround;
	}
}

void Player::UpdateAttack1()
{	
	// ゴミに攻撃を当てる
	std::list<Dust*> dusts = ObjectManager::FindGameObjects<Dust>();
	/*
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
	
	if (!doneAtk && animator->CurrentFrame() == 40) { //攻撃のヒットしたタイミング
		doneAtk = true;
		for (Dust* d : dusts) {
			SphereCollider dCol = d->Collider(d->GetNum()); //ゴミの判定球
			SphereCollider atkCol = Collider();			//攻撃判定の球
			VECTOR3 forward = VECTOR3(0, 0.5f, 1.0f); // 回転してない時の移動量
			MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Yの回転行列
			forward = forward * rotY; // キャラの向いてる方への移動量
			atkCol.center = transform.position + forward; //攻撃判定の球を作る
			atkCol.radius = 1.0f;
			VECTOR3 pushVec = atkCol.center - dCol.center;
			float rSum = atkCol.radius + dCol.radius;
			if (pushVec.LengthSquare() < rSum * rSum) { // 球の当たり判定
				// 当たってる
				d->AddDamage(1); //ダメージを与える
			}
		}
	}
	
	if (animator->Finished())
	{
		//攻撃アニメーションの終了
		doneAtk = false;
		animator->SetPlaySpeed(1.0);
		state = sOnGround;
	}

}

void Player::UpdateAttack2()
{
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
	}
}

void Player::UpdateAttack3()
{
}
