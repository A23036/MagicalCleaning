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

	int viewType;			// ���_�؂�ւ��̔ԍ�
	vector<float> rotationY;
	
	vector<float> changeTime;		// ���_�؂�ւ����Ԃ𐔂���
	vector<VECTOR3> changePosStart; // ���_�ړ��̊J�n�_
	vector<VECTOR3> changePosGoal;	// ���_�ړ��̏I���_
	vector<VECTOR3> changeLookStart;// �����_�ړ��̊J�n�_
	vector<VECTOR3> changeLookGoal;	// �����_�ړ��̏I���_

	vector<VECTOR3> prevPlayerPos;	// �v���C���[�̑O��ʒu
	float autoRotateSpeed;	// �J�����̎�����]���x

	vector<MATRIX4X4>  view;
	vector<VECTOR3>  eyePt;
	vector<VECTOR3>  lookatPt;
};