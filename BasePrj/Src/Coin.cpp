#include "Coin.h"
#include "Player.h"

Coin::Coin()
{
	mesh = new CFbxMesh();
	mesh->Load("data/models/coin.mesh");
	got = false;
}

Coin::~Coin()
{
}

void Coin::Update()
{
	if (got) {
		transform.rotation.y += 30.0f * DegToRad;
		transform.scale -= VECTOR3(0.02f, 0.02f, 0.02f);
		transform.position += VECTOR3(0, 0.05f, 0);
		if (transform.scale.x < 0.5f) {
			DestroyMe();
		}
	} else {
		transform.rotation.y += 3.0f * DegToRad;
		Player* pl = ObjectManager::FindGameObject<Player>();
		VECTOR3 diff = pl->Position() - transform.position;
		if (diff.Length() < 0.5f) {
			transform.position.y += 0.5f;
			got = true;
			//pl->addMP(10);
		}
	}
}
