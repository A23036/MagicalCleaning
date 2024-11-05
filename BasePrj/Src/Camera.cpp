#include "Camera.h"
#include "Player.h"
#include <iostream>
										//������_			//������_
static const VECTOR3 CameraPos[] = { VECTOR3(0, 3, -7), VECTOR3(0, 5, -0.5) };
static const VECTOR3 LookPos[] =   { VECTOR3(0, 1.5,  0), VECTOR3(0, 1,    1) };
static const float CHANGE_TIME_LIMIT = 0.5f; // �b

Camera::Camera()
{
	ObjectManager::SetVisible(this, false);		// ���͕̂\�����Ȃ�
	SetDrawOrder(-1000);
	viewType = 0;
	
	autoRotateSpeed = 0.05f;			// �J�����̎�����]���x

	ssObj = ObjectManager::FindGameObject<SplitScreen>();
	
	while (view.size() < ssObj->MultiSize())
	{
		MATRIX4X4 m = XMMatrixIdentity();
		view.emplace_back(m);                                                                         
		VECTOR3 v = VECTOR3(0, 0, 0);
		eyePt.emplace_back(v);
		lookatPt.emplace_back(v);
	}

	// �z��v�f�̏�����
	for (int i = 0; i < MAXPLAYER; i++) {
		rotationY[i] = 0.0f;
		prevPlayerPos[i] = VECTOR3(0, 0, 0);
		changeTime[i] = CHANGE_TIME_LIMIT;
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
		// ����ʂ̂Ƃ�
		for (int i = 0; i < ssObj->MultiSize(); i++)
		{
			/*
			// �Q��ʂ̂Ƃ�
			switch (i)
			{
			case 0:
			{
				// ���0��Player���_
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
				// ���1��EnemyRS���_
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
			// �S��ʂ̂Ƃ�
			switch (i)
			{
			case 0:
				{
					// ���0��Player1���_
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
					// ���1��Player2���_
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
					// ���2�͌��_���_
					updateCamera(i, VECTOR3(0, 0, 0), VECTOR3(0, 0, 0));
					/*
					// ���2��EnemyGolem���_
					EnemyGolem* enm = ObjectManager::FindGameObject<EnemyGolem>();
					if (enm != nullptr && enm->Mesh() != nullptr)
					{
						updateCamera(i, enm->Position(), enm->Rotation());
					}
					else {
						updateCamera(i, VECTOR3(0, 0, 0), VECTOR3(0, 0, 0));
					}
					*/
				}
				break;

			case 3:
				{
					// ���3�͌��_���_
					updateCamera(i, VECTOR3(0, 0, 0), VECTOR3(0, 0, 0));
				}
				break;
			}
			//*/
		}
	}
	else {
		// �P��ʂ̂Ƃ�Player���_
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
		transform.position, // �J�������W
		lookPosition,	//�����_
		VECTOR3(0, 1, 0));
}

void Camera::updateCamera(int counter, VECTOR3 pos, VECTOR3 rot)
{
	/* �f�o�b�O�Ɏg����������Ȃ��̂ŃR�����g�A�E�g
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

	// �v���C���[�̈ʒu���擾
	VECTOR3 playerPos = pos;

	MATRIX4X4 rotY = XMMatrixRotationY(rotationY[counter]);
	MATRIX4X4 trans = XMMatrixTranslation(pos.x, pos.y, pos.z);
	MATRIX4X4 m = rotY * trans;

	// 2024.10.12 �J������]���Z�b�g��

	// ��SHIFT�L�[�������ꂽ��J�������v���C���[�̌���Ɉړ�
	auto di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_TRG, DIK_LSHIFT) || di->CheckJoy(KD_TRG, DIJ_Y,counter)) {
		//�v���C���[�̉�]��ۑ�
		rotationY[counter] = rot.y;

		changeTime[counter] = 0.0f;

		changePosStart[counter] = eyePt[counter];
		changeLookStart[counter] = lookatPt[counter];
		changePosGoal[counter] = CameraPos[viewType];
		changeLookGoal[counter] = LookPos[viewType];
		
	}
	// 2024.10.12 �J������]���Z�b�g��

	// �v���C���[����]�E�ړ����ĂȂ����̃J�����ʒu��
	// �v���C���[�̉�]�E�ړ��s����|����ƁA
	if (changeTime[counter] >= CHANGE_TIME_LIMIT) {
		transform.position = CameraPos[viewType] * m;
		lookPosition = LookPos[viewType] * m;
	}
	else { // ���_�؂�ւ���
		changeTime[counter] += 1.0f / 60.0f;
		float timeRate = changeTime[counter] / CHANGE_TIME_LIMIT; // 0,0�`1.0
		//float rate = -((timeRate - 1) * (timeRate - 1)) + 1;
		float rate = 1 - pow(1 - timeRate, 3);
		VECTOR3 position = (changePosGoal[counter] * m - changePosStart[counter]) * rate + changePosStart[counter];
		VECTOR3 look = (changeLookGoal[counter] * m - changeLookStart[counter]) * rate + changeLookStart[counter];
		transform.position = position;
		lookPosition = look;
	}

	//2024.10.25 �J����������]������

	// �v���C���[�̈ړ��������v�Z
	VECTOR3 moveDir = playerPos - prevPlayerPos[counter];
	moveDir.y = 0.0f;	// ���������̓����𖳎�
	moveDir = XMVector3Normalize(moveDir);

	if (prevPlayerPos[counter] != VECTOR3(0, 0, 0) && moveDir.Length() != 0)
	{
		MATRIX4X4 rotY = XMMatrixRotationY(rotationY[counter]);	// �J�����̉�]��
		VECTOR3 rightDir = VECTOR3(1, 0, 0) * rotY;		// �J�������猩�ĉE�����̃x�N�g��

		float dotVal = dot(moveDir, rightDir); // ���ό��� 0:���� / �`1.0:�E�ړ� / -1.0�`:���ړ� 

		// �v���C���[�̌��ɃJ��������荞�܂��鏈��
		float targetRotationY = rotationY[counter];

		if (dotVal > 0.1) // �v���C���[���J�����̉E���ɂ���
		{
			// �ɂ₩�ɉE�ɉ�]������
			targetRotationY += autoRotateSpeed;
		}
		else if (dotVal < -0.1) // �v���C���[���J�����̍����ɂ���
		{
			// �ɂ₩�ɍ��ɉ�]������
			targetRotationY -= autoRotateSpeed;
		}

		// �v���C���[���΂ߑO�ɂ���ꍇ�͉�]�������}����
		float forwardDot = dot(moveDir, VECTOR3(0, 0, 1) * rotY); // ���ʕ����Ƃ̓���
		if (forwardDot > 0.0f) // �v���C���[���J�����̑O�ɂ���ꍇ
		{
			float adjustment = 1.0f - forwardDot; // �O�ɂ���قǉ�]��}����
			targetRotationY = rotationY[counter] + (targetRotationY - rotationY[counter]) * adjustment;
		}

		// ��]�ʂ��X�V
		rotationY[counter] = targetRotationY;

		ImGui::SetNextWindowPos(ImVec2(0, 160));
		ImGui::SetNextWindowSize(ImVec2(200, 60));
		ImGui::Begin("Dot");
		ImGui::InputFloat("dot", &dotVal);
		ImGui::End();
	}
	//2024.10.25 �J����������]������


	// �J�������ǂɂ߂荞�܂Ȃ��悤�ɂ���
	VECTOR3 start = pos;
	start.y = 1.0;
	VECTOR3 end = transform.position;
	std::list<Object3D*> objects = ObjectManager::FindGameObjects<Object3D>();
	for (auto g : objects) {
		VECTOR3 hit;
		if (g->HitLineToMesh(start, end, &hit)) {
			VECTOR3 move = pos - transform.position; // �O�ɏo�������̃x�N�g�������
			VECTOR3 push = XMVector3Normalize(move) * 0.1; // ���̃x�N�g���̒�����0.1�ɂ���
			hit += push;
			end = hit;
		}
	}
	transform.position = end;

	// �v���C���[�̈ʒu��ۑ�
	prevPlayerPos[counter] = playerPos;

	// ------------------------------------------------------------------
	// �J�������W��z��ɐݒ肷��
	eyePt[counter] = transform.position;   // �J�������W
	lookatPt[counter] = lookPosition;      // �����_
	view[counter] = XMMatrixLookAtLH(	    // �r���[�}�g���b�N�X
		transform.position,
		lookPosition,
		VECTOR3(0, 1, 0));

	// ------------------------------------------------------------------
	// ���_����̋����̂Q���DrawObject�ɐݒ肷��
	// ����́A���_����̋����̍~���ɕ`�悵��������
	std::list<Object3D*> objList = ObjectManager::FindGameObjects<Object3D>();
	for (Object3D*& obj : objList)
	{
		if (obj != this)
		{
			float distQ = magnitudeSQ(obj->Position() - transform.position);
			ObjectManager::SetEyeDist(obj, distQ, counter);  // ���_����̋����̂Q���DrawObject�ɐݒ�
		}
	}
}
