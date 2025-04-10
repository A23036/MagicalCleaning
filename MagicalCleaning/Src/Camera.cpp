#include "Camera.h"
#include "Player.h"
#include "CsvReader.h"
#include "SplitScreen.h"
#include "DataCarrier.h"
#include "Stage.h"

// ---------------------------------------------------------------------------
// コンストラクタ
// ---------------------------------------------------------------------------
Camera::Camera()
{
	CsvLoad(); // csvからデータの設定

	ObjectManager::SetVisible(this, false);	// 自体は表示しない
	SetDrawOrder(-1000);

	// 配列要素の初期化
	for (int i = 0; i < MAXPLAYER; i++) {
		rotationY[i] = 0.0f + i*90*DegToRad;
		prevPlayerPos[i] = VECTOR3(0, 0, 0);
		changeTime[i] = CHANGE_TIME;
		changePosStart[i]	= VECTOR3(0, 0, 0);
		changePosGoal[i]	= VECTOR3(0, 0, 0);
		changeLookStart[i]	= VECTOR3(0, 0, 0);
		changeLookGoal[i]	= VECTOR3(0, 0, 0);
	}
}

// ---------------------------------------------------------------------------
// デストラクタ
// ---------------------------------------------------------------------------
Camera::~Camera()
{
	SAFE_DELETE(csv);
}

// ---------------------------------------------------------------------------
// 開始時に呼ばれる関数
// 
// オブジェクトを生成後、最初にUpdate()の前に呼ばれる
// ---------------------------------------------------------------------------
void Camera::Start()
{
	//プレイヤーオブジェクトの格納
	for (int i = 0; i < MAXPLAYER; i++) {
		string s = "Player" + to_string(i+1);
		player[i] = ObjectManager::FindGameObjectWithTag<Player>(s);
	}

	st = ObjectManager::FindGameObject<Stage>();
	dc = ObjectManager::FindGameObject<DataCarrier>();
	ssObj = ObjectManager::FindGameObject<SplitScreen>();

	//各カメラ設定の初期化
	while (view.size() < ssObj->MultiSize())
	{
		MATRIX4X4 m = XMMatrixIdentity();
		view.emplace_back(m);
		VECTOR3 v = VECTOR3(0, 0, 0);
		eyePt.emplace_back(v);
		lookatPt.emplace_back(v);
	}
}

// ---------------------------------------------------------------------------
// カメラの更新処理
// 
// 画面数に応じたカメラの更新処理を行う
// ---------------------------------------------------------------------------
void Camera::Update()
{
	if (ssObj->Multi()) // 多画面のとき
	{
		for (int i = 0; i < ssObj->MultiSize(); i++)
		{
			// ４画面のとき
			switch (i)
			{
			case 0:
				{
					// 画面0はPlayer1視点
					if (player[i] != nullptr)
					{
						updateCamera(i, player[0]->Position(), player[0]->Rotation());
					}
					else {
						updateCamera(i, VECTOR3(0,0,0), VECTOR3(0,0,0));
					}
				}
				break;

			case 1:
				{
					// 画面1はPlayer2視点
					if (player[i] != nullptr)
					{
						updateCamera(i, player[i]->Position(), player[i]->Rotation());
					}
					else {
						updateCamera(i, VECTOR3(0, 0, 0), VECTOR3(0, 0, 0));
					}
				}
				break;

			case 2:
				{
					// 画面2はPlayer3視点
					if (player[i] != nullptr)
					{
						updateCamera(i, player[i]->Position(), player[i]->Rotation());
					}
					else {
						updateCamera(i, VECTOR3(0, 0, 0), VECTOR3(0, 0, 0));
					}
				}
				break;

			case 3:
				{
					// 画面3はPlayer4視点
					if (player[i] != nullptr)
					{
						updateCamera(i, player[i]->Position(), player[i]->Rotation());
					}
					else {
						updateCamera(i, VECTOR3(0, 0, 0), VECTOR3(0, 0, 0));
					}
				}
				break;
			}
		}
	}
	else {
		// １画面のときPlayer視点
		if (player[0] != nullptr)
		{
			updateCamera(0, player[0]->Position(), player[0]->Rotation());
		}
		else {
			updateCamera(0, VECTOR3(0, 0, 0), VECTOR3(0, 0, 0));
		}
	}
}

// ---------------------------------------------------------------------------
// カメラの描画処理
// ---------------------------------------------------------------------------
void Camera::Draw()
{
	GameDevice()->m_mView = XMMatrixLookAtLH(
		transform.position, //カメラ座標
		lookPosition,	//注視点
		VECTOR3(0, 1, 0));
}

// ---------------------------------------------------------------------------
// CSV読み込み処理
// 
// カメラ処理に使用する定数のCSV読み込みを行う
// ---------------------------------------------------------------------------
void Camera::CsvLoad()
{
	// csvからデータ読み込み
	CsvCameraDataLoader dataLoader("data/csv/Paramater.csv");
	dataLoader.Load();
	cameraParams = dataLoader.GetCameraParams();

	//読み込んだ構造体からデータの代入
	CameraPos = cameraParams.CameraPos;
	LookPos = cameraParams.LookPos;
	CHANGE_TIME = cameraParams.CHANGE_TIME;
	ROTATE_SPEED = cameraParams.ROTATE_SPEED;
}

// ---------------------------------------------------------------------------
// カメラの更新処理サブ関数
// 
// 各プレイヤーカメラの更新処理を行う
// ---------------------------------------------------------------------------
void Camera::updateCamera(int counter, VECTOR3 pos, VECTOR3 rot)
{
	//プレイ中でないときカメラ更新処理を行わない
	if (!dc->GetIsPlay() && dc->GetGameState() != 0) {
		return;
	}

	auto di = GameDevice()->m_pDI;
	
	// プレイヤーの位置を取得
	VECTOR3 playerPos = pos;

	//回転マトリクスの取得
	MATRIX4X4 rotY = XMMatrixRotationY(rotationY[counter]);
	MATRIX4X4 trans = XMMatrixTranslation(pos.x, pos.y, pos.z);
	MATRIX4X4 m = rotY * trans;

	//カメラの手動回転処理
	if (dc->GetCameraSetting(counter) == 1) {
		float z = di->GetJoyState(counter).lZ;	// 左:0 / 右:65535

		// 入力値を正規化 (-1.0f ~ 1.0f)
		float normalizedZ = (z / 32767.5f) - 1.0f;

		// デッドゾーンの設定
		const float deadZone = 0.1f;
		if (fabs(normalizedZ) > deadZone) {
			// カメラのY軸回転処理
			rotationY[counter] += normalizedZ * ROTATE_SPEED;
		}
	}
	else {
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
		}
		//2024.10.25 カメラ自動回転処理↑
	}
	
	//MP変換中のカメラ操作
	if (player[counter]->GetPlayerState() == sCharge) {

		//左スティックの左右入力
		// 入力値を正規化 (-1.0f ~ 1.0f)
		float ix = di->GetJoyState(counter).lX / 1000.0f; 
		
		// デッドゾーンの設定
		const float deadZone = 0.1f;
		if (fabs(ix) > deadZone) {
			// カメラのY軸回転処理
			rotationY[counter] += ix * ROTATE_SPEED;
		}
	}

	// 2024.10.12 カメラ回転リセット↓
	// 左SHIFTキーが押されたらカメラをプレイヤーの後方に移動
	if (di->CheckKey(KD_TRG, DIK_LSHIFT)&&counter == 0 || di->CheckJoy(KD_TRG, 6,counter)) {
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
		float rate = 1 - pow(1 - timeRate, 3); //滑らかな視点移動
		VECTOR3 position = (changePosGoal[counter] * m - changePosStart[counter]) * rate + changePosStart[counter];
		VECTOR3 look = (changeLookGoal[counter] * m - changeLookStart[counter]) * rate + changeLookStart[counter];
		transform.position = position;
		lookPosition = look;
	}

	// カメラが壁にめり込まないようにする
	VECTOR3 start = pos;
	//start.y = CameraPos.y;
	VECTOR3 end = transform.position;
	VECTOR3 hit;
	VECTOR3 normal;
	if (st->MapCol()->IsCollisionLay(start, end, hit, normal)) {
		VECTOR3 move = pos - transform.position;		// 前に出す方向のベクトルを作る
		VECTOR3 push = XMVector3Normalize(move) * 0.1;	// そのベクトルの長さを0.1にする
		hit += push;
		end.x = hit.x;
		end.z = hit.z;
	}
	transform.position = end;

	// プレイヤーの位置を保存
	prevPlayerPos[counter] = playerPos;
	
	// カメラ座標を配列に設定する
	eyePt[counter] = transform.position;   // カメラ座標
	lookatPt[counter] = lookPosition;      // 注視点
	view[counter] = XMMatrixLookAtLH(transform.position,lookPosition,VECTOR3(0, 1, 0));	// ビューマトリックス

	// ------------------------------------------------------------------
	// 視点からの距離の２乗をDrawObjectに設定する
	// これは、視点からの距離の降順に描画したいため
	// ------------------------------------------------------------------
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
