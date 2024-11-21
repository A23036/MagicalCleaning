#include "Dust.h"
#include "Stage.h"
#include "Player.h"

Dust::Dust(int number, VECTOR3 pos)
{
	SetTag("STAGEOBJ");
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
	Stage* st = ObjectManager::FindGameObject<Stage>();
	if (!(st->IsLandBlock(transform.position))) {
		transform.position.y -= 0.05f;
	}
}

void Dust::Draw()
{
	// 画面ごとにプレイヤー周辺のみマップ描画
	int num = ObjectManager::DrawCounter() + 1;
	std::string s = "Player" + std::to_string(num);
	Player* pl = ObjectManager::FindGameObjectWithTag<Player>(s);
	VECTOR3 pos = pl->Position();

	VECTOR3 dist = pos - transform.position;
	if (dist.LengthSquare() < 400) //特定の範囲内だけ描画
	{
		mesh->Render(transform.matrix());
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

void Dust::AddDamage(Player* player,int damage)
{
	player->AddMP(1);
	player->AddScore(1);
	hp -= damage;
	if (hp <= 0)
	{
		player->AddMP(maxHp);
		player->AddScore(maxHp);
		DestroyMe();
		VECTOR3 pos = VECTOR3(transform.position.x, transform.position.y + 3, transform.position.z);
		new Dust(dustNum, pos);
	}
}
