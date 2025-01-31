#include "Leaf.h"
#include "Stage.h"
#include "Player.h"
#include "PlayScene.h"

Leaf::Leaf(int number, VECTOR3 pos)
{
	SetTag("STAGEOBJ");
	mesh = new CFbxMesh();

	mesh->Load("data/Leaf/leaf.mesh");
	transform.position = pos;

	SmallSize = 0.5f;
	MidiumSize = 1.0f;
	BigSize = 2.0f;

	switch (number) { //�T�C�Y���Ƃ̐ݒ�
	case Small:
		transform.scale = VECTOR3(SmallSize, SmallSize, SmallSize);
		size = 0.5f;
		maxHp = 1;
		break;
	case Midium:
		transform.scale = VECTOR3(MidiumSize, MidiumSize, MidiumSize);
		size = 1.0f;
		maxHp = 5;
		break;
	case Big:
		transform.scale = VECTOR3(BigSize, BigSize, BigSize);
		size = 2.0f;
		maxHp = 10;
		break;
	case Item:
		transform.scale = VECTOR3(SmallSize, SmallSize, SmallSize);
		size = 0.5f;
		maxHp = 1;
		mesh->m_vDiffuse = VECTOR4(2.0f, 0.2f, 0.2f, 1.0);
		break;
	}
	hp = maxHp;
	leafNum = number;
	MaxScale = size;
}

Leaf::~Leaf()
{
}

void Leaf::Start()
{
	st = ObjectManager::FindGameObject<Stage>();
}

void Leaf::Update()
{
	posOld = transform.position;

	if (st->MapCol()->IsCollisionMoveGravity(posOld, transform.position) == clFall) {
		transform.position.y -= 0.2f;
	}

}

void Leaf::Draw()
{
	/*
	// ��ʂ��ƂɃv���C���[���ӂ̂݃}�b�v�`��
	int num = ObjectManager::DrawCounter() + 1;
	std::string s = "Player" + std::to_string(num);
	Player* pl = ObjectManager::FindGameObjectWithTag<Player>(s);
	VECTOR3 pos = pl->Position();

	VECTOR3 dist = pos - transform.position;
	if (dist.LengthSquare() < 400) //����͈͓̔������`��
	{
		mesh->Render(transform.matrix());
	}*/
	mesh->Render(transform.matrix());
}


SphereCollider Leaf::Collider(int n)
{
	SphereCollider col;
	col.center = transform.position;
	
	//�T�C�Y�ɉ���������ύX
	col.radius = size / 2;
	
	return col;
}

void Leaf::AddDamage(Player* player,int damage)
{
	//�t���ς̔�юU��G�t�F�N�g�̍Đ�
	new LeafEffect(transform.position, VECTOR3(size, size, size),10);

	player->AddLeaf(1);
	hp -= damage;

	if (hp <= 0)
	{
		player->AddCleanReaf();
		SceneBase* scene = SceneManager::CurrentScene(); // ���݂� PlayScene �̃C���X�^���X�Q�Ƃ��擾
		PlayScene* play = dynamic_cast<PlayScene*>(scene);
		play->LeafDestroyed(this); // PlayScene �ɒʒm
		
		DestroyMe();

		VECTOR3 pos = VECTOR3(transform.position.x, transform.position.y + 3, transform.position.z);
	}

	size -= (MaxScale - 0.5f) / (float)maxHp;

	transform.scale = VECTOR3(size, size, size);
}
