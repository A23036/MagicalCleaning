#include "Player.h"
#include "../Libs/Imgui/imgui.h"
#include <dinput.h>
#include "CsvReader.h"
#include "Stage.h"
#include "Camera.h"
#include "Dust.h"
#include "DustBox.h"

Player::Player()
{
}

Player::Player(VECTOR3 pos, VECTOR3 rot, int num)//�Z���N�g�V�[��/���U���g�V�[���Ŏg�p
{
	animator = new Animator(); // �C���X�^���X���쐬

	mesh = new CFbxMesh();

	std::string f = "Data/Player/Color" + std::to_string(num);
	mesh->Load((f + "/witch.mesh").c_str());

	mesh->LoadAnimation(aIdle, (f + "/standby.anmx").c_str(), true);

	animator->SetModel(mesh); // ���̃��f���ŃA�j���[�V��������
	animator->Play(aIdle);
	animator->SetPlaySpeed(1.0f);

	transform.position = pos;
	transform.rotation = rot;

	state = sStandby;
}


Player::Player(int num) : playerNum(num) // �v���C�V�[���Ŏg�p
{
	CsvLoad(); // csv����f�[�^�̐ݒ�

	dc = ObjectManager::FindGameObject<DataCarrier>();

	// �v���C���[�̎���ⴂ̐���
	child = new Broom(this,playerNum);

	animator = new Animator(); // �C���X�^���X���쐬

	mesh = new CFbxMesh();

	std::string f = "Data/Player/Color" + std::to_string(playerNum);
	mesh->Load((f + "/witch.mesh").c_str());

	mesh->LoadAnimation(aIdle, (f + "/idle.anmx").c_str(), true);
	mesh->LoadAnimation(aRun, (f + "/run.anmx").c_str(), true);
	//mesh->LoadAnimation(aWalk, (f + "/walk.anmx").c_str(), true);
	mesh->LoadAnimation(aJump, (f + "/jump.anmx").c_str(), false);
	mesh->LoadAnimation(aJump2, (f + "/jump2.anmx").c_str(), false);
	mesh->LoadAnimation(aFly, (f + "/fly.anmx").c_str(), false);
	mesh->LoadAnimation(aAttack1, (f + "/attack1.anmx").c_str(), false);
	mesh->LoadAnimation(aAttack2, (f + "/attack2.anmx").c_str(), false);
	//mesh->LoadAnimation(aAttack3, (f + "/attack3.anmx").c_str(), false);

	animator->SetModel(mesh); // ���̃��f���ŃA�j���[�V��������
	animator->Play(aIdle);
	animator->SetPlaySpeed(1.0f);
	
	transform.position = VECTOR3(0, 0, 0);
	transform.rotation = VECTOR3(0, 0, 0);
	state = sOnGround;
	curState = sOnGround;
	speedY = 0;
	mp = 0;
	weight = 0;
	jumpCount = 0;
	isFly = false;
	finishAtkAnim = true;

	msNum = 0;
	jnNum = 0;
	asNum = 0;
	arNum = 0;
	cwNum = 0;

	moveSpeed	= MoveSpeedT[msNum];
	jumpNum		= JumpNumT[jnNum];
	atkSpeed	= AtkSpeedT[asNum];
	atkRange	= AtkRangeT[arNum];
	carWeight	= CarWeightT[cwNum];

	selectPower = 0;
}

Player::~Player()
{
	SAFE_DELETE(mesh);
	SAFE_DELETE(animator);
}

void Player::Update()
{
	// ⴂ̈ʒu���X�V
	if (state != sStandby) { //�L�����Z���N�g��ʂł͎����Ȃ�

		MATRIX4X4 bone;
		if (state == sAttack2) {
			bone = mesh->GetFrameMatrices(18);// �v���C���[�̌��_����̍���̈ʒu(18�͍���)
		}
		else {
			bone = mesh->GetFrameMatrices(0);// �v���C���[�̌��_����̉E��̈ʒu(0�͉E��)
		}
		if (animator->PlayingID() == aFly) {
			bone = mesh->GetFrameMatrices(2);//�v���C���[�̃��[�g�ʒu
		}

		child->SetPos(bone);
	}
	
	
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(100, 60));
	ImGui::Begin("isFly");
	if (isFly) {
		ImGui::Text("Fly");
	}
	else
	{
		ImGui::Text("notFly");
	}
	ImGui::End();
	
	/**
	switch (state) {
	case sStandby:
		ImGui::Text("sStandby");
		break;
	case sOnGround:
		ImGui::Text("sOnGround");
		break;
	case sJump:
		ImGui::Text("sJump");
		break;
	case sAttack1:
		ImGui::Text("sAttack1");
		break;
	case sAttack2:
		ImGui::Text("sAttack2");
		break;
	case sStop:
		ImGui::Text("sStop");
		break;
	}
	ImGui::End();
	*/

	if (dc->GetIsPlay()) {
		animator->Update();
		state = curState;
	}
	else
	{
		state = sStop;
	}

	switch (state) {
	case sStandby:
		return;
	case sOnGround:
		UpdateOnGround();
		break;
	case sJump:
		UpdateJump();
		break;
	case sAttack1:
		UpdateAttack1();
		break;
	case sAttack2:
		UpdateAttack1();
		break;
	case sAttack3:
		UpdateAttack1();
		break;
	case sStop:
		return;
	default:
		break;
	}

	if (GameDevice()->m_pDI->CheckJoy(KD_TRG, 3, playerNum)) { //�\�͋���
		switch (selectPower) {
		case pMS:
			if (mp >= MoveSpeedC[msNum] && msNum < MsTableNum - 1)
			{
				mp -= MoveSpeedC[msNum];
				msNum++;
				moveSpeed = MoveSpeedT[msNum];
			}
			break;
		case pJN:
			if (mp >= JumpNumC[jnNum] && jnNum < JnTableNum - 1)
			{
				mp -= JumpNumC[jnNum];
				jnNum++;
				jumpNum = JumpNumT[jnNum];
			}
			break;
		case pAS:
			if (mp >= AtkSpeedC[asNum] && asNum < AsTableNum - 1)
			{
				mp -= AtkSpeedC[asNum];
				asNum++;
				atkSpeed = AtkSpeedT[asNum];
			}
			break;
		case pAR:
			if (mp >= AtkRangeC[arNum] && arNum < ArTableNum - 1)
			{
				mp -= AtkRangeC[arNum];
				arNum++;
				atkRange = AtkRangeT[arNum];
			}
			break;
		case pCW:
			if (mp >= CarWeightC[cwNum] && cwNum < CwTableNum - 1)
			{
				mp -= CarWeightC[cwNum];
				cwNum++;
				carWeight = CarWeightT[cwNum];
			}
			break;
		}
	}

	
	if (GameDevice()->m_pDI->CheckJoy(KD_TRG, 4, playerNum)) { //�����\�͕ύX
		selectPower--;
		if (selectPower < 0) {
			selectPower = 4;
		}
	}
	if (GameDevice()->m_pDI->CheckJoy(KD_TRG, 5, playerNum)) { //�����\�͕ύX
		selectPower++;
		if (selectPower > 4) {
			selectPower = 0;
		}
	}



	//�����蔻�菈��
	Stage* st = ObjectManager::FindGameObject<Stage>();
	VECTOR3 push;
	if (st->HitSphere(Collider(), &push)) {
		transform.position += push;
	}

	// ImGui�E�B���h�E�̈ʒu�ƃT�C�Y��ݒ�
	/*
	ImGui::SetNextWindowPos(ImVec2(0, 60));
	ImGui::SetNextWindowSize(ImVec2(100, 100));
	ImGui::Begin("PlayerPos");
	ImGui::InputFloat("X", &transform.position.x);
	ImGui::InputFloat("Y", &transform.position.y);
	ImGui::InputFloat("Z", &transform.position.z);
	ImGui::End();
	
	// ���̓{�^���m�F
	ImGui::SetNextWindowPos(ImVec2(0, 220));
	ImGui::SetNextWindowSize(ImVec2(200, 120));
	ImGui::Begin("CheckJoy");
	auto di = GameDevice()->m_pDI;
	auto* pdi = GameDevice()->m_pDI;
	DIJOYSTATE2 joyState = pdi->GetJoyState(playerNum);
	for (int i = 0; i < 32; i++)
	{
		// �e�{�^���̉�����Ԃ��m�F
		if (joyState.rgbButtons[i] & 0x80) // �{�^�� i ��������Ă���ꍇ
		{
			ImGui::Text("%dP : Button %d is pressed", playerNum,i); // �{�^���ԍ���\��
		}
	}
	
	float rx = di->GetJoyState().lRx;
	float ry = di->GetJoyState().lRy;
	float rz = di->GetJoyState().lRz;
	ImGui::End();
	*/
	// Dust�ɂ߂荞�܂Ȃ��悤�ɂ���
	// �����̍��W�́Atransform.position
	// Dust�̍��W��m��
	
	std::list<Dust*> dusts = ObjectManager::FindGameObjects<Dust>();
	
	for (Dust* dust : dusts) {
		SphereCollider tCol = dust->Collider(dust->GetNum());
		SphereCollider pCol = Collider();
		VECTOR3 pushVec = pCol.center - tCol.center;
		float rSum = pCol.radius + tCol.radius;
		if (pushVec.LengthSquare() < rSum * rSum) { // ���̓����蔻��
			// �������Ă�
			// �����o��������pushVec
			// �����o�����������߂�
			float pushLen = rSum - pushVec.Length();
			pushVec.y = 0.0f; // y�������̉����Ԃ��𖳌��ɂ���(�n�ʂɖ��܂����蕂�����肷�邽��)
			pushVec = XMVector3Normalize(pushVec); // pushVec�̒������P�ɂ���
			transform.position += pushVec * pushLen;
		}
	}
	
	// DustBox�ɂ߂荞�܂Ȃ��悤�ɂ���
	// �����̍��W�́Atransform.position
	// DustBox�̍��W��m��
	std::list<DustBox*> boxs = ObjectManager::FindGameObjects<DustBox>();

	for (DustBox* box : boxs) {
		SphereCollider tCol = box->Collider();
		SphereCollider pCol = Collider();
		VECTOR3 pushVec = pCol.center - tCol.center;
		float rSum = pCol.radius + tCol.radius;
		if (pushVec.LengthSquare() < rSum * rSum) { // ���̓����蔻��
			// �������Ă�
			// �����o��������pushVec
			// �����o�����������߂�
			float pushLen = rSum - pushVec.Length();
			pushVec.y = 0.0f; // y�������̉����Ԃ��𖳌��ɂ���(�n�ʂɖ��܂����蕂�����肷�邽��)
			pushVec = XMVector3Normalize(pushVec); // pushVec�̒������P�ɂ���
			transform.position += pushVec * pushLen;
		}
	}

	// player�ɂ߂荞�܂Ȃ��悤�ɂ���
	// �����̍��W�́Atransform.position
	// player�̍��W��m��
	std::list<Player*> players = ObjectManager::FindGameObjects<Player>();

	for (Player* player : players) {
		SphereCollider tCol = player->Collider();
		SphereCollider pCol = Collider();
		VECTOR3 pushVec = pCol.center - tCol.center;
		float rSum = pCol.radius + tCol.radius;
		if (pushVec.LengthSquare() < rSum * rSum) { // ���̓����蔻��
			// �������Ă�
			// �����o��������pushVec
			// �����o�����������߂�
			float pushLen = rSum - pushVec.Length();
			pushVec.y = 0.0f; // y�������̉����Ԃ��𖳌��ɂ���(�n�ʂɖ��܂����蕂�����肷�邽��)
			pushVec = XMVector3Normalize(pushVec); // pushVec�̒������P�ɂ���
			transform.position += pushVec * pushLen;
		}
	}

	curState = state;
}
	
void Player::Draw()
{
	Object3D::Draw(); // �p�����̊֐����Ă�
	/*
	MATRIX4X4 tip = XMMatrixRotationRollPitchYawFromVector(VECTOR3(-33, 82, 0) * DegToRad);
	VECTOR3 tipPos = VECTOR3(0, 0, 1.2f) * tip;
	VECTOR3 tipPos = VECTOR3(0.9966, 0.6536, 0.140);
	MATRIX4X4 mat = transform.matrix();// ���E(���[���h)�̒��ŁA�v���C���[�̈ʒu�ƌ���
	MATRIX4X4 bone = mesh->GetFrameMatrices(34); // �v���C���[�̌��_����̎��̈ʒu(34�͎��)
	VECTOR3 start = VECTOR3(0, 0, 0) * bone * mat;
	VECTOR3 end = tipPos * bone * mat;
	
	CSprite spr;
	spr.DrawLine3D(start, end, RGB(255, 0, 0));
	*/
}

void Player::CsvLoad()
{
	// csv����f�[�^�ǂݍ���
	csv = new CsvReader("data/csv/Paramater.csv");
	if (csv->GetLines() < 1) {
		MessageBox(NULL, "Paramater.csv���ǂ߂܂���", "�G���[", MB_OK);
	}

	for (int i = 1; i < csv->GetLines(); i++) { //CSV�t�@�C������ݒ�̓ǂݍ���
		if (csv->GetString(i, 0) == "Player") {
			if (csv->GetString(i, 1) == "Gravity") {		// �d��
				GRAVITY = csv->GetFloat(i, 3);
			}
			if (csv->GetString(i, 1) == "JumpPower") {		// �W�����v��
				JUMP_POWER = csv->GetFloat(i, 3);
			}
			if (csv->GetString(i, 1) == "MoveSpeed") {		// �ړ����x
				MOVE_SPEED = csv->GetFloat(i, 3);
			}
			if (csv->GetString(i, 1) == "MoveSpeedT") {		// �ړ����x�e�[�u��
				for (int j = 0; j < MsTableNum; j++) {
					MoveSpeedT[j] = csv->GetFloat(i, 3+j);
				}
			}
			if (csv->GetString(i, 1) == "JumpNumT") {		// �W�����v�񐔃e�[�u��
				for (int j = 0; j < JnTableNum; j++) {
					JumpNumT[j] = csv->GetInt(i, 3 + j);
				}
			}
			if (csv->GetString(i, 1) == "AtkSpeedT") {		// �U�����x�e�[�u��
				for (int j = 0; j < AsTableNum; j++) {
					AtkSpeedT[j] = csv->GetFloat(i, 3 + j);
				}
			}
			if (csv->GetString(i, 1) == "AtkRangeT") {		// �U���͈̓e�[�u��
				for (int j = 0; j < ArTableNum; j++) {
					AtkRangeT[j] = csv->GetFloat(i, 3 + j);
				}
			}
			if (csv->GetString(i, 1) == "CarWeightT") {		// �^���\�d�ʃe�[�u��
				for (int j = 0; j < CwTableNum; j++) {
					CarWeightT[j] = csv->GetInt(i, 3 + j);
				}
			}
			if (csv->GetString(i, 1) == "MoveSpeedC") {		// �ړ����x�R�X�g�e�[�u��
				for (int j = 0; j < MsTableNum; j++) {
					MoveSpeedC[j] = csv->GetInt(i, 3 + j);
				}
			}
			if (csv->GetString(i, 1) == "JumpNumC") {		// �W�����v�񐔃R�X�g�e�[�u��
				for (int j = 0; j < JnTableNum; j++) {
					JumpNumC[j] = csv->GetInt(i, 3 + j);
				}
			}
			if (csv->GetString(i, 1) == "AtkSpeedC") {		// �U�����x�R�X�g�e�[�u��
				for (int j = 0; j < AsTableNum; j++) {
					AtkSpeedC[j] = csv->GetInt(i, 3 + j);
				}
			}
			if (csv->GetString(i, 1) == "AtkRangeC") {		// �U���͈̓R�X�g�e�[�u��
				for (int j = 0; j < ArTableNum; j++) {
					AtkRangeC[j] = csv->GetInt(i, 3 + j);
				}
			}
			if (csv->GetString(i, 1) == "CarWeightC") {		// �^���\�d�ʃR�X�g�e�[�u��
				for (int j = 0; j < CwTableNum; j++) {
					CarWeightC[j] = csv->GetInt(i, 3 + j);
				}
			}
		}
		if (csv->GetString(i, 0) == "Dust") {
			break;
		}
	}
}

SphereCollider Player::Collider()
{
	SphereCollider col;
	col.center = transform.position + VECTOR3(0, 0.6f, 0);
	col.radius = 0.58f;
	return col;
}

void Player::SetPlayerState(int state)
{
	this->state = state;
}

void Player::SetPlayerCurState(int state)
{
	curState = state;
}

void Player::AddMP(int n)
{
	mp += n;
}

void Player::AddWeight(int n)
{
	weight += n;

}

void Player::UpdateOnGround()
{
	
	auto di = GameDevice()->m_pDI;
	
	int x = di->GetJoyState(playerNum).lX;	// �E:1000 / ��:-1000
	int y = di->GetJoyState(playerNum).lY;	// ��:1000 / ��:-1000
	/*
	ImGui::SetNextWindowPos(ImVec2(0, 300));
	ImGui::SetNextWindowSize(ImVec2(200, 200));
	ImGui::Begin("Joystick");
	ImGui::InputInt("IX", &x);
	ImGui::InputInt("IY", &y);
	ImGui::End();
	*/
	Stage* st = ObjectManager::FindGameObject<Stage>();
	if (!(st->IsLandBlock(transform.position))) {
		// ��
		state = sJump;
		return; //���̉��̏����͍s��Ȃ�
	}
	 
	// 2024.10.26 �v���C���[������R���g���[���[�ɑΉ���
	
	// �R���g���[���[�X�e�B�b�N�̓��͂��擾
	// ���K������ -1.0�`1.0 �͈̔͂ɂ���
	float ix = di->GetJoyState(playerNum).lX / 1000.0f;
	float iy = di->GetJoyState(playerNum).lY / 1000.0f;
	
	// �X�e�B�b�N�����͂���Ă��邩�m�F
	if (fabs(ix) > 0.1f || fabs(iy) > 0.1f) {
		// ���͂Ɋ�Â��Ĉړ�������ݒ�
		VECTOR3 inputDirection = VECTOR3(ix, 0, -iy); // Z�������͔��]

		// �J������Y����]���擾
		Camera* camera = ObjectManager::FindGameObject<Camera>();
		float cameraYRotation = camera->GetRotY(playerNum);

		// �J�����̉�]�Ɋ�Â��ړ��x�N�g���̌v�Z
		MATRIX4X4 cameraRotY = XMMatrixRotationY(cameraYRotation);
		VECTOR3 moveDirection = XMVector3TransformNormal(inputDirection, cameraRotY);
		moveDirection = XMVector3Normalize(moveDirection) * MOVE_SPEED * sqrt(ix * ix + iy * iy); // �X����ɉ��������x

		// �ړ��̓K�p
		transform.position += moveDirection * moveSpeed;

		// �v���C���[�̉�]���J������ŕ�������������
		transform.rotation.y = cameraYRotation + atan2(ix, -iy); // �J�����̉�]�ɑ΂��ăX�e�B�b�N�̕����ɍ��킹��

		if (sqrt(ix * ix + iy * iy) * moveSpeed > 2.0) {
			// ��s�A�j���[�V�������Đ�
			animator->MergePlay(aFly, 0);
			animator->SetPlaySpeed(1.0f);
			isFly = true;
		}
		else {
			//���s���x�ɉ������A�j���[�V�����X�s�[�h��ݒ�
			animator->SetPlaySpeed(sqrt(ix * ix + iy * iy) * moveSpeed);

			// ���s�A�j���[�V�������Đ�
			animator->MergePlay(aRun, 0);
			isFly = false;
		}
	}
	else {
		// �X�e�B�b�N���X���Ă��Ȃ��ꍇ�͑ҋ@�A�j���[�V�������Đ�
		animator->SetPlaySpeed(1.0f);
		animator->MergePlay(aIdle,0);
	}
	// 2024.10.26 �v���C���[������R���g���[���[�ɑΉ���

	if ((di->CheckKey(KD_TRG, DIK_SPACE) || di->CheckJoy(KD_TRG, 2, playerNum) )) { //�W�����v
		speedY = JUMP_POWER;
		animator->MergePlay(aJump,0);
		animator->SetPlaySpeed(1.0f);
		jumpCount++;
		state = sJump;
	}
	if (di->CheckKey(KD_TRG, DIK_N) || di->CheckJoy(KD_TRG, 0, playerNum)) { //�U���{�^��
		animator->MergePlay(aAttack1,0);
		animator->SetPlaySpeed(1.0f * atkSpeed);
		transform.rotation.y += 15 * DegToRad; //���ʕ����ɉ�]������
		state = sAttack1;
	}
	if (di->CheckKey(KD_TRG, DIK_M) || di->CheckJoy(KD_TRG, 1, playerNum)) { //�U���{�^��
		animator->MergePlay(aAttack2,0);
		animator->SetPlaySpeed(1.0f * atkSpeed);
		transform.rotation.y += 10 * DegToRad; //���ʕ����ɉ�]������
		state = sAttack2;
	}
}


void Player::UpdateJump()
{
	Stage* st = ObjectManager::FindGameObject<Stage>();
	transform.position.y += speedY;
	speedY -= GRAVITY;	// �d��

	auto di = GameDevice()->m_pDI;
	// �R���g���[���[�X�e�B�b�N�̓��͂��擾
	// ���K������ -1.0�`1.0 �͈̔͂ɂ���
	float ix = di->GetJoyState(playerNum).lX / 1000.0f;
	float iy = di->GetJoyState(playerNum).lY / 1000.0f;

	// �X�e�B�b�N�����͂���Ă��邩�m�F
	if (fabs(ix) > 0.1f || fabs(iy) > 0.1f) {
		// ���͂Ɋ�Â��Ĉړ�������ݒ�
		VECTOR3 inputDirection = VECTOR3(ix, 0, -iy); // Z�������͔��]

		// �J������Y����]���擾
		Camera* camera = ObjectManager::FindGameObject<Camera>();
		float cameraYRotation = camera->GetRotY(playerNum);

		// �J�����̉�]�Ɋ�Â��ړ��x�N�g���̌v�Z
		MATRIX4X4 cameraRotY = XMMatrixRotationY(cameraYRotation);
		VECTOR3 moveDirection = XMVector3TransformNormal(inputDirection, cameraRotY);
		moveDirection = XMVector3Normalize(moveDirection) * MOVE_SPEED* sqrt(ix * ix + iy * iy); // �X����ɉ��������x

		// �ړ��̓K�p
		transform.position += moveDirection * moveSpeed;

		// �v���C���[�̉�]���J������ŕ�������������
		transform.rotation.y = cameraYRotation + atan2(ix, -iy); // �J�����̉�]�ɑ΂��ăX�e�B�b�N�̕����ɍ��킹��

	}

	if (di->CheckJoy(KD_TRG, 2, playerNum) && jumpCount <= jumpNum) {
		speedY = JUMP_POWER;
		if (jumpCount % 2 == 0) {
			animator->MergePlay(aJump);
		}
		else {
			animator->MergePlay(aJump2);
		}
		animator->SetPlaySpeed(1.0f);
		jumpCount++;
	}

	if (st->IsLandBlock(transform.position)) {
		// �W�����v�I��
		state = sOnGround;
		jumpCount = 0;
	}
}

void Player::UpdateAttack1()
{	
	// �S�~�ɍU���𓖂Ă�
	std::list<Dust*> dusts = ObjectManager::FindGameObjects<Dust>();
	
	if (!finishAtkAnim && animator->CurrentFrame() <= 20.0f/atkSpeed) { //�U���̃q�b�g�����^�C�~���O
		finishAtkAnim = true;
		for (Dust* d : dusts) {
			SphereCollider dCol = d->Collider(d->GetNum()); //�S�~�̔��苅
			SphereCollider atkCol = Collider();			//�U������̋�
			VECTOR3 forward = VECTOR3(0, 0, 1.0f); // ��]���ĂȂ����̈ړ���
			MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
			forward = forward * rotY; // �L�����̌����Ă���ւ̈ړ���
			atkCol.center = transform.position + forward; //�U������̋������
			atkCol.radius = 1.0f;
			VECTOR3 pushVec = atkCol.center - dCol.center;
			float rSum = atkCol.radius + dCol.radius;
			if (pushVec.LengthSquare() < rSum * rSum) { // ���̓����蔻��
				// �������Ă�
				d->AddDamage(this,1); //�_���[�W��^����
			}
		}
	}
	
	if (animator->Finished())
	{
		//�U���A�j���[�V�����̏I��
		finishAtkAnim = false;
		animator->SetPlaySpeed(1.0f);
		transform.rotation.y -= 15 * DegToRad; //��]��߂�
		state = sOnGround;
	}

}

void Player::UpdateAttack2()
{
	/*
	// �S�~�ɍU���𓖂Ă�
	std::list<Dust*> dusts = ObjectManager::FindGameObjects<Dust>();
	VECTOR3 tipPos = VECTOR3(0.9966, 0.6536, 0.140);
	MATRIX4X4 mat = transform.matrix();// ���E(���[���h)�̒��ŁA�v���C���[�̈ʒu�ƌ���
	MATRIX4X4 bone = mesh->GetFrameMatrices(34); // �v���C���[�̌��_����̎��̈ʒu(34�͎��)
	VECTOR3 start = VECTOR3(0, 0, 0) * bone * mat;
	VECTOR3 end = tipPos * bone * mat;

	for (Dust* d : dusts)
	{
		if (d->HitLineToMesh(start, end)) {
			d->AddDamage(1); //�S�~�ɓ��Ă�
		}
		if (animator->Finished())
		{
			//�U���A�j���[�V�����̏I��
			state = sOnGround;
		}
	}*/

	// �S�~�ɍU���𓖂Ă�
	std::list<Dust*> dusts = ObjectManager::FindGameObjects<Dust>();

	if (!finishAtkAnim && animator->CurrentFrame() <= 20.0f/atkSpeed) { //�U���̃q�b�g�����^�C�~���O
		finishAtkAnim = true;
		for (Dust* d : dusts) {
			SphereCollider dCol = d->Collider(d->GetNum()); //�S�~�̔��苅
			SphereCollider atkCol = Collider();			//�U������̋�
			VECTOR3 forward = VECTOR3(0, 0, 1.0f); // ��]���ĂȂ����̈ړ���
			MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
			forward = forward * rotY; // �L�����̌����Ă���ւ̈ړ���
			atkCol.center = transform.position + forward; //�U������̋������
			atkCol.radius = 1.0f;
			VECTOR3 pushVec = atkCol.center - dCol.center;
			float rSum = atkCol.radius + dCol.radius;
			if (pushVec.LengthSquare() < rSum * rSum) { // ���̓����蔻��
				// �������Ă�
				d->AddDamage(this,1); //�_���[�W��^����
			}
		}
	}

	if (animator->Finished())
	{
		//�U���A�j���[�V�����̏I��
		finishAtkAnim = false;
		animator->SetPlaySpeed(1.0f);
		state = sOnGround;
	}
}

void Player::UpdateAttack3()
{
}

//�@�v���C���[�̎����
Broom::Broom(Object3D* parentModel, int num)
{
	parent = parentModel;

	mesh = new CFbxMesh();

	std::string f = "Data/Player/Color" + std::to_string(num) + "/weapon";
	mesh->Load((f + "/broom.mesh").c_str());

	transform.position = VECTOR3(0, 0, 0);
	transform.rotation = VECTOR3(0, 0, -70);
}

Broom::~Broom()
{
}

void Broom::Update()
{
	Player* pl = dynamic_cast<Player*>(parent);

	// ��Ԃ��Ƃ̊p�x�ύX
	switch (pl->GetPlayerState()) {
	case sOnGround:
		if (!pl->GetIsFly()) {
			transform.position = VECTOR3(0, 0, 0);
			transform.rotation = VECTOR3(0, 0, -70);
		}
		else {
			transform.position = VECTOR3(7*0.01, -10*0.01, -35*0.01);
			transform.rotation = VECTOR3(77, 0, 90);
		}
		break;

	case sJump:
		transform.position = VECTOR3(0, 0, 0);
		transform.rotation = VECTOR3(0, 0, -70);
		break;
	case sAttack1:
	case sAttack2:
		transform.position = VECTOR3(0, 0, 0);
		transform.rotation = VECTOR3(0, 0, 0);
		break;
	}
}

void Broom::Draw()
{
	mesh->Render(transform.matrix() * mat * parent->Matrix());
}

void Broom::SetPos(MATRIX4X4 m)
{
	mat = m;
}
