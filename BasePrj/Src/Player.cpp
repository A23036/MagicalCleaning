#include "Player.h"
#include "../Libs/Imgui/imgui.h"
#include "Stage.h"
#include "Camera.h"
#include "Dust.h"

namespace { // ����cpp�ȊO�ł͎g���Ȃ�
	static const float Gravity = 0.005f; // �d�͉����x(���̒l)
	static const float JumpPower = 0.1f;
	static const float RotationSpeed = 3.0f; // ��]���x(�x)
	static const float MoveSpeed = 0.1f;
};

Player::Player()
{
	animator = new Animator(); // �C���X�^���X���쐬

	mesh = new CFbxMesh();
	//ToDo: ���f���ύX
	//mesh->Load("Data/Player/witch.mesh");
	//mesh->LoadAnimation(aIdle, "Data/Player/witch_idle.anmx", true);
	mesh->Load("Data/Player/PlayerChara.mesh");
	mesh->LoadAnimation(aIdle, "Data/Player/Idle.anmx", true);
	mesh->LoadAnimation(aRun, "Data/Player/Run.anmx", true);
	mesh->LoadAnimation(aAttack1, "Data/Player/attack1.anmx", false);
	mesh->LoadAnimation(aAttack2, "Data/Player/attack2.anmx", false);
	mesh->LoadAnimation(aAttack3, "Data/Player/attack3.anmx", false);

	animator->SetModel(mesh); // ���̃��f���ŃA�j���[�V��������
	animator->Play(aIdle);
	animator->SetPlaySpeed(1.0f);

	transform.position = VECTOR3(0, 0, 0);
	transform.rotation = VECTOR3(0, 0, 0);
	state = sOnGround;
	speedY = 0;
	MP = 0;
	doneAtk = false;

	moveSpeed	= 1;
	jumpNum		= 1;
	atkSpeed	= 1;
	atkRange	= 1;
	carWeight	= 1;
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

	ImGui::SetNextWindowPos(ImVec2(100, 0));
	ImGui::SetNextWindowSize(ImVec2(180, 60));
	ImGui::Begin("JumpSpeed");
	ImGui::InputFloat("speedY", &speedY);
	ImGui::End();
	
	ImGui::SetNextWindowPos(ImVec2(0, 60));
	ImGui::SetNextWindowSize(ImVec2(100, 100));
	ImGui::Begin("PlayerPos");
	ImGui::InputFloat("X", &transform.position.x);
	ImGui::InputFloat("Y", &transform.position.y);
	ImGui::InputFloat("Z", &transform.position.z);
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
	

	// Dancer�ɂ߂荞�܂Ȃ��悤�ɂ���
	// �����̍��W�́Atransform.position
	// Dancer�̍��W��m��
	//std::list<Dancer*> dancers =
	//	ObjectManager::FindGameObjects<Dancer>();
	// �@�C�e���[�^�[��
	//for (std::list<Dancer*>::iterator itr = dancers.begin();
	//				itr != dancers.end(); itr++) {
	//	Dancer* dancer = *itr;
	// �A�C�e���[�^�̕ύX
	//for (auto itr = dancers.begin(); itr != dancers.end(); itr++) {
	//	Dancer* dancer = *itr;
	// �Bfor(:)�ŉ�
	/*
	for (Dancer* dancer : dancers) {
		// �ȏ�R�o�[�W����
		SphereCollider tCol = dancer->Collider();
		SphereCollider pCol = Collider();
		VECTOR3 pushVec = pCol.center - tCol.center;
		float rSum = pCol.radius + tCol.radius;
		if (pushVec.LengthSquare() < rSum * rSum) { // ���̓����蔻��
			// �������Ă�
			// �����o��������pushVec
			// �����o�����������߂�
			float pushLen = rSum - pushVec.Length();
			pushVec = XMVector3Normalize(pushVec); // pushVec�̒������P�ɂ���
			transform.position += pushVec * pushLen;
		}
	}
	*/
}
	
void Player::Draw()
{
	Object3D::Draw(); // �p�����̊֐����Ă�

	//MATRIX4X4 tip = XMMatrixRotationRollPitchYawFromVector(VECTOR3(-33, 82, 0) * DegToRad);
	//VECTOR3 tipPos = VECTOR3(0, 0, 1.2f) * tip;
	VECTOR3 tipPos = VECTOR3(0.9966, 0.6536, 0.140);
	MATRIX4X4 mat = transform.matrix();// ���E(���[���h)�̒��ŁA�v���C���[�̈ʒu�ƌ���
	MATRIX4X4 bone = mesh->GetFrameMatrices(34); // �v���C���[�̌��_����̎��̈ʒu(34�͎��)
	VECTOR3 start = VECTOR3(0, 0, 0) * bone * mat;
	VECTOR3 end = tipPos * bone * mat;

	CSprite spr;
	spr.DrawLine3D(start, end, RGB(255, 0, 0));
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
	MP += n;
}


void Player::UpdateOnGround()
{
	auto di = GameDevice()->m_pDI;
	Stage* st = ObjectManager::FindGameObject<Stage>();
	if (!(st->IsLandBlock(transform.position))) {
		// ��
		state = sJump;
	}
	if (di->CheckKey(KD_DAT, DIK_W)) {
		// �s��ł��ꍇ
		VECTOR3 forward = VECTOR3(0, 0, MoveSpeed); // ��]���ĂȂ����̈ړ���
		MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
		VECTOR3 move = forward * rotY; // �L�����̌����Ă���ւ̈ړ���
		transform.position += move;
		animator->MergePlay(aRun);
	}
	else if (di->CheckKey(KD_DAT, DIK_S)) {
		// �s��ł��ꍇ
		VECTOR3 forward = VECTOR3(0, 0, MoveSpeed); // ��]���ĂȂ����̈ړ���
		MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
		VECTOR3 move = forward * rotY; // �L�����̌����Ă���ւ̈ړ���
		transform.position -= move;
		animator->MergePlay(aRun);
	}
	else {
		animator->MergePlay(aIdle);
	}
	if (di->CheckKey(KD_DAT, DIK_A)) {
		transform.rotation.y -= RotationSpeed / 180.0f * XM_PI;
	}
	if (di->CheckKey(KD_DAT, DIK_D)) {
		transform.rotation.y += RotationSpeed / 180.0f * XM_PI;
	}
	if (di->CheckKey(KD_TRG, DIK_SPACE)) {
		speedY = JumpPower;
		state = sJump;
	}
	if (di->CheckKey(KD_TRG, DIK_N)) { //�U���{�^��
		animator->MergePlay(aAttack1);
		animator->SetPlaySpeed(2.5);
		state = sAttack1;
	}
	if (di->CheckKey(KD_TRG, DIK_M)) { //�U���{�^��
		animator->MergePlay(aAttack2);
		state = sAttack2;
	}

}


void Player::UpdateJump()
{
	Stage* st = ObjectManager::FindGameObject<Stage>();
	//Stage* cam = ObjectManager::FindGameObject<>();
	VECTOR3 bufPos = transform.position;
	transform.position.y += speedY;
	speedY -= Gravity;	// �d��

	
	if (GameDevice()->m_pDI->CheckKey(KD_DAT, DIK_W)) {
		// �s��ł��ꍇ
		VECTOR3 forward = VECTOR3(0, 0, MoveSpeed); // ��]���ĂȂ����̈ړ���
		MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
		VECTOR3 move = forward * rotY; // �L�����̌����Ă���ւ̈ړ���
		transform.position += move;
		animator->MergePlay(aRun);
	}
	else if (GameDevice()->m_pDI->CheckKey(KD_DAT, DIK_S)) {
		// �s��ł��ꍇ
		VECTOR3 forward = VECTOR3(0, 0, -MoveSpeed); // ��]���ĂȂ����̈ړ���
		MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
		VECTOR3 move = forward * rotY; // �L�����̌����Ă���ւ̈ړ���
		transform.position += move;
		animator->MergePlay(aRun);
	}
	else {
		animator->MergePlay(aIdle);
	}
	if (GameDevice()->m_pDI->CheckKey(KD_DAT, DIK_A)) {
		transform.rotation.y -= RotationSpeed / 180.0f * XM_PI;
	}
	if (GameDevice()->m_pDI->CheckKey(KD_DAT, DIK_D)) {
		transform.rotation.y += RotationSpeed / 180.0f * XM_PI;
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
	/*
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
	
	if (!doneAtk && animator->CurrentFrame() == 40) { //�U���̃q�b�g�����^�C�~���O
		doneAtk = true;
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
		doneAtk = false;
		animator->SetPlaySpeed(1.0);
		state = sOnGround;
	}

}

void Player::UpdateAttack2()
{
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
	}
}

void Player::UpdateAttack3()
{
}
