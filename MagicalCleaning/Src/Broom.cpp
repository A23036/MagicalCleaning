#include "Broom.h"
#include "Player.h"

// ---------------------------------------------------------------------------
// コンストラクタ
// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
// デストラクタ
// ---------------------------------------------------------------------------
Broom::~Broom()
{
	SAFE_DELETE(mesh);
}

// ---------------------------------------------------------------------------
// 箒の更新処理
// 
// 親(プレイヤー)の状態に応じて、手元に追従する処理を行う
// ---------------------------------------------------------------------------
void Broom::Update()
{
	Player* pl = dynamic_cast<Player*>(parent);

	// 状態ごとの角度変更
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

// ---------------------------------------------------------------------------
// 箒の描画処理
// 
// 親(プレイヤー)の状態に応じて、表示非表示を切り替える
// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
// 箒の位置セット処理
// ---------------------------------------------------------------------------
void Broom::SetPos(MATRIX4X4 m)
{
	mat = m;
}
