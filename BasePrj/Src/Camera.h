#pragma once
#include "Object3D.h"
#include "SplitScreen.h"

using namespace std;

namespace {
	static const int MAXPLAYER = 4;
}

class Camera : public Object3D {
public:
	Camera();
	~Camera();
	void Update() override;
	void Draw() override;
	float GetRotY(int cameraNum) { return rotationY[cameraNum]; };

	MATRIX4X4 View(int counter) { return view[counter]; }
	VECTOR3 EyePt(int counter) { return eyePt[counter]; }
	VECTOR3 LookatPt(int counter) { return lookatPt[counter]; }

private:
	void updateCamera(int counter, VECTOR3 pos, VECTOR3 rot);

	SplitScreen* ssObj;

	VECTOR3 lookPosition;

	int viewType;			// 視点切り替えの番号
	float rotationY[MAXPLAYER];
	
	float changeTime[MAXPLAYER];		// 視点切り替え時間を数える
	VECTOR3 changePosStart[MAXPLAYER]; // 視点移動の開始点
	VECTOR3 changePosGoal[MAXPLAYER];	// 視点移動の終了点
	VECTOR3 changeLookStart[MAXPLAYER];// 注視点移動の開始点
	VECTOR3 changeLookGoal[MAXPLAYER];	// 注視点移動の終了点

	VECTOR3 prevPlayerPos[MAXPLAYER];	// プレイヤーの前回位置
	float autoRotateSpeed;	// カメラの自動回転速度

	vector<MATRIX4X4>  view;
	vector<VECTOR3>  eyePt;
	vector<VECTOR3>  lookatPt;
};