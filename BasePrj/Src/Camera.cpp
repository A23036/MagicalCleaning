#include "Camera.h"
#include "Player.h"
#include <iostream>
										//������_			//������_
static const VECTOR3 CameraPos[] = { VECTOR3(0, 3, -7), VECTOR3(0, 5, -0.5) };
static const VECTOR3 LookPos[] =   { VECTOR3(0, 1.5,  0), VECTOR3(0, 1,    1) };
static const float CHANGE_TIME_LIMIT = 0.5f; // �b

Camera::Camera()
{
	SetDrawOrder(-1000);
	viewType = 0;
	changeTime = CHANGE_TIME_LIMIT; // �؂�ւ��Ȃ�
	rotationY = 0.0f;
}

Camera::~Camera()
{
}


void Camera::Update()
{
	/*
	// ��̎��_��'L'�L�[�ɂ���Đ؂�ւ���
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

	// �v���C���[�̍s������߂�
	Player* player = ObjectManager::FindGameObject<Player>();

	MATRIX4X4 rotY = XMMatrixRotationY(rotationY);
	MATRIX4X4 trans = XMMatrixTranslation(player->Position().x, player->Position().y, player->Position().z);
	MATRIX4X4 m = rotY * trans;

	// ��SHIFT�L�[�������ꂽ��J�������v���C���[�̌���Ɉړ�
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_LSHIFT)) {
		//�v���C���[�̉�]��ۑ�
		rotationY = player->Rotation().y;

		changeTime = 0.0f;

		changePosStart = transform.position;
		changeLookStart = lookPosition;
		changePosGoal = CameraPos[viewType];
		changeLookGoal = LookPos[viewType];
	}

	// �v���C���[����]�E�ړ����ĂȂ����̃J�����ʒu��
	// �v���C���[�̉�]�E�ړ��s����|����ƁA
	if (changeTime >= CHANGE_TIME_LIMIT) {
		transform.position = CameraPos[viewType] * m;
		lookPosition = LookPos[viewType] * m;
	}
	else { // ���_�؂�ւ���
		changeTime += 1.0f / 60.0f;
		float timeRate = changeTime / CHANGE_TIME_LIMIT; // 0,0�`1.0
		//float rate = -((timeRate - 1) * (timeRate - 1)) + 1;
		float rate = 1 - pow(1 - timeRate, 3);
		VECTOR3 position = (changePosGoal * m - changePosStart) * rate + changePosStart;
		VECTOR3 look = (changeLookGoal * m - changeLookStart) * rate + changeLookStart;
		transform.position = position;
		lookPosition = look;
	}

	// �J�������ǂɂ߂荞�܂Ȃ��悤�ɂ���
	VECTOR3 start = player->Position();
	start.y = 1.0;
	VECTOR3 end = transform.position;
	std::list<Object3D*> objects = ObjectManager::FindGameObjects<Object3D>();
	for (auto g : objects) {
		VECTOR3 hit;
		if (g->HitLineToMesh(start, end, &hit)) {
			VECTOR3 move = player->Position() - transform.position; // �O�ɏo�������̃x�N�g�������
			VECTOR3 push = XMVector3Normalize(move) * 0.1; // ���̃x�N�g���̒�����0.1�ɂ���
			hit += push;
			end = hit;
		}
	}
	transform.position = end;
}

void Camera::Draw()
{
	GameDevice()->m_mView = XMMatrixLookAtLH(
		transform.position, // �J�������W
		lookPosition,	//�����_
		VECTOR3(0, 1, 0));
}
