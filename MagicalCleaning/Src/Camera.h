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

	void CsvLoad(); //CSV�t�@�C������f�[�^�̐ݒ�

	MATRIX4X4 View(int counter) { return view[counter]; }
	VECTOR3 EyePt(int counter) { return eyePt[counter]; }
	VECTOR3 LookatPt(int counter) { return lookatPt[counter]; }

private:
	void updateCamera(int counter, VECTOR3 pos, VECTOR3 rot);

	CsvReader* csv;
	DataCarrier* dc;
	Stage* st;

	VECTOR3 posOld;

	VECTOR3 CameraPos = VECTOR3(0, 0, 0);	// �J�����ʒu
	VECTOR3 LookPos = VECTOR3(0, 1, 0);		// �J���������_

	Player* player[MAXPLAYER];

	float CHANGE_TIME;		// �J�����ړ�����
	float ROTATE_SPEED;		// �J�����̎�����]���x

	SplitScreen* ssObj;

	VECTOR3 lookPosition;

	float rotationY[MAXPLAYER];
	
	float changeTime[MAXPLAYER];		// ���_�؂�ւ����Ԃ𐔂���
	VECTOR3 changePosStart[MAXPLAYER];	// ���_�ړ��̊J�n�_
	VECTOR3 changePosGoal[MAXPLAYER];	// ���_�ړ��̏I���_
	VECTOR3 changeLookStart[MAXPLAYER];	// �����_�ړ��̊J�n�_
	VECTOR3 changeLookGoal[MAXPLAYER];	// �����_�ړ��̏I���_

	VECTOR3 prevPlayerPos[MAXPLAYER];	// �v���C���[�̑O��ʒu

	vector<MATRIX4X4>  view;
	vector<VECTOR3>  eyePt;
	vector<VECTOR3>  lookatPt;
};