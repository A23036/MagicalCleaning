#include "Camera.h"
#include "Player.h"
#include <iostream>
										//後方視点			//上方視点
static const VECTOR3 CameraPos[] = { VECTOR3(0, 3, -7), VECTOR3(0, 5, -0.5) };
static const VECTOR3 LookPos[] =   { VECTOR3(0, 1.5,  0), VECTOR3(0, 1,    1) };
static const float CHANGE_TIME_LIMIT = 0.5f; // 秒

Camera::Camera()
{
	SetDrawOrder(-1000);
	viewType = 0;
	changeTime = CHANGE_TIME_LIMIT; // 切り替えない
	rotationY = 0.0f;

	prevPlayerPos = VECTOR3(0, 0, 0);	// プレイヤーの前回位置
	autoRotateSpeed = 0.05f;			// カメラの自動回転速度
}

Camera::~Camera()
{
}


void Camera::Update()
{
	/* デバッグに使うかもしれないのでコメントアウト
	// 二つの視点を'L'キーによって切り替える
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_L)) {
		changePosStart = CameraPos[viewType];
		changeLookStart = LookPos[viewType];
		viewType += 1;
		if (viewType >= sizeof(CameraPos) / sizeof(CameraPos[0])) {
			viewType = 0;
		}
		changePosGoal = CameraPos[viewType];
		changeLookGoal = LookPos[viewType];
		changeTime = 0.0f;
	}*/

	// プレイヤーの行列を求める
	Player* player = ObjectManager::FindGameObject<Player>();

	// プレイヤーの位置を取得
	VECTOR3 playerPos = player->Position();

	MATRIX4X4 rotY = XMMatrixRotationY(rotationY);
	MATRIX4X4 trans = XMMatrixTranslation(player->Position().x, player->Position().y, player->Position().z);
	MATRIX4X4 m = rotY * trans;

	// 2024.10.12 カメラ回転リセット↓
	
	// 左SHIFTキーが押されたらカメラをプレイヤーの後方に移動
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_LSHIFT)) {
		//プレイヤーの回転を保存
		rotationY = player->Rotation().y;

		changeTime = 0.0f;

		changePosStart = transform.position;
		changeLookStart = lookPosition;
		changePosGoal = CameraPos[viewType];
		changeLookGoal = LookPos[viewType];
	}
	// 2024.10.12 カメラ回転リセット↑

	// プレイヤーが回転・移動してない時のカメラ位置に
	// プレイヤーの回転・移動行列を掛けると、
	if (changeTime >= CHANGE_TIME_LIMIT) {
		transform.position = CameraPos[viewType] * m;
		lookPosition = LookPos[viewType] * m;
	}
	else { // 視点切り替え中
		changeTime += 1.0f / 60.0f;
		float timeRate = changeTime / CHANGE_TIME_LIMIT; // 0,0～1.0
		//float rate = -((timeRate - 1) * (timeRate - 1)) + 1;
		float rate = 1 - pow(1 - timeRate, 3);
		VECTOR3 position = (changePosGoal * m - changePosStart) * rate + changePosStart;
		VECTOR3 look = (changeLookGoal * m - changeLookStart) * rate + changeLookStart;
		transform.position = position;
		lookPosition = look;
	}

	// カメラが壁にめり込まないようにする
	VECTOR3 start = player->Position();
	start.y = 1.0;
	VECTOR3 end = transform.position;
	std::list<Object3D*> objects = ObjectManager::FindGameObjects<Object3D>();
	for (auto g : objects) {
		VECTOR3 hit;
		if (g->HitLineToMesh(start, end, &hit)) {
			VECTOR3 move = player->Position() - transform.position; // 前に出す方向のベクトルを作る
			VECTOR3 push = XMVector3Normalize(move) * 0.1; // そのベクトルの長さを0.1にする
			hit += push;
			end = hit;
		}
	}
	transform.position = end;

	//2024.10.25 カメラ自動回転処理↓

	// プレイヤーの移動方向を計算
	VECTOR3 moveDir = playerPos - prevPlayerPos;
	moveDir.y = 0.0f;	// 垂直方向の動きを無視
	moveDir = XMVector3Normalize(moveDir);

	if (prevPlayerPos != VECTOR3(0, 0, 0) && moveDir.Length() != 0)
	{
		MATRIX4X4 rotY = XMMatrixRotationY(rotationY);	// カメラの回転量
		VECTOR3 rightDir = VECTOR3(1, 0, 0) * rotY;		// カメラから見て右方向のベクトル

		float dotVal = dot(moveDir, rightDir); // 内積結果 0:正面 / ～1.0:右移動 / -1.0～:左移動 

		// プレイヤーの後ろにカメラを回り込ませる処理
		float targetRotationY = rotationY;

		if (dotVal > 0.1) // プレイヤーがカメラの右側にいる
		{
			// 緩やかに右に回転させる
			targetRotationY += autoRotateSpeed;
		}
		else if (dotVal < -0.1) // プレイヤーがカメラの左側にいる
		{
			// 緩やかに左に回転させる
			targetRotationY -= autoRotateSpeed;
		}

		// プレイヤーが斜め前にいる場合は回転を少し抑える
		float forwardDot = dot(moveDir, VECTOR3(0, 0, 1) * rotY); // 正面方向との内積
		if (forwardDot > 0.0f) // プレイヤーがカメラの前にいる場合
		{
			float adjustment = 1.0f - forwardDot; // 前にいるほど回転を抑える
			targetRotationY = rotationY + (targetRotationY - rotationY) * adjustment;
		}

		// 回転量を更新
		rotationY = targetRotationY;

		ImGui::SetNextWindowPos(ImVec2(0, 160));
		ImGui::SetNextWindowSize(ImVec2(200, 60));
		ImGui::Begin("Dot");
		ImGui::InputFloat("dot", &dotVal);
		ImGui::End();
	}
	//2024.10.25 カメラ自動回転処理↑

	// プレイヤーの位置を保存
	prevPlayerPos = playerPos;
}

void Camera::Draw()
{
	GameDevice()->m_mView = XMMatrixLookAtLH(
		transform.position, // カメラ座標
		lookPosition,	//注視点
		VECTOR3(0, 1, 0));
}
