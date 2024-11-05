#include "Player.h"
#include "../Libs/Imgui/imgui.h"
#include "Stage.h"
#include "Camera.h"
#include "Dust.h"
#include "DustBox.h"
#include <dinput.h>

namespace { // ����cpp�ȊO�ł͎g���Ȃ�
	static const float Gravity = 0.005f; // �d�͉����x(���̒l)
	static const float JumpPower = 0.1f;
	static const float RotationSpeed = 3.0f; // ��]���x(�x)
	static const float MoveSpeed = 0.05f;
};

Player::Player()
{
	// �v���C���[�̎���ⴂ̐���
	child = new Broom(this);

	animator = new Animator(); // �C���X�^���X���쐬

	mesh = new CFbxMesh();

	//�L�����N�^�[���f���ݒ�
	mesh->Load("Data/Player2/witch.mesh");	
	mesh->LoadAnimation(aIdle, "Data/Player2/idle.anmx", true);
	mesh->LoadAnimation(aRun, "Data/Player2/run.anmx", true);
	mesh->LoadAnimation(aWalk, "Data/Player2/walk.anmx", true);
	mesh->LoadAnimation(aJump, "Data/Player2/jump.anmx", false);
	mesh->LoadAnimation(aAttack1, "Data/Player2/attack1.anmx", false);
	mesh->LoadAnimation(aAttack2, "Data/Player2/attack2.anmx", false);
	mesh->LoadAnimation(aAttack3, "Data/Player2/attack3.anmx", false);
	//mesh->Load("Data/Player/PlayerChara.mesh");
	//mesh->LoadAnimation(aIdle, "Data/Player/Idle.anmx", true);
	//mesh->LoadAnimation(aRun, "Data/Player/Run.anmx", true);
	//mesh->LoadAnimation(aAttack1, "Data/Player/attack1.anmx", false);
	//mesh->LoadAnimation(aAttack2, "Data/Player/attack2.anmx", false);
	//mesh->LoadAnimation(aAttack3, "Data/Player/attack3.anmx", false);

	animator->SetModel(mesh); // ���̃��f���ŃA�j���[�V��������
	animator->Play(aIdle);
	animator->SetPlaySpeed(1.0f);
	
	transform.position = VECTOR3(0, 0, 0);
	transform.rotation = VECTOR3(0, 0, 0);
	state = sOnGround;
	speedY = 0;
	mp = 0;
	weight = 0;
	doneAtkAnim = false;

	moveSpeed	= 1;
	jumpNum		= 1;
	atkSpeed	= 1;
	atkRange	= 1;
	carWeight	= 1;

	playerNum = 0;
}

Player::~Player()
{
	if (mesh != nullptr) {
		delete mesh;
		mesh = nullptr;
	}
	if (animator != nullptr) {
		delete animator;
		animator = nullptr;
	}
}

void Player::Update()
{
	// ⴂ̈ʒu���X�V
	MATRIX4X4 bone;
	if (state != 3) {
		bone = mesh->GetFrameMatrices(0);// �v���C���[�̌��_����̉E��̈ʒu(0�͉E��)
	}
	else {
		bone = mesh->GetFrameMatrices(19);// �v���C���[�̌��_����̉E��̈ʒu(0�͉E��)
	}
	
	child->SetPos(bone);

	animator->Update(); // ���t���[���AUpdate���Ă�

	switch (state) {
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
	}

	//�����蔻�菈��
	
	Stage* st = ObjectManager::FindGameObject<Stage>();
	VECTOR3 push;
	if (st->HitSphere(Collider(), &push)) {
		transform.position += push;
	}

	// ImGui�E�B���h�E�̈ʒu�ƃT�C�Y��ݒ�
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(100, 60));
	ImGui::Begin("State");
	switch (state) {
	case 0:
		ImGui::Text("sOnGround");
		break;
	case 1:
		ImGui::Text("sJump");
		break;
	case 2:
		ImGui::Text("sAttack");
		break;
	}
	ImGui::End();
	
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(100, 100));
	ImGui::Begin("PlayerNum");
	ImGui::InputInt("X", &playerNum);
	ImGui::End();

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
	//ImGui::InputFloat("rX", &rx);
	//ImGui::InputFloat("rY", &ry);
	//ImGui::InputFloat("rZ", &rz);
	ImGui::End();
	
	// Dust�ɂ߂荞�܂Ȃ��悤�ɂ���
	// �����̍��W�́Atransform.position
	// Dust�̍��W��m��
	std::list<Dust*> dusts =ObjectManager::FindGameObjects<Dust>();
	
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

SphereCollider Player::Collider()
{
	SphereCollider col;
	col.center = transform.position + VECTOR3(0, 0.6f, 0);
	col.radius = 0.55f;
	return col;
}

void Player::addMP(int n)
{
	mp += n;
}

void Player::addWeight(int n)
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
		moveDirection = XMVector3Normalize(moveDirection) * MoveSpeed * sqrt(ix * ix + iy * iy); // �X����ɉ��������x

		// �ړ��̓K�p
		transform.position += moveDirection;

		// �v���C���[�̉�]���J������ŕ�������������
		transform.rotation.y = cameraYRotation + atan2(ix, -iy); // �J�����̉�]�ɑ΂��ăX�e�B�b�N�̕����ɍ��킹��

		//���s���x�ɉ������A�j���[�V�����X�s�[�h��ݒ�
		animator->SetPlaySpeed(sqrt(ix * ix + iy * iy));
		
		// ���s�A�j���[�V�������Đ�
		animator->MergePlay(aRun);
	}
	else {
		// �X�e�B�b�N���X���Ă��Ȃ��ꍇ�͑ҋ@�A�j���[�V�������Đ�
		animator->SetPlaySpeed(1.0f);
		animator->MergePlay(aIdle);
	}
	// 2024.10.26 �v���C���[������R���g���[���[�ɑΉ���

	if (di->CheckKey(KD_TRG, DIK_SPACE) || di->CheckJoy(KD_TRG, 2, playerNum)) {
		speedY = JumpPower;
		state = sJump;
		animator->SetPlaySpeed(1.0f);
		animator->MergePlay(aJump);
	}
	if (di->CheckKey(KD_TRG, DIK_N) || di->CheckJoy(KD_TRG, 0, playerNum)) { //�U���{�^��
		animator->MergePlay(aAttack1);
		animator->SetPlaySpeed(1.0f);
		state = sAttack1;
	}
	if (di->CheckKey(KD_TRG, DIK_M) || di->CheckJoy(KD_TRG, 1, playerNum)) { //�U���{�^��
		animator->MergePlay(aAttack2);
		animator->SetPlaySpeed(1.0f);
		state = sAttack2;
	}

	
}


void Player::UpdateJump()
{
	Stage* st = ObjectManager::FindGameObject<Stage>();
	transform.position.y += speedY;
	speedY -= Gravity;	// �d��

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
		moveDirection = XMVector3Normalize(moveDirection) * MoveSpeed * sqrt(ix * ix + iy * iy); // �X����ɉ��������x

		// �ړ��̓K�p
		transform.position += moveDirection;

		// �v���C���[�̉�]���J������ŕ�������������
		transform.rotation.y = cameraYRotation + atan2(ix, -iy); // �J�����̉�]�ɑ΂��ăX�e�B�b�N�̕����ɍ��킹��

	}

	if (st->IsLandBlock(transform.position)) {
		// �W�����v�I��
		state = sOnGround;
	}
}

void Player::UpdateAttack1()
{	
	// �S�~�ɍU���𓖂Ă�
	std::list<Dust*> dusts = ObjectManager::FindGameObjects<Dust>();
	
	if (!doneAtkAnim && animator->CurrentFrame() == 20) { //�U���̃q�b�g�����^�C�~���O
		doneAtkAnim = true;
		for (Dust* d : dusts) {
			SphereCollider dCol = d->Collider(d->GetNum()); //�S�~�̔��苅
			SphereCollider atkCol = Collider();			//�U������̋�
			VECTOR3 forward = VECTOR3(0, 0.5f, 1.0f); // ��]���ĂȂ����̈ړ���
			MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
			forward = forward * rotY; // �L�����̌����Ă���ւ̈ړ���
			atkCol.center = transform.position + forward; //�U������̋������
			atkCol.radius = 1.0f;
			VECTOR3 pushVec = atkCol.center - dCol.center;
			float rSum = atkCol.radius + dCol.radius;
			if (pushVec.LengthSquare() < rSum * rSum) { // ���̓����蔻��
				// �������Ă�
				d->AddDamage(1); //�_���[�W��^����
			}
		}
	}
	
	if (animator->Finished())
	{
		//�U���A�j���[�V�����̏I��
		doneAtkAnim = false;
		animator->SetPlaySpeed(1.0f);
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

	if (!doneAtkAnim && animator->CurrentFrame() == 20) { //�U���̃q�b�g�����^�C�~���O
		doneAtkAnim = true;
		for (Dust* d : dusts) {
			SphereCollider dCol = d->Collider(d->GetNum()); //�S�~�̔��苅
			SphereCollider atkCol = Collider();			//�U������̋�
			VECTOR3 forward = VECTOR3(0, 0.5f, 1.0f); // ��]���ĂȂ����̈ړ���
			MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
			forward = forward * rotY; // �L�����̌����Ă���ւ̈ړ���
			atkCol.center = transform.position + forward; //�U������̋������
			atkCol.radius = 1.0f;
			VECTOR3 pushVec = atkCol.center - dCol.center;
			float rSum = atkCol.radius + dCol.radius;
			if (pushVec.LengthSquare() < rSum * rSum) { // ���̓����蔻��
				// �������Ă�
				d->AddDamage(1); //�_���[�W��^����
			}
		}
	}

	if (animator->Finished())
	{
		//�U���A�j���[�V�����̏I��
		doneAtkAnim = false;
		animator->SetPlaySpeed(1.0f);
		state = sOnGround;
	}
}

void Player::UpdateAttack3()
{
}

//�@�v���C���[�̎����
Broom::Broom(Object3D* parentModel)
{
	parent = parentModel;
//	transform.SetParent(parent); // tansform�ɂ��e��������

	mesh = new CFbxMesh();
	mesh->Load("data/Player2/weapon/broom.mesh");
	
	transform.position = VECTOR3(0, 0, 0);
}

Broom::~Broom()
{
}

void Broom::Update()
{
	Player* pl = ObjectManager::FindGameObject<Player>();

	// ��Ԃ��Ƃ̊p�x�ύX
	switch (pl->getPlayerState()) {
	case 0:
	case 1:
		transform.position = VECTOR3(0, 0, 0);
		transform.rotation = VECTOR3(0, 0, -70);
		break;
	case 2:
	case 3:
	default:
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
