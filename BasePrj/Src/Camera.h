#pragma once
#include "Object3D.h"
#include "CsvReader.h"
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

	CsvReader* csv;
	int readLine;

	VECTOR3 CameraPos;
	VECTOR3 LookPos;

	float CHANGE_TIME;

	SplitScreen* ssObj;

	VECTOR3 lookPosition;

	float rotationY[MAXPLAYER];
	
	float changeTime[MAXPLAYER];		// ���_�؂�ւ����Ԃ𐔂���
	VECTOR3 changePosStart[MAXPLAYER];	// ���_�ړ��̊J�n�_
	VECTOR3 changePosGoal[MAXPLAYER];	// ���_�ړ��̏I���_
	VECTOR3 changeLookStart[MAXPLAYER];	// �����_�ړ��̊J�n�_
	VECTOR3 changeLookGoal[MAXPLAYER];	// �����_�ړ��̏I���_

	VECTOR3 prevPlayerPos[MAXPLAYER];	// �v���C���[�̑O��ʒu
	float ROTATE_SPEED;				// �J�����̎�����]���x

	vector<MATRIX4X4>  view;
	vector<VECTOR3>  eyePt;
	vector<VECTOR3>  lookatPt;
};