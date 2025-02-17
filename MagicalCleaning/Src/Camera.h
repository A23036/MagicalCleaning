#pragma once
#include "Object3D.h"

class CsvReader;
class SplitScreen;
class DataCarrier;
class Stage;
class Player;

using namespace std;

class Camera : public Object3D {
public:
	Camera();
	~Camera();
	void Start() override;
	void Update() override;
	void Draw() override;
	
	float GetRotY(int cameraNum) { return rotationY[cameraNum]; };

	void CsvLoad(); //CSVファイルからデータの設定

	MATRIX4X4 View(int counter) { return view[counter]; }
	VECTOR3 EyePt(int counter) { return eyePt[counter]; }
	VECTOR3 LookatPt(int counter) { return lookatPt[counter]; }

private:
	void updateCamera(int counter, VECTOR3 pos, VECTOR3 rot);

	CsvReader* csv;
	DataCarrier* dc;
	Stage* st;

	VECTOR3 posOld;

	VECTOR3 CameraPos = VECTOR3(0, 0, 0);	// カメラ位置
	VECTOR3 LookPos = VECTOR3(0, 1, 0);		// カメラ注視点

	Player* player[MAXPLAYER];

	float CHANGE_TIME;		// カメラ移動時間
	float ROTATE_SPEED;		// カメラの自動回転速度

	SplitScreen* ssObj;

	VECTOR3 lookPosition;

	float rotationY[MAXPLAYER];
	
	float changeTime[MAXPLAYER];		// 視点切り替え時間を数える
	VECTOR3 changePosStart[MAXPLAYER];	// 視点移動の開始点
	VECTOR3 changePosGoal[MAXPLAYER];	// 視点移動の終了点
	VECTOR3 changeLookStart[MAXPLAYER];	// 注視点移動の開始点
	VECTOR3 changeLookGoal[MAXPLAYER];	// 注視点移動の終了点

	VECTOR3 prevPlayerPos[MAXPLAYER];	// プレイヤーの前回位置

	vector<MATRIX4X4>  view;
	vector<VECTOR3>  eyePt;
	vector<VECTOR3>  lookatPt;
};