#include "Dust.h"
#include "Stage.h"
#include "Player.h"

Dust::Dust(int number, VECTOR3 pos)
{
	SetTag("DUST");
	mesh = new CFbxMesh();
	mesh->Load("data/models/boxWooden.mesh");
	transform.position = pos;
	switch (number) { //サイズごとの設定
	case Small:
		transform.scale = VECTOR3(0.5, 0.5, 0.5);
		maxHp = 1;
		break;
	case Midium:
		maxHp = 5;
		break;
	case Big:
		transform.scale = VECTOR3(2, 2, 2);
		maxHp = 10;
		break;
	}
	hp = maxHp;
	dustNum = number;
}

Dust::~Dust()
{
}

void Dust::Update()
{
	ImGui::SetNextWindowPos(ImVec2(100, 60));
	ImGui::SetNextWindowSize(ImVec2(150, 100));
	ImGui::Begin("DustHP");
	ImGui::InputInt("HP", &hp);
	ImGui::End();

	Stage* st = ObjectManager::FindGameObject<Stage>();
	Player* pl = ObjectManager::FindGameObject<Player>();
	if (!(st->IsLandBlock(transform.position))) {
		transform.position.y -= 0.05f;
	}
	if (hp <= 0)
	{
		pl->addMP(maxHp);
		DestroyMe();
	}
}


SphereCollider Dust::Collider(int n)
{
	SphereCollider col;
	switch (n) //サイズごとに判定サイズ変更
	{
	case Small:
		col.center = transform.position + VECTOR3(0, 0.3f, 0);
		col.radius = 0.3f;
		break;
	case Midium:
		col.center = transform.position + VECTOR3(0, 0.4f, 0);
		col.radius = 0.4f;
		break;
	case Big:
		col.center = transform.position + VECTOR3(0, 1.0f, 0);
		col.radius = 1.0f;
		break;
	}
	
	return col;
}

void Dust::AddDamage(int damage)
{
	hp -= damage;
}
