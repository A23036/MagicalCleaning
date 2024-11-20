#include "Camera.h"
#include "Player.h"
#include "CsvReader.h"
#include <iostream>

Camera::Camera()
{
	CsvLoad(); // csv����f�[�^�̐ݒ�

	ObjectManager::SetVisible(this, false);		// ���͕̂\�����Ȃ�
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

	// �z��v�f�̏�����
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
					// ���2��Player3���_
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
					// ���3��Player4���_
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

void Camera::CsvLoad()
{
	// csv����f�[�^�ǂݍ���
	csv = new CsvReader("data/csv/Paramater.csv");
	if (csv->GetLines() < 1) {
		MessageBox(NULL, "Paramater.csv���ǂ߂܂���", "�G���[", MB_OK);
	}

	for (int i = 1; i < csv->GetLines(); i++) { //CSV�t�@�C������ݒ�̓ǂݍ���
		if (csv->GetString(i, 0) == "Camera") {
			if (csv->GetString(i, 1) == "CameraPos") {		// �J�������W
				CameraPos = VECTOR3( csv->GetFloat(i, 3), csv->GetFloat(i, 4), csv->GetFloat(i, 5) );
			}
			if (csv->GetString(i, 1) == "LookPos") {		// �J���������_
				LookPos = VECTOR3( csv->GetFloat(i, 3), csv->GetFloat(i, 4), csv->GetFloat(i, 5) );
			}
			if (csv->GetString(i, 1) == "ChangeTime") {		// �J�����؂�ւ�����
				CHANGE_TIME = csv->GetFloat(i, 3);
			}
			if (csv->GetString(i, 1) == "RotateSpeed") {	// �J������]���x
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
	// �v���C���[�̈ʒu���擾
	VECTOR3 playerPos = pos;

	MATRIX4X4 rotY = XMMatrixRotationY(rotationY[counter]);
	MATRIX4X4 trans = XMMatrixTranslation(pos.x, pos.y, pos.z);
	MATRIX4X4 m = rotY * trans;

	// 2024.10.12 �J������]���Z�b�g��

	// ��SHIFT�L�[�������ꂽ��J�������v���C���[�̌���Ɉړ�
	auto di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_TRG, DIK_LSHIFT) || di->CheckJoy(KD_TRG, 6,counter)) {
		//�v���C���[�̉�]��ۑ�
		rotationY[counter] = rot.y;

		changeTime[counter] = 0.0f;

		changePosStart[counter] = eyePt[counter];
		changeLookStart[counter] = lookatPt[counter];
		changePosGoal[counter] = CameraPos;
		changeLookGoal[counter] = LookPos;
		
	}
	// 2024.10.12 �J������]���Z�b�g��

	// �v���C���[����]�E�ړ����ĂȂ����̃J�����ʒu��
	// �v���C���[�̉�]�E�ړ��s����|����ƁA
	if (changeTime[counter] >= CHANGE_TIME) {
		transform.position = CameraPos * m;
		lookPosition = LookPos * m;
	}
	else { // ���_�؂�ւ���
		changeTime[counter] += 1.0f / 60.0f;
		float timeRate = changeTime[counter] / CHANGE_TIME; // 0,0�`1.0
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
			targetRotationY += ROTATE_SPEED;
		}
		else if (dotVal < -0.1) // �v���C���[���J�����̍����ɂ���
		{
			// �ɂ₩�ɍ��ɉ�]������
			targetRotationY -= ROTATE_SPEED;
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

		/*
		ImGui::SetNextWindowPos(ImVec2(0, 160));
		ImGui::SetNextWindowSize(ImVec2(200, 60));
		ImGui::Begin("Dot");
		ImGui::InputFloat("dot", &dotVal);
		ImGui::End();
		*/
	}
	//2024.10.25 �J����������]������


	// �J�������ǂɂ߂荞�܂Ȃ��悤�ɂ���
	/*
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
	*/
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
