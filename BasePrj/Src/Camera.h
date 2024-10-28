#pragma once
#include "Object3D.h"
#include "SplitScreen.h"

using namespace std;
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
	vector<float> rotationY;
	
	vector<float> changeTime;		// 視点切り替え時間を数える
	vector<VECTOR3> changePosStart; // 視点移動の開始点
	vector<VECTOR3> changePosGoal;	// 視点移動の終了点
	vector<VECTOR3> changeLookStart;// 注視点移動の開始点
	vector<VECTOR3> changeLookGoal;	// 注視点移動の終了点

	vector<VECTOR3> prevPlayerPos;	// プレイヤーの前回位置
	float autoRotateSpeed;	// カメラの自動回転速度

	vector<MATRIX4X4>  view;
	vector<VECTOR3>  eyePt;
	vector<VECTOR3>  lookatPt;
};