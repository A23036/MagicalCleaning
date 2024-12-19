#include "Player.h"
#include "../Libs/Imgui/imgui.h"
#include <dinput.h>
#include "CsvReader.h"
#include "Stage.h"
#include "Camera.h"
#include "Dust.h"
#include "DustBox.h"
#include "SlashEffect.h"
#include "PowerUpEffect.h"
#include "LeafEffect.h"
#include "JumpEffect.h"

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
	ObjectManager::SetDrawOrder(this, -100);

	CsvLoad(); // csv����f�[�^�̐ݒ�

	// �v���C���[�̎���ⴂ̐���
	child = new Broom(this,playerNum);

	animator = new Animator(); // �C���X�^���X���쐬

	mesh = new CFbxMesh();

	transform.position = VECTOR3(0, 0, 0);
	transform.rotation = VECTOR3(0, 0, 0);
	deltaTime = 0.0f;

	state = sOnGround;
	prevState = sOnGround;

	speedY = 0;
	score = 0;
	leaf = 0;
	mp = 0;
	weight = 0;
	jumpCount = 0;
	chargeSpeed = 0;
	chargeFrm = 0;
	invisibleTime = 0;

	isDash = false;
	isFly = false;
	isBlow = false;
	canFly = false;
	finishAtkAnim = false;
	atkComboFlg = false;
	isMagicReady = false;
	isInvisible = false;

	blowVec = VECTOR3(0, 0, 0);

	comboWaitFrm = 20;

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
	anmFrame = 0;
}

Player::~Player()
{
	SAFE_DELETE(mesh);
	SAFE_DELETE(animator);
	SAFE_DELETE(csv);
}

void Player::Start()
{
	//���g�ȊO�̃v���C���[�����X�g�Ɋi�[
	for (int i = 0; i < MAXPLAYER; i++) {
		if (i == playerNum) {
			continue;
		}
		std::string s = "Player";
		s = s + std::to_string(i + 1);
		Player* pl = ObjectManager::FindGameObjectWithTag<Player>(s);
		otherPlayers.push_back(pl);
	}
	
	dc = ObjectManager::FindGameObject<DataCarrier>();
	st = ObjectManager::FindGameObject<Stage>();
	cm = ObjectManager::FindGameObject<Camera>();

	std::string f = "Data/Player/Color" + std::to_string(color);
	mesh->Load((f + "/witch.mesh").c_str());

	mesh->LoadAnimation(aIdle, (f + "/idle.anmx").c_str(), true);
	mesh->LoadAnimation(aRun, (f + "/run.anmx").c_str(), true);
	mesh->LoadAnimation(aJump, (f + "/jump.anmx").c_str(), false);
	mesh->LoadAnimation(aJump2, (f + "/jump2.anmx").c_str(), false);
	mesh->LoadAnimation(aFly, (f + "/fly.anmx").c_str(), true);
	mesh->LoadAnimation(aFall, (f + "/fall.anmx").c_str(), true);
	mesh->LoadAnimation(aAttack1, (f + "/attack1.anmx").c_str(), false);
	mesh->LoadAnimation(aAttack2, (f + "/attack2.anmx").c_str(), false);
	mesh->LoadAnimation(aAttack3, (f + "/attack3.anmx").c_str(), false);
	mesh->LoadAnimation(aChargeReady, (f + "/chargeReady.anmx").c_str(), false);
	mesh->LoadAnimation(aCharge, (f + "/charge.anmx").c_str(), true);
	mesh->LoadAnimation(aBlow, (f + "/blow.anmx").c_str(), true);

	animator->SetModel(mesh); // ���̃��f���ŃA�j���[�V��������
	animator->Play(aIdle);
	animator->SetPlaySpeed(1.0f);
}

void Player::Update()
{
	deltaTime = 60 * SceneManager::DeltaTime();
	posOld = transform.position;    // -- 2024.12.2

	// ⴂ̈ʒu���X�V
	if (state != sStandby) { //�L�����Z���N�g��ʂł͎����Ȃ�

		MATRIX4X4 bone;
		if (state == sAttack2) {
			bone = mesh->GetFrameMatrices(18);// �v���C���[�̌��_����̍���̈ʒu(18�͍���)
		}
		else {
			bone = mesh->GetFrameMatrices(0);// �v���C���[�̌��_����̉E��̈ʒu(0�͉E��)
		}
		if (isDash || isFly) {
			bone = mesh->GetFrameMatrices(2);//�v���C���[�̃��[�g�ʒu
		}
		
		child->SetPos(bone);
	}
	
	/*
	ImGui::SetNextWindowPos(ImVec2(600, 0));
	ImGui::SetNextWindowSize(ImVec2(100, 120));
	ImGui::Begin("Player");
	if (ImGui::Button("DataLoad")) {
		CsvLoad();
	}
	ImGui::End();
	
	
	ImGui::SetNextWindowPos(ImVec2(0, 50));
	ImGui::SetNextWindowSize(ImVec2(100, 160));
	ImGui::Begin("state");
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
	case sAttack3:
		ImGui::Text("sAttack3");
		break;
	case sStop:
		ImGui::Text("sStop");
		break;
	}
	ImGui::End();
	*/

	if (dc->GetIsPlay()) {
		animator->Update();
		state = prevState;
	}
	else
	{
		state = sStop;
	}

	switch (state) {
	case sStandby:
	case sStop:
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
		UpdateAttack2();
		break;
	case sAttack3:
		UpdateAttack3();
		break;
	case sCharge:
		UpdateCharge();
		break;
	case sBlow:
		UpdateBlow();
		break;
	default:
		break;
	}
	
	auto di = GameDevice()->m_pDI;
	if ((di->CheckKey(KD_TRG, DIK_UP) && playerNum == 0) || di->CheckJoy(KD_TRG, 2, playerNum)) { //�\�͋���
		switch (selectPower) {
		case pMS:
			if (mp >= MoveSpeedC[msNum] && msNum < MsTableNum - 1)
			{
				mp -= MoveSpeedC[msNum];
				msNum++;
				moveSpeed = MoveSpeedT[msNum];
				new PowerUpEffect(this,transform.position,selectPower);
			}
			break;
		case pJN:
			if (mp >= JumpNumC[jnNum] && jnNum < JnTableNum - 1)
			{
				mp -= JumpNumC[jnNum];
				jnNum++;
				new PowerUpEffect(this, transform.position, selectPower);
				if (jnNum == JnTableNum - 1) {
					canFly = true;
				}
				else {
					jumpNum = JumpNumT[jnNum];
				}
			}
			break;
		case pAS:
			if (mp >= AtkSpeedC[asNum] && asNum < AsTableNum - 1)
			{
				mp -= AtkSpeedC[asNum];
				asNum++;
				atkSpeed = AtkSpeedT[asNum];
				new PowerUpEffect(this, transform.position, selectPower);
			}
			break;
		case pAR:
			if (mp >= AtkRangeC[arNum] && arNum < ArTableNum - 1)
			{
				mp -= AtkRangeC[arNum];
				arNum++;
				atkRange = AtkRangeT[arNum];
				new PowerUpEffect(this, transform.position, selectPower);
			}
			break;
		case pCW:
			if (mp >= CarWeightC[cwNum] && cwNum < CwTableNum - 1)
			{
				mp -= CarWeightC[cwNum];
				cwNum++;
				carWeight = CarWeightT[cwNum];
				new PowerUpEffect(this, transform.position, selectPower);
			}
			break;
		}
	}
	
	
	if ((di->CheckKey(KD_TRG, DIK_LEFT) && playerNum == 0) || di->CheckJoy(KD_TRG, 4, playerNum)) { //�����\�͕ύX
		selectPower--;
		if (selectPower < 0) {
			selectPower = 4;
		}
	}
	if ((di->CheckKey(KD_TRG, DIK_RIGHT) && playerNum == 0) || di->CheckJoy(KD_TRG, 5, playerNum)) { //�����\�͕ύX
		selectPower++;
		if (selectPower > 4) {
			selectPower = 0;
		}
	}



	//�����蔻�菈��
	//VECTOR3 push;
	
	//if (st->HitSphereToMesh(Collider(), &push)) {
	//	transform.position += push;
	//}

	//��O����
	if (transform.position.y <= -30.0f) {
		transform.position = VECTOR3(0, 20, 0);
		ResetLeaf();
	}

	//�����蔻�菈�� // -- 2024.12.2
	//�X�e�[�W
	st->MapCol()->IsCollisionMoveGravity(posOld, transform.position);

	//�������
	transform.position += blowVec;

	//������
	if (isInvisible) {
		mesh->m_vDiffuse = VECTOR4(1, 1, 1, 0.2f);
		invisibleTime++;
		if (invisibleTime * (1.0f / 60.0f) > InvisibleTime) {
			invisibleTime = 0;
			isInvisible = false;
			mesh->m_vDiffuse = VECTOR4(1, 1, 1, 1);
		}
	}

	// ImGui�E�B���h�E�̈ʒu�ƃT�C�Y��ݒ�
	/*
	ImGui::SetNextWindowPos(ImVec2(0, 60));
	ImGui::SetNextWindowSize(ImVec2(120, 400));
	ImGui::Begin("PlayerPos");
	ImGui::InputFloat("X", &transform.position.x);
	ImGui::InputFloat("Y", &transform.position.y);
	ImGui::InputFloat("Z", &transform.position.z);
	ImGui::InputFloat("speedY", &speedY);
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
	/*
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
	*/
	// player�ɂ߂荞�܂Ȃ��悤�ɂ���
	// �����̍��W�́Atransform.position
	// player�̍��W��m��
	
	for (Player* player : otherPlayers) {
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
	
	prevState = state;
}
	
void Player::Draw()
{
	if (ObjectManager::DrawCounter() != playerNum) { //�����̃v���C���[��`�悵�Ȃ�
		if (isInvisible) {
			return;
		}
	}
	Object3D::Draw(); // �p�����̊֐����Ă�
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
			if (csv->GetString(i, 1) == "InvisibleTime") {		// ����������
				InvisibleTime = csv->GetInt(i, 3);
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
	col.center = transform.position + VECTOR3(0, 0.58f, 0);
	col.radius = 0.58f;
	return col;
}

int Player::GetPowerCost(int selectPower)
{ 
	switch (selectPower) {
	case pMS:
		return MoveSpeedC[msNum];
	case pJN:
		return JumpNumC[jnNum];
	case pAS:
		return AtkSpeedC[asNum];
	case pAR:
		return AtkRangeC[arNum];
	case pCW:
		return CarWeightC[cwNum];
	default:
		return 0;
	}
}

int Player::GetPowerLv(int selectPower)
{
	switch (selectPower) {
	case pMS:
		return msNum;
	case pJN:
		return jnNum;
	case pAS:
		return asNum;
	case pAR:
		return arNum;
	case pCW:
		return cwNum;
	default:
		return 0;
	}
}

int Player::GetMaxPowerLv(int selectPower)
{
	switch (selectPower) {
	case pMS:
		return MsTableNum;
	case pJN:
		return JnTableNum;
	case pAS:
		return AsTableNum;
	case pAR:
		return ArTableNum;
	case pCW:
		return CwTableNum;
	default:
		return 0;
	}
}

void Player::SetPlayerColor(int color)
{
	this->color = color;
}

void Player::SetPlayerState(int state)
{
	this->state = state;
}

void Player::SetPlayerPrevState(int state)
{
	prevState = state;
}

void Player::SetBlowVec(VECTOR3 vec)
{
	blowVec = vec;
}

void Player::SetSpeedY(float y)
{
	speedY = y;
}

void Player::SetIsBlow(bool isBlow)
{
	isDash = false; 
	isFly = false;
	finishAtkAnim = false;
	if (mcEffect != nullptr) {
		mcEffect->SetIsFinish();
	}
	this->isBlow = isBlow;
}

void Player::ResetLeaf()
{
	leaf = 0;
}

void Player::AddLeaf(int n)
{
	if (leaf + n > carWeight) { //��������܂ł������Z���Ȃ�
		leaf = carWeight;
	}
	else {
		leaf += n;
	}
}

void Player::AddMP(int n)
{
	mp += n;
}

void Player::AddWeight(int n)
{
	weight += n;

}

void Player::AddScore(int n)
{
	score += n;
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
	
	if (st->MapCol()->IsCollisionMoveGravity(posOld, transform.position) == clFall) {
		state = sJump;
		return;
	}

	/*
	VECTOR3 start = transform.position;
	start.y += 1.0f;
	VECTOR3 end = transform.position;
	if (!HitLineToMesh(start, end)) {
		state = sJump;
		return;
	}
	*/   

	// 2024.10.26 �v���C���[������R���g���[���[�ɑΉ���
	
	// �R���g���[���[�X�e�B�b�N�̓��͂��擾
	// ���K������ -1.0�`1.0 �͈̔͂ɂ���
	float ix = di->GetJoyState(playerNum).lX / 1000.0f;
	float iy = di->GetJoyState(playerNum).lY / 1000.0f;
	
	// �L�[�{�[�h������擾(1P�̂ݓ�������)
	if (di->CheckKey(KD_DAT, DIK_W) && playerNum == 0) {
		iy = -1.0f; // �O�i
	}
	if (di->CheckKey(KD_DAT, DIK_S) && playerNum == 0) {
		iy = 1.0f;  // ���
	}
	if (di->CheckKey(KD_DAT, DIK_A) && playerNum == 0) {
		ix = -1.0f; // ���ړ�
	}
	if (di->CheckKey(KD_DAT, DIK_D) && playerNum == 0) {
		ix = 1.0f;  // �E�ړ�
	}

	// �X�e�B�b�N�����͂���Ă��邩�m�F
	if (fabs(ix) > 0.1f || fabs(iy) > 0.1f) {
		// ���͂Ɋ�Â��Ĉړ�������ݒ�
		VECTOR3 inputDirection = VECTOR3(ix, 0, -iy); // Z�������͔��]

		// �J������Y����]���擾
		float cameraYRotation = cm->GetRotY(playerNum);

		// �J�����̉�]�Ɋ�Â��ړ��x�N�g���̌v�Z
		MATRIX4X4 cameraRotY = XMMatrixRotationY(cameraYRotation);
		VECTOR3 moveDirection = XMVector3TransformNormal(inputDirection, cameraRotY);
		if (sqrt(ix * ix + iy * iy) > 1.0f) {
			moveDirection = XMVector3Normalize(moveDirection) * MOVE_SPEED; // �΂߈ړ������i��葁���Ȃ�Ȃ��悤�␳
		}
		else {
			moveDirection = XMVector3Normalize(moveDirection) * MOVE_SPEED * sqrt(ix * ix + iy * iy); // �X����ɉ��������x
		}
		
		// �ړ��̓K�p
		transform.position += moveDirection * moveSpeed * deltaTime;

		// �v���C���[�̉�]���J������ŕ�������������
		transform.rotation.y = cameraYRotation + atan2(ix, -iy); // �J�����̉�]�ɑ΂��ăX�e�B�b�N�̕����ɍ��킹��

		if (sqrt(ix * ix + iy * iy) * moveSpeed > 2.0) {
			// ��s�A�j���[�V�������Đ�
			animator->MergePlay(aFly, 0);
			animator->SetPlaySpeed(1.0f);
			isDash = true;
		}
		else {
			//���s���x�ɉ������A�j���[�V�����X�s�[�h��ݒ�
			animator->SetPlaySpeed(sqrt(ix * ix + iy * iy) * moveSpeed);

			// ���s�A�j���[�V�������Đ�
			animator->MergePlay(aRun, 0);
			isDash = false;
		}
	}
	else {
		// �X�e�B�b�N���X���Ă��Ȃ��ꍇ�͑ҋ@�A�j���[�V�������Đ�
		animator->SetPlaySpeed(1.0f);
		animator->MergePlay(aIdle,0);
		isDash = false;
	}
	// 2024.10.26 �v���C���[������R���g���[���[�ɑΉ���

	if ((di->CheckKey(KD_TRG, DIK_SPACE) && playerNum == 0) || di->CheckJoy(KD_TRG, 1, playerNum) ) { //�W�����v
		speedY = JUMP_POWER;
		if (!isDash) {
			animator->MergePlay(aJump, 0);
			animator->SetPlaySpeed(1.0f);
		}
		jumpCount++;
		state = sJump;
	}
	if ((di->CheckKey(KD_TRG, DIK_N) && playerNum == 0) || di->CheckJoy(KD_TRG, 0, playerNum)) { //�U��
		animator->MergePlay(aAttack1,0);
		animator->SetPlaySpeed(1.0f * atkSpeed);
		transform.rotation.y += 15 * DegToRad; //���ʕ����ɉ�]������
		state = sAttack1;
		isDash = false;
		anmFrame = 0;
	}
	if ((di->CheckKey(KD_TRG, DIK_M) && playerNum == 0) || di->CheckJoy(KD_TRG, 3, playerNum)) { //MP�ϊ�
		animator->MergePlay(aChargeReady);
		animator->SetPlaySpeed(1.0f);
		state = sCharge;
		isDash = false;
		chargeSpeed = 1.0f; //MP�ϊ����x�����l
		mcEffect = new MagicCircleEffect(transform.position, playerNum);
	}
}


void Player::UpdateJump()
{
	transform.position.y += speedY * deltaTime;
	if (canFly && GameDevice()->m_pDI->CheckJoy(KD_DAT, 7, playerNum)) {
		speedY = -0.01;	// �d��
		isFly = true;
		if (animator->PlayingID() == aRun || animator->PlayingID() == aFall) {
			animator->MergePlay(aFly);
		}
	}
	else {
		if (animator->PlayingID() == aFly && !isDash) {
			animator->MergePlay(aFall);
		}
		speedY -= GRAVITY * deltaTime;	// �d��
		isFly = false;
	}

	auto di = GameDevice()->m_pDI;
	// �R���g���[���[�X�e�B�b�N�̓��͂��擾
	// ���K������ -1.0�`1.0 �͈̔͂ɂ���
	float ix = di->GetJoyState(playerNum).lX / 1000.0f;
	float iy = di->GetJoyState(playerNum).lY / 1000.0f;

	// �L�[�{�[�h������擾(1P�̂ݓ�������)
	if (di->CheckKey(KD_DAT, DIK_W) && playerNum == 0) {
		iy = -1.0f; // �O�i
	}
	if (di->CheckKey(KD_DAT, DIK_S) && playerNum == 0) {
		iy = 1.0f;  // ���
	}
	if (di->CheckKey(KD_DAT, DIK_A) && playerNum == 0) {
		ix = -1.0f; // ���ړ�
	}
	if (di->CheckKey(KD_DAT, DIK_D) && playerNum == 0) {
		ix = 1.0f;  // �E�ړ�
	}

	// �X�e�B�b�N�����͂���Ă��邩�m�F
	if (fabs(ix) > 0.1f || fabs(iy) > 0.1f) {
		// ���͂Ɋ�Â��Ĉړ�������ݒ�
		VECTOR3 inputDirection = VECTOR3(ix, 0, -iy); // Z�������͔��]

		// �J������Y����]���擾
		float cameraYRotation = cm->GetRotY(playerNum);

		// �J�����̉�]�Ɋ�Â��ړ��x�N�g���̌v�Z
		MATRIX4X4 cameraRotY = XMMatrixRotationY(cameraYRotation);
		VECTOR3 moveDirection = XMVector3TransformNormal(inputDirection, cameraRotY);
		if (sqrt(ix * ix + iy * iy) > 1.0f) {
			moveDirection = XMVector3Normalize(moveDirection) * MOVE_SPEED; // �΂߈ړ������i��葁���Ȃ�Ȃ��悤�␳
		}
		else {
			moveDirection = XMVector3Normalize(moveDirection) * MOVE_SPEED * sqrt(ix * ix + iy * iy); // �X����ɉ��������x
		}
		// �ړ��̓K�p
		transform.position += moveDirection * moveSpeed * deltaTime;

		// �v���C���[�̉�]���J������ŕ�������������
		transform.rotation.y = cameraYRotation + atan2(ix, -iy); // �J�����̉�]�ɑ΂��ăX�e�B�b�N�̕����ɍ��킹��
	}

	if (di->CheckJoy(KD_TRG, 1, playerNum) && jumpCount <= jumpNum) {
		speedY = JUMP_POWER;
		isDash = false;
		if (jumpCount % 2 == 0) {
			animator->MergePlay(aJump); //�W�����v�A�j���[�V����1
		}
		else {
			animator->MergePlay(aJump2);//�W�����v�A�j���[�V����2
		}
		animator->SetPlaySpeed(1.0f);
		new JumpEffect(transform.position);
		jumpCount++;
	}


	if (animator->Finished()) {
		if (isFly) {
			animator->MergePlay(aFly);
		}
		else
		{
			animator->MergePlay(aFall);
		}
		
	}

	if (st->MapCol()->IsCollisionMoveGravity(posOld, transform.position) != clFall) {
		// �W�����v�I��
		isFly = false;
		state = sOnGround;
		jumpCount = 0;
		speedY = 0;
		blowVec = VECTOR3(0, 0, 0);
	}
}

void Player::UpdateAttack1()
{	
	anmFrame++;

	// �S�~�ɍU���𓖂Ă�
	std::list<Dust*> dusts = ObjectManager::FindGameObjects<Dust>();
	
	if (!finishAtkAnim && animator->CurrentFrame() >= 20.0f/atkSpeed) { //�U���̃q�b�g�����^�C�~���O
		finishAtkAnim = true;
		//�G�t�F�N�g�̍Đ�
		VECTOR3 forward = VECTOR3(0, 0, 1.1f*atkRange); // ��]���ĂȂ����̈ړ���
		MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
		forward = forward * rotY; // �L�����̌����Ă���ւ̈ړ���
		VECTOR3 pos = forward + transform.position;
		new SlashEffect(pos,atkRange);

		for (Dust* d : dusts) {
			SphereCollider dCol = d->Collider(d->GetNum()); //�S�~�̔��苅
			SphereCollider atkCol = Collider();			//�U������̋�
			
			atkCol.center = transform.position + forward; //�U������̋������
			atkCol.radius = 1.0f * atkRange;
			VECTOR3 pushVec = atkCol.center - dCol.center;
			float rSum = atkCol.radius + dCol.radius;
			if (pushVec.LengthSquare() < rSum * rSum) { // ���̓����蔻��
				// �������Ă�
				d->AddDamage(this,1); //�_���[�W��^����
				if (d->GetNum() == 3) { //������
					invisibleTime = 0;
					isInvisible = true;
				}
			}
		}
		//���v���C���[�ւ̍U������
		for (Player* p : otherPlayers) {
			SphereCollider pCol = p->Collider(); //���v���C���[�̔��苅
			SphereCollider atkCol = Collider();		//�U������̋�
			atkCol.center = transform.position + forward; //�U������̋������
			atkCol.radius = 1.0f * atkRange;
			VECTOR3 pushVec = pCol.center - atkCol.center;
			
			float rSum = atkCol.radius + pCol.radius;
			if (pushVec.LengthSquare() < rSum * rSum) { // ���̓����蔻��
				// �������Ă�
				pushVec = XMVector3Normalize(pushVec);
				pushVec *= 0.1f;
				pushVec.y = 0.201f;
				p->SetBlowVec(pushVec);
				p->SetSpeedY(pushVec.y);
				p->SetPlayerState(sBlow);
				p->SetPlayerPrevState(sBlow);
				p->SetIsBlow(true);
				p->animator->MergePlay(aBlow, 0);
				p->animator->SetPlaySpeed(2.0f);
				AddLeaf(p->GetLeaf());
				p->ResetLeaf();
				new LeafEffect(p->Position(), VECTOR3(1, 1, 1), p->GetLeaf());
			}
		}
	}
	
	auto di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_TRG, DIK_N) || di->CheckJoy(KD_TRG, 0, playerNum)) { //�U�����̐�s���͎�t
		atkComboFlg = true;
		anmFrame = 0;
	}

	if (anmFrame >= comboWaitFrm) { //�A�����͌��莞�ԂŘA���U���t���O����
		atkComboFlg = false;
	}

	if (animator->Finished())
	{
		//�U���A�j���[�V�����̏I��
		finishAtkAnim = false;
		animator->SetPlaySpeed(1.0f);
		transform.rotation.y -= 15 * DegToRad; //��]��߂�
		if (atkComboFlg) { //�A���U��
			animator->MergePlay(aAttack2, 0);
			animator->SetPlaySpeed(1.0f * atkSpeed);
			transform.rotation.y += 15 * DegToRad; //���ʕ����ɉ�]������
			state = sAttack2;
			anmFrame = 0;
		}
		else {
			//�n���Ԃ�
			state = sOnGround;
		}
		atkComboFlg = false;
	}
	
}

void Player::UpdateAttack2()
{
	anmFrame++;

	// �S�~�ɍU���𓖂Ă�
	std::list<Dust*> dusts = ObjectManager::FindGameObjects<Dust>();

	if (!finishAtkAnim && animator->CurrentFrame() >= 20.0f / atkSpeed) { //�U���̃q�b�g�����^�C�~���O
		finishAtkAnim = true;
		//�G�t�F�N�g�̍Đ�
		VECTOR3 forward = VECTOR3(0, 0, 1.1f * atkRange); // ��]���ĂȂ����̈ړ���
		MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
		forward = forward * rotY; // �L�����̌����Ă���ւ̈ړ���
		VECTOR3 pos = forward + transform.position;
		new SlashEffect(pos, atkRange);

		for (Dust* d : dusts) {
			SphereCollider dCol = d->Collider(d->GetNum()); //�S�~�̔��苅
			SphereCollider atkCol = Collider();			//�U������̋�

			atkCol.center = transform.position + forward; //�U������̋������
			atkCol.radius = 1.0f * atkRange;
			VECTOR3 pushVec = atkCol.center - dCol.center;
			float rSum = atkCol.radius + dCol.radius;
			if (pushVec.LengthSquare() < rSum * rSum) { // ���̓����蔻��
				// �������Ă�
				d->AddDamage(this, 1); //�_���[�W��^����
				if (d->GetNum() == 3) { //������
					invisibleTime = 0;
					isInvisible = true;
				}
			}
		}
		//���v���C���[�ւ̍U������
		for (Player* p : otherPlayers) {
			SphereCollider pCol = p->Collider(); //���v���C���[�̔��苅
			SphereCollider atkCol = Collider();		//�U������̋�
			atkCol.center = transform.position + forward; //�U������̋������
			atkCol.radius = 1.0f * atkRange;
			VECTOR3 pushVec = pCol.center - atkCol.center;

			float rSum = atkCol.radius + pCol.radius;
			if (pushVec.LengthSquare() < rSum * rSum) { // ���̓����蔻��
				// �������Ă�
				pushVec = XMVector3Normalize(pushVec);
				pushVec *= 0.1f;
				pushVec.y = 0.201f;
				p->SetBlowVec(pushVec);
				p->SetSpeedY(pushVec.y);
				p->SetPlayerState(sBlow);
				p->SetPlayerPrevState(sBlow);
				p->SetIsBlow(true);
				p->animator->MergePlay(aBlow, 0);
				p->animator->SetPlaySpeed(2.0f);
				AddLeaf(p->GetLeaf());
				p->ResetLeaf();
				new LeafEffect(p->Position(), VECTOR3(1, 1, 1), p->GetLeaf());
			}
		}
	}

	auto di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_TRG, DIK_N) || di->CheckJoy(KD_TRG, 0, playerNum)) { //�U�����̐�s���͎�t
		atkComboFlg = true;
		anmFrame = 0;
	}

	if (anmFrame >= comboWaitFrm) { //�A�����͌��莞�ԂŘA���U���t���O����
		atkComboFlg = false;
	}

	if (animator->Finished())
	{
		//�U���A�j���[�V�����̏I��
		finishAtkAnim = false;
		animator->SetPlaySpeed(1.0f);
		transform.rotation.y -= 15 * DegToRad; //��]��߂�
		if (atkComboFlg) { //�A���U��
			animator->MergePlay(aAttack3, 0);
			animator->SetPlaySpeed(1.0f * atkSpeed);
			//transform.rotation.y += 15 * DegToRad; //���ʕ����ɉ�]������
			state = sAttack3;
			anmFrame = 0;
		}
		else {
			//�n���Ԃ�
			state = sOnGround;
		}
		atkComboFlg = false;
	}
}

void Player::UpdateAttack3()
{
	// �S�~�ɍU���𓖂Ă�
	std::list<Dust*> dusts = ObjectManager::FindGameObjects<Dust>();

	if (!finishAtkAnim && animator->CurrentFrame() >= 30.0f / atkSpeed) { //�U���̃q�b�g�����^�C�~���O
		finishAtkAnim = true;
		//�G�t�F�N�g�̍Đ�
		VECTOR3 forward = VECTOR3(0, 0, 1.1f * atkRange); // ��]���ĂȂ����̈ړ���
		MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
		forward = forward * rotY; // �L�����̌����Ă���ւ̈ړ���
		VECTOR3 pos = forward + transform.position;
		new SlashEffect(pos, atkRange);

		for (Dust* d : dusts) {
			SphereCollider dCol = d->Collider(d->GetNum()); //�S�~�̔��苅
			SphereCollider atkCol = Collider();			//�U������̋�

			atkCol.center = transform.position + forward; //�U������̋������
			atkCol.radius = 1.0f * atkRange;
			VECTOR3 pushVec = atkCol.center - dCol.center;
			float rSum = atkCol.radius + dCol.radius;
			if (pushVec.LengthSquare() < rSum * rSum) { // ���̓����蔻��
				// �������Ă�
				d->AddDamage(this, 1); //�_���[�W��^����
				if (d->GetNum() == 3) { //������
					invisibleTime = 0;
					isInvisible = true;
				}
			}
		}
		//���v���C���[�ւ̍U������
		for (Player* p : otherPlayers) {
			SphereCollider pCol = p->Collider(); //���v���C���[�̔��苅
			SphereCollider atkCol = Collider();		//�U������̋�
			atkCol.center = transform.position + forward; //�U������̋������
			atkCol.radius = 1.0f * atkRange;
			VECTOR3 pushVec = pCol.center - atkCol.center;

			float rSum = atkCol.radius + pCol.radius;
			if (pushVec.LengthSquare() < rSum * rSum) { // ���̓����蔻��
				// �������Ă�
				pushVec = XMVector3Normalize(pushVec);
				pushVec *= 0.1f;
				pushVec.y = 0.201f;
				p->SetBlowVec(pushVec);
				p->SetSpeedY(pushVec.y);
				p->SetPlayerState(sBlow);
				p->SetPlayerPrevState(sBlow);
				p->SetIsBlow(true);
				p->animator->MergePlay(aBlow, 0);
				p->animator->SetPlaySpeed(2.0f);
				AddLeaf(p->GetLeaf());
				p->ResetLeaf();
				new LeafEffect(p->Position(), VECTOR3(1, 1, 1), p->GetLeaf());
			}
		}
	}

	if (animator->Finished())
	{
		//�U���A�j���[�V�����̏I��
		finishAtkAnim = false;
		animator->SetPlaySpeed(1.0f);
		//transform.rotation.y -= 15 * DegToRad; //��]��߂�
		state = sOnGround;
		atkComboFlg = false;
	}
}

void Player::UpdateCharge()
{
	auto di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_UTRG, DIK_M) || di->CheckJoy(KD_UTRG, 3, playerNum)) {
		state = sOnGround;
		isMagicReady = false;
		chargeFrm = 0;
		mcEffect->SetIsFinish();
		return;
	}
	
	if (animator->PlayingID() == aChargeReady && animator->Finished()) {
		animator->SetPlaySpeed(1.0f);
		animator->MergePlay(aCharge);
		isMagicReady = true;
	}

	if (isMagicReady) {

		if (chargeSpeed <= (chargeFrm * (1.0f / 60.0f)) && leaf > 0) {
			new LeafEffect(transform.position,VECTOR3(0.5f, 0.5f, 0.5f),1);
			mp++;		//MP���Z
			score++;	//�X�R�A���Z
			leaf--; //�t���ς����炷
			if (chargeSpeed > 0.1f) {
				chargeSpeed -= 0.1f;
			}
			chargeFrm = 0;
		}

		chargeFrm++;
	}
}

void Player::UpdateBlow()
{
	transform.position.y += speedY * deltaTime;
	speedY -= GRAVITY * deltaTime;	// �d��

	if (isBlow && speedY <= -0.2f) { //������уA�j���[�V�����̉���(�������n�߂Ă��班�������Ă���)
		animator->MergePlay(aFall, 0); //�����A�j���[�V����
		animator->SetPlaySpeed(1.0f);
		isBlow = false;
	}

	auto di = GameDevice()->m_pDI;
	// �R���g���[���[�X�e�B�b�N�̓��͂��擾
	// ���K������ -1.0�`1.0 �͈̔͂ɂ���
	float ix = di->GetJoyState(playerNum).lX / 1000.0f;
	float iy = di->GetJoyState(playerNum).lY / 1000.0f;

	// �L�[�{�[�h������擾(1P�̂ݓ�������)
	if (di->CheckKey(KD_DAT, DIK_W) && playerNum == 0) {
		iy = -1.0f; // �O�i
	}
	if (di->CheckKey(KD_DAT, DIK_S) && playerNum == 0) {
		iy = 1.0f;  // ���
	}
	if (di->CheckKey(KD_DAT, DIK_A) && playerNum == 0) {
		ix = -1.0f; // ���ړ�
	}
	if (di->CheckKey(KD_DAT, DIK_D) && playerNum == 0) {
		ix = 1.0f;  // �E�ړ�
	}

	// �X�e�B�b�N�����͂���Ă��邩�m�F
	if (fabs(ix) > 0.1f || fabs(iy) > 0.1f) {
		// ���͂Ɋ�Â��Ĉړ�������ݒ�
		VECTOR3 inputDirection = VECTOR3(ix, 0, -iy); // Z�������͔��]

		// �J������Y����]���擾
		float cameraYRotation = cm->GetRotY(playerNum);

		// �J�����̉�]�Ɋ�Â��ړ��x�N�g���̌v�Z
		MATRIX4X4 cameraRotY = XMMatrixRotationY(cameraYRotation);
		VECTOR3 moveDirection = XMVector3TransformNormal(inputDirection, cameraRotY);
		if (sqrt(ix * ix + iy * iy) > 1.0f) {
			moveDirection = XMVector3Normalize(moveDirection) * MOVE_SPEED; // �΂߈ړ������i��葁���Ȃ�Ȃ��悤�␳
		}
		else {
			moveDirection = XMVector3Normalize(moveDirection) * MOVE_SPEED * sqrt(ix * ix + iy * iy); // �X����ɉ��������x
		}

		// �ړ��̓K�p
		transform.position += moveDirection * moveSpeed * deltaTime;

		// �v���C���[�̉�]���J������ŕ�������������
		transform.rotation.y = cameraYRotation + atan2(ix, -iy); // �J�����̉�]�ɑ΂��ăX�e�B�b�N�̕����ɍ��킹��

	}

	if (st->MapCol()->IsCollisionMoveGravity(posOld, transform.position) != clFall) {
		// ������яI��
		isFly = false;
		state = sOnGround;
		jumpCount = 0;
		speedY = 0;
		blowVec = VECTOR3(0, 0, 0);
	}
}

//�@�v���C���[�̎����
Broom::Broom(Object3D* parentModel, int num)
{
	ObjectManager::SetDrawOrder(this, -100);
	parent = parentModel;

	mesh = new CFbxMesh();

	std::string f = "Data/Player/Color" + std::to_string(num) + "/weapon";
	mesh->Load((f + "/broom.mesh").c_str());

	transform.position = VECTOR3(0, 0, 0);
	transform.rotation = VECTOR3(0, 0, -70 * DegToRad);
}

Broom::~Broom()
{
	SAFE_DELETE(mesh);
}

void Broom::Update()
{
	Player* pl = dynamic_cast<Player*>(parent);

	// ��Ԃ��Ƃ̊p�x�ύX
	switch (pl->GetPlayerState()) {
	case sOnGround:
	case sJump:
		if (pl->GetIsDash() || pl->GetIsFly()) {
			transform.position = VECTOR3(7 * 0.01, -10 * 0.01, -35 * 0.01);
			transform.rotation = VECTOR3(77 * DegToRad, 75 * DegToRad, 90 * DegToRad);
		}
		else {
			transform.position = VECTOR3(0, 0, 0);
			transform.rotation = VECTOR3(0, 0, -70 * DegToRad);
		}
		break;
	case sBlow:
		transform.position = VECTOR3(0, 0, 0);
		transform.rotation = VECTOR3(0, 0, -70 * DegToRad);
		break;
	case sAttack1:
	case sAttack2:
	case sAttack3:
		transform.position = VECTOR3(0, 0, 0);
		transform.rotation = VECTOR3(0, 0, 0);
		break;
	case sCharge:
		transform.position = VECTOR3(0, 0, 0);
		transform.rotation = VECTOR3(45 * DegToRad, 0, 0);
		break;
	}
	if (pl->GetIsInvisible()) {
		mesh->m_vDiffuse = VECTOR4(1, 1, 1, 0.2f);
	}
	else {
		mesh->m_vDiffuse = VECTOR4(1, 1, 1, 1);
	}
}

void Broom::Draw()
{
	Player* pl = dynamic_cast<Player*>(parent);
	if (ObjectManager::DrawCounter() != pl->GetPlayerNum()) {
		if (pl->GetIsInvisible()) {
			return;
		}
	}
	mesh->Render(transform.matrix() * mat * parent->Matrix());
}

void Broom::SetPos(MATRIX4X4 m)
{
	mat = m;
}
