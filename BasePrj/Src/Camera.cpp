#include "Camera.h"
#include "Player.h"
#include "CsvReader.h"
#include <iostream>

Camera::Camera()
{
	CsvLoad(); // csvからデータの設定

	ObjectManager::SetVisible(this, false);		// 自体は表示しない
	SetDrawOrder(-1000);

	ssObj = ObjectManager::FindGameObject<SplitScreen>();
	
	while (view.size() < ssObj->MultiSize())
	{
		MATRIX4X4 m = XMMatrixIdentity();
		view.emplace_back(m);                                                                         
		VECTOR3 v = VECTOR3(0, 0, 0);
		eyePt.emplace_back(v);
		lookatPt.emplace_back(v);
	}

	// 配列要素の初期化
	for (int i = 0; i < MAXPLAYER; i++) {
		rotationY[i] = 0.0f;
		prevPlayerPos[i] = VECTOR3(0, 0, 0);
		changeTime[i] = CHANGE_TIME;
		changePosStart[i]	= VECTOR3(0, 0, 0);
		changePosGoal[i]	= VECTOR3(0, 0, 0);
		changeLookStart[i]	= VECTOR3(0, 0, 0);
		changeLookGoal[i]	= VECTOR3(0, 0, 0);
	}
}

Camera::~Camera()
{
}


void Camera::Update()
{

	if (ssObj->Multi())
	{
		// 多画面のとき
		for (int i = 0; i < ssObj->MultiSize(); i++)
		{
			/*
			// ２画面のとき
			switch (i)
			{
			case 0:
			{
				// 画面0はPlayer視点
				Player* pc = ObjectManager::FindGameObject<Player>();
				if (pc != nullptr)
				{
					updateCamera(i, pc->Position(), pc->Rotation());
				}
				else {
					updateCamera(i, VECTOR3(0, 0, 0), VECTOR3(0, 0, 0));
				}
			}
			break;

			case 1:
			{
				// 画面1はEnemyRS視点
				EnemyRS* enm = ObjectManager::FindGameObject<EnemyRS>();
				if (enm != nullptr && enm->Mesh() != nullptr)
				{
					updateCamera(i, enm->Position(), enm->Rotation());
				}
				else {
					updateCamera(i, VECTOR3(0, 0, 0), VECTOR3(0, 0, 0));
				}
			}
			break;
			}
			*/
			///*
			// ４画面のとき
			switch (i)
			{
			case 0:
				{
					// 画面0はPlayer1視点
					Player* pc = ObjectManager::FindGameObjectWithTag<Player>("Player1");
					if (pc != nullptr)
					{
						updateCamera(i, pc->Position(), pc->Rotation());
					}
					else {
						updateCamera(i, VECTOR3(0,0,0), VECTOR3(0,0,0));
					}
				}
				break;

			case 1:
				{
					// 画面1はPlayer2視点
					Player* pc = ObjectManager::FindGameObjectWithTag<Player>("Player2");
					if (pc != nullptr)
					{
						updateCamera(i, pc->Position(), pc->Rotation());
					}
					else {
						updateCamera(i, VECTOR3(0, 0, 0), VECTOR3(0, 0, 0));
					}
				}
				break;

			case 2:
				{
					// 画面2はPlayer3視点
					Player* pc = ObjectManager::FindGameObjectWithTag<Player>("Player3");
					if (pc != nullptr)
					{
						updateCamera(i, pc->Position(), pc->Rotation());
					}
					else {
						updateCamera(i, VECTOR3(0, 0, 0), VECTOR3(0, 0, 0));
					}
				}
				break;

			case 3:
				{
					// 画面3はPlayer4視点
					Player* pc = ObjectManager::FindGameObjectWithTag<Player>("Player4");
					if (pc != nullptr)
					{
						updateCamera(i, pc->Position(), pc->Rotation());
					}
					else {
						updateCamera(i, VECTOR3(0, 0, 0), VECTOR3(0, 0, 0));
					}
				}
				break;
			}
			//*/
		}
	}
	else {
		// １画面のときPlayer視点
		Player* pc = ObjectManager::FindGameObject<Player>();
		if (pc != nullptr)
		{
			updateCamera(0, pc->Position(), pc->Rotation());
		}
		else {
			updateCamera(0, VECTOR3(0, 0, 0), VECTOR3(0, 0, 0));
		}
	}
}

void Camera::Draw()
{
	GameDevice()->m_mView = XMMatrixLookAtLH(
		transform.position, // カメラ座標
		lookPosition,	//注視点
		VECTOR3(0, 1, 0));
}

void Camera::CsvLoad()
{
	// csvからデータ読み込み
	csv = new CsvReader("data/csv/Paramater.csv");
	if (csv->GetLines() < 1) {
		MessageBox(NULL, "Paramater.csvが読めません", "エラー", MB_OK);
	}

	for (int i = 1; i < csv->GetLines(); i++) { //CSVファイルから設定の読み込み
		if (csv->GetString(i, 0) == "Camera") {
			if (csv->GetString(i, 1) == "CameraPos") {		// カメラ座標
				CameraPos = VECTOR3( csv->GetFloat(i, 3), csv->GetFloat(i, 4), csv->GetFloat(i, 5) );
			}
			if (csv->GetString(i, 1) == "LookPos") {		// カメラ注視点
				LookPos = VECTOR3( csv->GetFloat(i, 3), csv->GetFloat(i, 4), csv->GetFloat(i, 5) );
			}
			if (csv->GetString(i, 1) == "ChangeTime") {		// カメラ切り替え時間
				CHANGE_TIME = csv->GetFloat(i, 3);
			}
			if (csv->GetString(i, 1) == "RotateSpeed") {	// カメラ回転速度
				ROTATE_SPEED = csv->GetFloat(i, 3);
			}
		}
		if (csv->GetString(i, 0) == "Player") {
			break;
		}
	}
}


void Camera::updateCamera(int counter, VECTOR3 pos, VECTOR3 rot)
{
	// プレイヤーの位置を取得
	VECTOR3 playerPos = pos;

	MATRIX4X4 rotY = XMMatrixRotationY(rotationY[counter]);
	MATRIX4X4 trans = XMMatrixTranslation(pos.x, pos.y, pos.z);
	MATRIX4X4 m = rotY * trans;

	// 2024.10.12 カメラ回転リセット↓

	// 左SHIFTキーが押されたらカメラをプレイヤーの後方に移動
	auto di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_TRG, DIK_LSHIFT) || di->CheckJoy(KD_TRG, 6,counter)) {
		//プレイヤーの回転を保存
		rotationY[counter] = rot.y;

		changeTime[counter] = 0.0f;

		changePosStart[counter] = eyePt[counter];
		changeLookStart[counter] = lookatPt[counter];
		changePosGoal[counter] = CameraPos;
		changeLookGoal[counter] = LookPos;
		
	}
	// 2024.10.12 カメラ回転リセット↑

	// プレイヤーが回転・移動してない時のカメラ位置に
	// プレイヤーの回転・移動行列を掛けると、
	if (changeTime[counter] >= CHANGE_TIME) {
		transform.position = CameraPos * m;
		lookPosition = LookPos * m;
	}
	else { // 視点切り替え中
		changeTime[counter] += 1.0f / 60.0f;
		float timeRate = changeTime[counter] / CHANGE_TIME; // 0,0～1.0
		//float rate = -((timeRate - 1) * (timeRate - 1)) + 1;
		float rate = 1 - pow(1 - timeRate, 3);
		VECTOR3 position = (changePosGoal[counter] * m - changePosStart[counter]) * rate + changePosStart[counter];
		VECTOR3 look = (changeLookGoal[counter] * m - changeLookStart[counter]) * rate + changeLookStart[counter];
		transform.position = position;
		lookPosition = look;
	}

	//2024.10.25 カメラ自動回転処理↓

	// プレイヤーの移動方向を計算
	VECTOR3 moveDir = playerPos - prevPlayerPos[counter];
	moveDir.y = 0.0f;	// 垂直方向の動きを無視
	moveDir = XMVector3Normalize(moveDir);

	if (prevPlayerPos[counter] != VECTOR3(0, 0, 0) && moveDir.Length() != 0)
	{
		MATRIX4X4 rotY = XMMatrixRotationY(rotationY[counter]);	// カメラの回転量
		VECTOR3 rightDir = VECTOR3(1, 0, 0) * rotY;		// カメラから見て右方向のベクトル

		float dotVal = dot(moveDir, rightDir); // 内積結果 0:正面 / ～1.0:右移動 / -1.0～:左移動 

		// プレイヤーの後ろにカメラを回り込ませる処理
		float targetRotationY = rotationY[counter];

		if (dotVal > 0.1) // プレイヤーがカメラの右側にいる
		{
			// 緩やかに右に回転させる
			targetRotationY += ROTATE_SPEED;
		}
		else if (dotVal < -0.1) // プレイヤーがカメラの左側にいる
		{
			// 緩やかに左に回転させる
			targetRotationY -= ROTATE_SPEED;
		}

		// プレイヤーが斜め前にいる場合は回転を少し抑える
		float forwardDot = dot(moveDir, VECTOR3(0, 0, 1) * rotY); // 正面方向との内積
		if (forwardDot > 0.0f) // プレイヤーがカメラの前にいる場合
		{
			float adjustment = 1.0f - forwardDot; // 前にいるほど回転を抑える
			targetRotationY = rotationY[counter] + (targetRotationY - rotationY[counter]) * adjustment;
		}

		// 回転量を更新
		rotationY[counter] = targetRotationY;

		/*
		ImGui::SetNextWindowPos(ImVec2(0, 160));
		ImGui::SetNextWindowSize(ImVec2(200, 60));
		ImGui::Begin("Dot");
		ImGui::InputFloat("dot", &dotVal);
		ImGui::End();
		*/
	}
	//2024.10.25 カメラ自動回転処理↑


	// カメラが壁にめり込まないようにする
	/*
	VECTOR3 start = pos;
	start.y = 1.0;
	VECTOR3 end = transform.position;
	std::list<Object3D*> objects = ObjectManager::FindGameObjects<Object3D>();
	for (auto g : objects) {
		VECTOR3 hit;
		if (g->HitLineToMesh(start, end, &hit)) {
			VECTOR3 move = pos - transform.position; // 前に出す方向のベクトルを作る
			VECTOR3 push = XMVector3Normalize(move) * 0.1; // そのベクトルの長さを0.1にする
			hit += push;
			end = hit;
		}
	}
	
	transform.position = end;
	*/
	// プレイヤーの位置を保存
	prevPlayerPos[counter] = playerPos;
	
	// ------------------------------------------------------------------
	// カメラ座標を配列に設定する
	eyePt[counter] = transform.position;   // カメラ座標
	lookatPt[counter] = lookPosition;      // 注視点
	view[counter] = XMMatrixLookAtLH(	    // ビューマトリックス
		transform.position,
		lookPosition,
		VECTOR3(0, 1, 0));

	// ------------------------------------------------------------------
	// 視点からの距離の２乗をDrawObjectに設定する
	// これは、視点からの距離の降順に描画したいため
	std::list<Object3D*> objList = ObjectManager::FindGameObjects<Object3D>();
	for (Object3D*& obj : objList)
	{
		if (obj != this)
		{
			float distQ = magnitudeSQ(obj->Position() - transform.position);
			ObjectManager::SetEyeDist(obj, distQ, counter);  // 視点からの距離の２乗をDrawObjectに設定
		}
	}
}
