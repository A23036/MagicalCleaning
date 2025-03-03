#include "Player.h"
#include "../Libs/Imgui/imgui.h"
#include <dinput.h>
#include "CsvReader.h"
#include "Stage.h"
#include "Camera.h"
#include "Leaf.h"
#include "SlashEffect.h"
#include "PowerUpEffect.h"
#include "LeafEffect.h"
#include "JumpEffect.h"
#include "Animator.h"
#include "DataCarrier.h"
#include "MagicCircleEffect.h"
#include "TeleportCircleEffect.h"
#include "PlayDisplay.h"

Player::Player()
{
}

Player::Player(VECTOR3 pos, VECTOR3 rot, int num)//�Z���N�g�V�[��/���U���g�V�[���Ŏg�p
{
	animator = new Animator(); // �C���X�^���X���쐬

	mesh = new CFbxMesh();

	std::string f = "Data/Player/Color" + std::to_string(num);
	mesh->Load((f + "/witch.mesh").c_str());

	mesh->LoadAnimation(aStandBy, (f + "/standby.anmx").c_str(), true);
	

	animator->SetModel(mesh); // ���̃��f���ŃA�j���[�V��������
	animator->Play(aStandBy);
	animator->SetPlaySpeed(1.0f);

	transform.position = pos;
	transform.rotation = rot;

	state = sStandby;
	prevState = sStandby;
}


Player::Player(int num,int color) : playerNum(num),color(color)// �v���C�V�[���Ŏg�p
{
	ObjectManager::SetDrawOrder(this, -100);

	CsvLoad(); // csv����f�[�^�̐ݒ�

	// �v���C���[�̎���ⴂ̐���
	child = new Broom(this,color);

	animator = new Animator(); // �C���X�^���X���쐬

	mesh = new CFbxMesh();

	std::string f = "Data/Player/Color" + std::to_string(color);
	mesh->Load((f + "/witch.mesh").c_str());

	mesh->LoadAnimation(aStandBy, (f + "/standby.anmx").c_str(), true);
	mesh->LoadAnimation(aIdle, _T("Data/Player/idle.anmx"), true);
	mesh->LoadAnimation(aRun, _T("Data/Player/run.anmx"), true);
	mesh->LoadAnimation(aJump, _T("Data/Player/jump.anmx"), false);
	mesh->LoadAnimation(aJump2, _T("Data/Player/jump2.anmx"), false);
	mesh->LoadAnimation(aFly, _T("Data/Player/fly.anmx"), true);
	mesh->LoadAnimation(aFall, _T("Data/Player/fall.anmx"), true);
	mesh->LoadAnimation(aAttack1, _T("Data/Player/attack1.anmx"), false);
	mesh->LoadAnimation(aAttack2, _T("Data/Player/attack2.anmx"), false);
	mesh->LoadAnimation(aAttack3, _T("Data/Player/attack3.anmx"), false);
	mesh->LoadAnimation(aComboAttack, _T("Data/Player/comboAttack.anmx"), true);
	mesh->LoadAnimation(aChargeReady, _T("Data/Player/chargeReady.anmx"), false);
	mesh->LoadAnimation(aCharge, _T("Data/Player/charge.anmx"), true);
	mesh->LoadAnimation(aBlow, _T("Data/Player/blow.anmx"), true);

	animator->SetModel(mesh); // ���̃��f���ŃA�j���[�V��������
	animator->Play(aIdle);
	animator->SetPlaySpeed(1.0f);

	transform.position = VECTOR3(0, 0, 0);
	transform.rotation = VECTOR3(0, playerNum * 90 * DegToRad, 0);
	deltaTime = 0.0f;

	state = sWait;
	prevState = sWait;

	speedY = 0;
	score = 0;
	leaf = 0;
	mp = 100;
	weight = 0;
	jumpCount = 0;
	atkNum = 0;
	chargeSpeed = 1.0f;
	chargeTime = 0.0f;
	invisibleTime = 0.0f;
	tereportPos = VECTOR3(0,0,0);
	teleportFrm = 0;
	fastAtkSpeed = 8;
	damageTime = 0;
	itemNum = -1;

	isDash = false;
	isFly = false;
	isBlow = false;
	isTeleporting = false;

	canTeleport = false;
	setTeleport = false;
	canFly = false;
	canSpeedAtk = false;
	canRangeAtk = false;
	canFastCharge = false;

	finishAtkAnim = false;
	atkComboFlg = false;
	isMagicReady = false;
	isInvisible = false;
	isDamageCool = false;

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

	moveDistance = 0.0f;
	jumpCountAll = 0;
	knockOutCount = 0;
	itemCount = 0;
	cleanReafCount = 0;
	blowCount = 0;

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
	pd = ObjectManager::FindGameObject<PlayDisplay>();
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
	if (playerNum == 0) {
		ImGui::Begin("Rot");
		ImGui::InputFloat("Y", &transform.rotation.y);
		ImGui::End();
	}*/
	/*
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
		if (state == sWait) {
			state = sOnGround;
			prevState = sOnGround;
		}
		state = prevState;
	}
	else if(state != sStandby && state != sWait)
	{
		state = sStop;
	}
	else {
		animator->Update();
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
	case sAttack2:
	case sAttack3:
		UpdateAttack();
		break;
	case sCharge:
		UpdateCharge();
		break;
	case sBlow:
		UpdateBlow();
		break;
	case sTeleport:
		UpdateTeleport();
		return;
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
				new PowerUpEffect(this,transform.position,selectPower);
				GameDevice()->m_pDI->PlayJoyEffect(2, 1, playerNum); // �U���̍Đ�
				GameDevice()->powerUpSE->Play();
				if (msNum == MsTableNum - 1) {
					canTeleport = true;
				}
				else {
					moveSpeed = MoveSpeedT[msNum];
				}
			}
			break;
		case pJN:
			if (mp >= JumpNumC[jnNum] && jnNum < JnTableNum - 1)
			{
				mp -= JumpNumC[jnNum];
				jnNum++;
				new PowerUpEffect(this, transform.position, selectPower);
				GameDevice()->m_pDI->PlayJoyEffect(2, 1, playerNum); // �U���̍Đ�
				GameDevice()->powerUpSE->Play();
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
				new PowerUpEffect(this, transform.position, selectPower);
				GameDevice()->m_pDI->PlayJoyEffect(2, 1, playerNum); // �U���̍Đ�
				GameDevice()->powerUpSE->Play();
				if (asNum == AsTableNum - 1) {
					canSpeedAtk = true;
				}
				else {
					atkSpeed = AtkSpeedT[asNum];
				}
			}
			break;
		case pAR:
			if (mp >= AtkRangeC[arNum] && arNum < ArTableNum - 1)
			{
				mp -= AtkRangeC[arNum];
				arNum++;
				new PowerUpEffect(this, transform.position, selectPower);
				GameDevice()->m_pDI->PlayJoyEffect(2, 1, playerNum); // �U���̍Đ�
				GameDevice()->powerUpSE->Play();
				if (arNum == ArTableNum - 1) {
					canRangeAtk = true;
				}
				else {
					atkRange = AtkRangeT[arNum];
				}
			}
			break;
		case pCW:
			if (mp >= CarWeightC[cwNum] && cwNum < CwTableNum - 1)
			{
				mp -= CarWeightC[cwNum];
				cwNum++;
				new PowerUpEffect(this, transform.position, selectPower);
				GameDevice()->m_pDI->PlayJoyEffect(2, 1, playerNum); // �U���̍Đ�
				GameDevice()->powerUpSE->Play();
				if (cwNum == CwTableNum - 1) {
					canFastCharge = true;
				}
				else {
					carWeight = CarWeightT[cwNum];
				}
			}
			break;
		}
	}
	
	
	if ((di->CheckKey(KD_TRG, DIK_LEFT) && playerNum == 0) || di->CheckJoy(KD_TRG, 4, playerNum)) { //�����\�͕ύX
		selectPower--;
		if (selectPower < 0) {
			selectPower = 4;
		}
		GameDevice()->powerSelectSE->Play();
	}
	if ((di->CheckKey(KD_TRG, DIK_RIGHT) && playerNum == 0) || di->CheckJoy(KD_TRG, 5, playerNum)) { //�����\�͕ύX
		selectPower++;
		if (selectPower > 4) {
			selectPower = 0;
		}
		GameDevice()->powerSelectSE->Play();
	}

	auto* pdi = GameDevice()->m_pDI;
	DIJOYSTATE2 joyState = pdi->GetJoyState(playerNum);
	
	//�A�C�e���g�p����
	if (joyState.rgbButtons[11] & 0x80) {
		if (itemNum != -1) {
			UseItem(itemNum);
			itemNum = -1;
		}
	}
	 
	//�e���|�[�g����
	// �e���|�[�g�悪�ݒu����Ă��Đ�����΂���Ă��Ȃ��Ƃ�
	if (((di->CheckKey(KD_TRG, DIK_F) && playerNum == 0) 
		|| joyState.rgbButtons[10] & 0x80) && setTeleport && state != sBlow) {
		state = sTeleport;
		finishAtkAnim = false;
		isTeleporting = true;
		if (mcEffect != nullptr) {
			mcEffect->SetIsFinish();
		}
	}

	//���G���ԏ���
	if (isDamageCool) {
		damageTime++;
		if (damageTime * (1.0f / 60.0f) > DamageCoolTime) {
			damageTime = 0;
			isDamageCool = false;
			mesh->m_vDiffuse = VECTOR4(1, 1, 1, 1.0f);
		}
		else { //�����x��ς��ē_�ł���
			if (damageTime % 12 > 6) {
				mesh->m_vDiffuse = VECTOR4(1, 1, 1, 0.3f);
			}
			else {
				mesh->m_vDiffuse = VECTOR4(1, 1, 1, 0.7f);
			}
		}
	}

	//��O����
	if (transform.position.y < -30.0f || fabs(transform.position.x) > 60.0f || fabs(transform.position.z) > 60.0f){
		transform.position = VECTOR3(0, 20, 0);
		SetIsBlow();
		SetPlayerState(sBlow);
		animator->MergePlay(aBlow, 0);
		animator->SetPlaySpeed(2.0f);
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
		if (invisibleTime * (1.0f / 60.0f) > InvisibleTime[dc->GetRank(playerNum) - 1]) {
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

	for (int i = 0; i < 32; i++)
	{
		// �e�{�^���̉�����Ԃ��m�F
		if (joyState.rgbButtons[i] & 0x80) // �{�^�� i ��������Ă���ꍇ
		{
			ImGui::Text("%dP : Button %d is pressed", playerNum,i); // �{�^���ԍ���\��
		}
	}
	
	if (di->IfJoyFF(playerNum)) {
		ImGui::Text("True");
	}
	else {
		ImGui::Text("False");
	}
	float rx = di->GetJoyState().lRx;
	float ry = di->GetJoyState().lRy;
	float rz = di->GetJoyState().lRz;
	ImGui::End();
	*/

	// Leaf�ɂ߂荞�܂Ȃ��悤�ɂ���
	// �����̍��W�́Atransform.position
	// Leaf�̍��W��m��
	
	std::list<Leaf*> leaves = ObjectManager::FindGameObjects<Leaf>();
	
	for (Leaf* leaf : leaves) {
		SphereCollider tCol = leaf->Collider(leaf->GetNum());
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
				InvisibleTime[0] = csv->GetFloat(i, 3);
				InvisibleTime[1] = csv->GetFloat(i, 4);
				InvisibleTime[2] = csv->GetFloat(i, 5);
				InvisibleTime[3] = csv->GetFloat(i, 6);
			}
			if (csv->GetString(i, 1) == "DamageCoolTime") {		// ���G����
				DamageCoolTime = csv->GetInt(i, 3);
			}
			if (csv->GetString(i, 1) == "TeleportTime") {		// �e���|�[�g�ҋ@����
				TeleportTime = csv->GetInt(i, 3);
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
		if (csv->GetString(i, 0) == "Leaf") {
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

float Player::GetInvisibleTime()
{
	return  InvisibleTime[dc->GetRank(playerNum) - 1];
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

void Player::SetIsBlow()
{
	blowCount++;
	isBlow = true;
	isDash = false; 
	isFly = false;
	finishAtkAnim = false;
	GameDevice()->m_pDI->PlayJoyEffect(1, 1, playerNum); // �U���̍Đ�

	if (mcEffect != nullptr) {
		mcEffect->SetIsFinish();
	}
	if (isTeleporting) {
		teleportFrm = 0;
		mesh->m_vDiffuse = VECTOR4(1, 1, 1, 1);
	}
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

void Player::AddCleanReaf()
{
	cleanReafCount++;
}

void Player::UseItem(int num)
{
	switch (num) {
	case 0:
		GameDevice()->itemSE->Play();
		invisibleTime = 0;
		isInvisible = true;
		itemCount++;
		break;
	default:
		break;
	}
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
		moveDistance += moveDirection.Length(); //�ړ������̉��Z
		
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
		jumpCountAll++;
		state = sJump;
		GameDevice()->jumpSE->Play();
		GameDevice()->m_pDI->PlayJoyEffect(2, 1, playerNum); // �U���̍Đ�
	}
	if ((di->CheckKey(KD_TRG, DIK_N) && playerNum == 0) || di->CheckJoy(KD_TRG, 0, playerNum)) { //�U��
		if (canSpeedAtk) {
			animator->MergePlay(aComboAttack, 0);
			animator->SetPlaySpeed(8.0f);
		}
		else {
			animator->MergePlay(aAttack1, 0);
			animator->SetPlaySpeed(1.0f * atkSpeed);
		}
		state = sAttack1;
		isDash = false;
		anmFrame = 0;
		atkNum = 1;
	}
	if ((di->CheckKey(KD_TRG, DIK_M) && playerNum == 0) || di->CheckJoy(KD_TRG, 3, playerNum)) { //MP�ϊ�
		animator->MergePlay(aChargeReady);
		animator->SetPlaySpeed(1.0f);
		state = sCharge;
		isDash = false;
		chargeSpeed = 1.0f;
		mcEffect = new MagicCircleEffect(transform.position, color);
		GameDevice()->magicCircleSE->Play();
	}
	if (((di->CheckKey(KD_TRG, DIK_R) && playerNum == 0) || di->CheckJoy(KD_TRG, 7, playerNum)) && canTeleport) { //�e���|�[�g���@�w�ݒu
		setTeleport = true;
		tereportPos = transform.position;
		if (tpEffect != nullptr) {
			tpEffect->SetIsFinish();
		}
		tpEffect = new TeleportCircleEffect(transform.position, color);
	}
}


void Player::UpdateJump()
{
	transform.position.y += speedY * deltaTime;

	//��s�\���W�����v�L�[����͂����ςȂ��ō~�����Ă���Ƃ�
	if (canFly && GameDevice()->m_pDI->CheckJoy(KD_DAT, 1, playerNum) && speedY < 0) {
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
		moveDistance += moveDirection.Length(); //�ړ������̉��Z

		// �ړ��̓K�p
		transform.position += moveDirection * moveSpeed * deltaTime;

		// �v���C���[�̉�]���J������ŕ�������������
		transform.rotation.y = cameraYRotation + atan2(ix, -iy); // �J�����̉�]�ɑ΂��ăX�e�B�b�N�̕����ɍ��킹��
	}

	if (di->CheckJoy(KD_TRG, 1, playerNum) && jumpCount <= jumpNum) { //�W�����v�\�񐔂��c���Ă���Ƃ�
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
		GameDevice()->jumpSE->Play();
		jumpCount++;
		jumpCountAll++;
	}


	if (animator->Finished()) {
		if (isFly) {
			animator->MergePlay(aFly);
		}
		else{
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

void Player::UpdateAttack()
{	
	anmFrame++;

	if ((!finishAtkAnim && animator->CurrentFrame() >= 20.0f / atkSpeed) //�U���̃q�b�g�����^�C�~���O
		|| (canSpeedAtk && anmFrame % fastAtkSpeed == 0)) { //�A���U���̏ꍇ
		
		if (asNum > 6) {
			GameDevice()->attack3SE->Play();
		}
		else if (asNum > 3) {
			GameDevice()->attack2SE->Play();
		}
		else {
			GameDevice()->attack1SE->Play();
		}
		//�U�����菈��
		CheckAtkCoillision();
	}
	
	auto di = GameDevice()->m_pDI;
	if (canSpeedAtk) {
		// �U�����̕����]������
		auto di = GameDevice()->m_pDI;
		float ix = di->GetJoyState(playerNum).lX / 1000.0f;
		float iy = di->GetJoyState(playerNum).lY / 1000.0f;

		// �X�e�B�b�N�����͂���Ă��邩�m�F
		if (fabs(ix) > 0.1f || fabs(iy) > 0.1f) {
			// �J������Y����]���擾
			float cameraYRotation = cm->GetRotY(playerNum);
			// �v���C���[�̉�]���J������Ō��߂�
			transform.rotation.y = cameraYRotation + atan2(ix, -iy); // �J�����̉�]�ɑ΂��ăX�e�B�b�N�̕����ɍ��킹��
		}
	}
	else {
		if (di->CheckKey(KD_TRG, DIK_N) || di->CheckJoy(KD_TRG, 0, playerNum)) { //�U�����̐�s���͎�t
			atkComboFlg = true;
			anmFrame = 0;
		}

		if (anmFrame >= comboWaitFrm) { //�A�����͌��莞�ԂŘA���U���t���O����
			atkComboFlg = false;
		}
	}

	if (animator->Finished() || (canSpeedAtk && di->CheckJoy(KD_UTRG, 0, playerNum)))
	{
		if (canSpeedAtk) {
			animator->SetPlaySpeed(1.0f);
			state = sOnGround;
			anmFrame = 0;
		}
		//�U���A�j���[�V�����̏I��
		finishAtkAnim = false;
		animator->SetPlaySpeed(1.0f);
		if (atkComboFlg && atkNum < 3) { //�A���U������/�A���U����4��ڂłȂ�
			if (atkNum == 1) {
				animator->SetPlaySpeed(1.0f * atkSpeed);
				animator->MergePlay(aAttack2, 0);
				state = sAttack2;
			}
			else if (atkNum == 2) {
				animator->SetPlaySpeed(1.2f * atkSpeed);
				animator->MergePlay(aAttack3, 0);
				state = sAttack3;
			}
			anmFrame = 0;
			atkNum++;

			// �U�����̕����]������
			auto di = GameDevice()->m_pDI;
			float ix = di->GetJoyState(playerNum).lX / 1000.0f;
			float iy = di->GetJoyState(playerNum).lY / 1000.0f;

			// �X�e�B�b�N�����͂���Ă��邩�m�F
			if (fabs(ix) > 0.1f || fabs(iy) > 0.1f) {
				// �J������Y����]���擾
				float cameraYRotation = cm->GetRotY(playerNum);
				// �v���C���[�̉�]���J������Ō��߂�
				transform.rotation.y = cameraYRotation + atan2(ix, -iy); // �J�����̉�]�ɑ΂��ăX�e�B�b�N�̕����ɍ��킹��
			}
		}
		else {
			//�n���Ԃ�
			state = sOnGround;
		}
		atkComboFlg = false;
	}
	
}


void Player::UpdateCharge()
{
	auto di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_UTRG, DIK_M) || di->CheckJoy(KD_UTRG, 3, playerNum)) {
		animator->SetPlaySpeed(1.0f);
		state = sOnGround;
		isMagicReady = false;
		chargeTime = 0.0f;
		mcEffect->SetIsFinish();
		return;
	}
	
	if (animator->PlayingID() == aChargeReady && animator->Finished()) {
		animator->MergePlay(aCharge);
		isMagicReady = true;
	}

	if (isMagicReady) {
		if (chargeSpeed <= (chargeTime * (1.0f / 60.0f)) && leaf > 0) {
			new LeafEffect(transform.position,VECTOR3(0.5f, 0.5f, 0.5f),1);
			GameDevice()->chargeSE->Play();
			mp++;		//MP���Z
			score++;	//�X�R�A���Z
			leaf--; //�t���ς����炷
			GameDevice()->m_pDI->PlayJoyEffect(2, 1, playerNum); // �U���̍Đ�
			if (chargeSpeed > 0.1f) {
				chargeSpeed -= 0.1f;
			}
			chargeTime = 0;
		}
		if (canFastCharge) {
			animator->SetPlaySpeed(2.0f);
			chargeTime += 2.0f;
		}
		else {
			animator->SetPlaySpeed(1.0f);
			chargeTime++;
		}
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
		isDamageCool = true; //���G���Ԃ̊J�n
	}
}

void Player::UpdateTeleport()
{
	if (teleportFrm * (1.0f / 60.0f) > TeleportTime) {
		mesh->m_vDiffuse = VECTOR4(1, 1, 1, 1);
		transform.position = tereportPos;
		state = sOnGround;
		prevState = sOnGround;
		teleportFrm = 0;
		isTeleporting = false;
		isFly = false;
		return;
	}

	if (teleportFrm % 20 > 10) {
		mesh->m_vDiffuse = VECTOR4(1, 1, 1, 0.5f);
	}
	else {
		mesh->m_vDiffuse = VECTOR4(1, 1, 1, 1);
	}
	teleportFrm++;
}

void Player::CheckAtkCoillision()
{
	// �S�~�ɍU���𓖂Ă�
	std::list<Leaf*> dusts = ObjectManager::FindGameObjects<Leaf>();

	finishAtkAnim = true;

	// �U�����蔭���ʒu�i�[�p�z��
	vector<VECTOR3> attackPositions;

	int attackNum = 1; //�U�����萔
	// �͈͍U�����\�Ȏ��A�U�������3����
	if (canRangeAtk) {
		attackNum = 3;
	}

	// �U������̔����ʒu���v�Z
	for (int i = 0; i < attackNum; i++) {
		VECTOR3 forward;
		// �v���C���[��Y����]�s��(�͈͍U���̎��A�v���C���[�̑O���Ǝ΂ߑO2���ɍU������)
		MATRIX4X4 rotY;
		if (i > 0) {
			rotY = XMMatrixRotationY(transform.rotation.y + pow(-1,i)* 1.25f);
			// �΂ߑO�̍U���͂��O����
			forward = VECTOR3(0, 0, 1.8f * atkRange); // �O���x�N�g��
		}
		else {
			forward = VECTOR3(0, 0, 1.2f * atkRange); // �O���x�N�g��
			rotY = XMMatrixRotationY(transform.rotation.y);
		}
		forward = forward * rotY; // �v���C���[�̌����Ă���ւ̈ړ���
		VECTOR3 pos = forward + transform.position;

		// �U������ʒu�����i�[
		attackPositions.push_back(pos);

		//�G�t�F�N�g�̍Đ�
		new SlashEffect(pos, atkRange);
	}

	// �U�����萔�����������蔻����v�Z
	for (VECTOR3 atkPos : attackPositions) {
		for (Leaf* d : dusts) {
			SphereCollider dCol = d->Collider(d->GetNum()); //�S�~�̔��苅
			SphereCollider atkCol = Collider();			//�U������̋�

			//�U������̋������
			atkCol.center = atkPos;
			atkCol.radius = 1.0f * atkRange;
			VECTOR3 pushVec = atkCol.center - dCol.center;
			float rSum = atkCol.radius + dCol.radius;
			if (pushVec.LengthSquare() < rSum * rSum) { // ���̓����蔻��
				// �������Ă�
				GameDevice()->m_pDI->PlayJoyEffect(2, 1, playerNum); // �U���̍Đ�
				d->AddDamage(this, 1); //�_���[�W��^����
				if (d->GetNum() == 3) { //������
					itemNum = 0;
				}
			}
		}

		//���v���C���[�ւ̍U������
		for (Player* p : otherPlayers) {
			SphereCollider pCol = p->Collider(); //���v���C���[�̔��苅
			SphereCollider atkCol = Collider();		//�U������̋�
			
			//�U������̋������
			atkCol.center = atkPos;
			atkCol.radius = 1.0f * atkRange;
			VECTOR3 pushVec = pCol.center - atkCol.center;

			float rSum = atkCol.radius + pCol.radius;
			if (pushVec.LengthSquare() < rSum * rSum && !p->GetIsDamage()) { // ���̓����蔻��
				// �������Ă�
				GameDevice()->m_pDI->PlayJoyEffect(0, 1, playerNum); // �U���̍Đ�
				GameDevice()->blowSE->Play();
				GameDevice()->blowCheerSE->Play();
				pushVec = XMVector3Normalize(pushVec);
				pushVec *= 0.2f;
				pushVec.y = 0.201f;
				p->SetBlowVec(pushVec);
				p->SetSpeedY(pushVec.y);
				p->SetPlayerState(sBlow);
				p->SetPlayerPrevState(sBlow);
				p->SetIsBlow();
				p->animator->MergePlay(aBlow, 0);
				p->animator->SetPlaySpeed(2.0f);
				AddLeaf(p->GetLeaf());
				p->ResetLeaf();
				new LeafEffect(p->Position(), VECTOR3(1, 1, 1), p->GetLeaf());
				pd->SetBlowPlayer(playerNum,p->GetPlayerNum());
				knockOutCount++;
			}
		}
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
