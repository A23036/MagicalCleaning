#include "Camera.h"
#include "Player.h"
#include "CsvReader.h"
#include "SplitScreen.h"
#include "DataCarrier.h"
#include "Stage.h"

Camera::Camera()
{
	CsvLoad(); // csv����f�[�^�̐ݒ�

	ObjectManager::SetVisible(this, false);		// ���͕̂\�����Ȃ�
	SetDrawOrder(-1000);
	/*
	while (view.size() < ssObj->MultiSize())
	{
		MATRIX4X4 m = XMMatrixIdentity();
		view.emplace_back(m);
		VECTOR3 v = VECTOR3(0, 0, 0);
		eyePt.emplace_back(v);
		lookatPt.emplace_back(v);
	}
	*/
	// �z��v�f�̏�����
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

Camera::~Camera()
{
	SAFE_DELETE(csv);
}

void Camera::Start()
{
	for (int i = 0; i < MAXPLAYER; i++) {
		string s = "Player" + to_string(i+1);
		player[i] = ObjectManager::FindGameObjectWithTag<Player>(s);
	}

	st = ObjectManager::FindGameObject<Stage>();
	ssObj = ObjectManager::FindGameObject<SplitScreen>();
	dc = ObjectManager::FindGameObject<DataCarrier>();

	while (view.size() < ssObj->MultiSize())
	{
		MATRIX4X4 m = XMMatrixIdentity();
		view.emplace_back(m);
		VECTOR3 v = VECTOR3(0, 0, 0);
		eyePt.emplace_back(v);
		lookatPt.emplace_back(v);
	}
}

void Camera::Update()
{
	posOld = transform.position;
	if (ssObj->Multi())
	{
		// ����ʂ̂Ƃ�
		for (int i = 0; i < ssObj->MultiSize(); i++)
		{
			// �S��ʂ̂Ƃ�
			switch (i)
			{
			case 0:
				{
					// ���0��Player1���_
					//Player* pc = player[0];
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
					// ���1��Player2���_
					//Player* pc = ObjectManager::FindGameObjectWithTag<Player>("Player2");
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
					// ���2��Player3���_
					//Player* pc = ObjectManager::FindGameObjectWithTag<Player>("Player3");
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
					// ���3��Player4���_
					//Player* pc = ObjectManager::FindGameObjectWithTag<Player>("Player4");
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
		// �P��ʂ̂Ƃ�Player���_
		//Player* pc = ObjectManager::FindGameObject<Player>();
		if (player[0] != nullptr)
		{
			updateCamera(0, player[0]->Position(), player[0]->Rotation());
		}
		else {
			updateCamera(0, VECTOR3(0, 0, 0), VECTOR3(0, 0, 0));
		}
	}
	/*
	ImGui::Begin("Camera");
	if (ImGui::Button("DataLoad")) {
		CsvLoad();
	}
	ImGui::End();
	*/
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
	if (!dc->GetIsPlay() && dc->GetGameState() != 0) {
		return;
	}

	auto di = GameDevice()->m_pDI;
	
	// �v���C���[�̈ʒu���擾
	VECTOR3 playerPos = pos;

	MATRIX4X4 rotY = XMMatrixRotationY(rotationY[counter]);
	MATRIX4X4 trans = XMMatrixTranslation(pos.x, pos.y, pos.z);
	MATRIX4X4 m = rotY * trans;

	//�J�����̎蓮��]����
	if (dc->GetCameraSetting(counter) == 1) {
		float z = di->GetJoyState(counter).lZ;	// ��:0 / �E:65535

		// ���͒l�𐳋K�� (-1.0f ~ 1.0f)
		float normalizedZ = (z / 32767.5f) - 1.0f;

		// �f�b�h�]�[���̐ݒ�
		const float deadZone = 0.1f;
		if (fabs(normalizedZ) > deadZone) {
			// �J������Y����]����
			rotationY[counter] += normalizedZ * ROTATE_SPEED;
		}
	}
	else { //�J�����̎�����]����

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
		}
		//2024.10.25 �J����������]������
	}
	
	//MP�ϊ����̃J��������
	if (player[counter]->GetPlayerState() == sCharge) {

		//���X�e�B�b�N�̍��E����
		// ���͒l�𐳋K�� (-1.0f ~ 1.0f)
		float ix = di->GetJoyState(counter).lX / 1000.0f; 
		
		// �f�b�h�]�[���̐ݒ�
		const float deadZone = 0.1f;
		if (fabs(ix) > deadZone) {
			// �J������Y����]����
			rotationY[counter] += ix * ROTATE_SPEED;
		}
	}

	// 2024.10.12 �J������]���Z�b�g��
	// ��SHIFT�L�[�������ꂽ��J�������v���C���[�̌���Ɉړ�
	if (di->CheckKey(KD_TRG, DIK_LSHIFT)&&counter == 0 || di->CheckJoy(KD_TRG, 6,counter)) {
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
	
	VECTOR3 start = pos;
	//start.y = CameraPos.y;
	VECTOR3 end = transform.position;
	VECTOR3 hit;
	VECTOR3 normal;
	if (st->MapCol()->IsCollisionLay(start, end, hit, normal)) {
		VECTOR3 move = pos - transform.position; // �O�ɏo�������̃x�N�g�������
		VECTOR3 push = XMVector3Normalize(move) * 0.1; // ���̃x�N�g���̒�����0.1�ɂ���
		hit += push;
		end.x = hit.x;
		end.z = hit.z;
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
