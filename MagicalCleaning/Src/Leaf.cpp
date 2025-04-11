#include "Leaf.h"
#include "Stage.h"
#include "Player.h"
#include "PlayScene.h"
#include "LeafEffect.h"

//�萔��`
namespace {
	//���[�t�T�C�Y
	static const float SMALL_SIZE  = 0.5f;
	static const float MIDIUM_SIZE = 1.0f;
	static const float BIG_SIZE    = 2.0f;

	//���[�tHP
	static const int SMALL_HP  = 1;
	static const int MIDIUM_HP = 5;
	static const int BIG_HP    = 10;

	//�������x
	static const float FALL_SPEED = 0.2f;

	//�A�C�e�����[�t�J���[
	static const VECTOR4 ITEM_COLOR = VECTOR4(2.0f, 0.2f, 0.2f, 1.0);
}

// ---------------------------------------------------------------------------
// �R���X�g���N�^
// ---------------------------------------------------------------------------
Leaf::Leaf(int number, VECTOR3 pos)
{
	SetTag("STAGEOBJ");
	mesh = new CFbxMesh();

	mesh->Load("data/Leaf/leaf.mesh");
	transform.position = pos;

	switch (number) { //�T�C�Y���Ƃ̐ݒ�
	case Small:
		transform.scale = VECTOR3(SMALL_SIZE, SMALL_SIZE, SMALL_SIZE);
		size = SMALL_SIZE;
		maxHp = SMALL_HP;
		break;
	case Midium:
		transform.scale = VECTOR3(MIDIUM_SIZE, MIDIUM_SIZE, MIDIUM_SIZE);
		size = MIDIUM_SIZE;
		maxHp = MIDIUM_HP;
		break;
	case Big:
		transform.scale = VECTOR3(BIG_SIZE, BIG_SIZE, BIG_SIZE);
		size = BIG_SIZE;
		maxHp = BIG_HP;
		break;
	case Item:
		transform.scale = VECTOR3(SMALL_SIZE, SMALL_SIZE, SMALL_SIZE);
		size = SMALL_SIZE;
		maxHp = SMALL_HP;
		mesh->m_vDiffuse = ITEM_COLOR;
		break;
	}
	hp = maxHp;
	leafNum = number;
	maxScale = size;
}

// ---------------------------------------------------------------------------
// �f�X�g���N�^
// ---------------------------------------------------------------------------
Leaf::~Leaf()
{
}

// ---------------------------------------------------------------------------
// �J�n���ɌĂ΂��֐�
// 
// �I�u�W�F�N�g�𐶐���A�ŏ���Update()�̑O�ɌĂ΂��
// ---------------------------------------------------------------------------
void Leaf::Start()
{
	st = ObjectManager::FindGameObject<Stage>();
}

// ---------------------------------------------------------------------------
// ���[�t�̍X�V����
// 
// �������̗����������s��
// ---------------------------------------------------------------------------
void Leaf::Update()
{
	prevPos = transform.position;

	if (st->MapCol()->IsCollisionMoveGravity(prevPos, transform.position) == clFall) {
		transform.position.y -= FALL_SPEED;
	}

}

// ---------------------------------------------------------------------------
// ���[�t�̕`�揈��
// ---------------------------------------------------------------------------
void Leaf::Draw()
{
	mesh->Render(transform.matrix());
}

// ---------------------------------------------------------------------------
// ���[�t�̋��̃R���C�_�[����
// 
// ���[�t�̃R���C�_�[��ݒ肷��
// ---------------------------------------------------------------------------
SphereCollider Leaf::Collider(int n)
{
	SphereCollider col;
	col.center = transform.position;
	
	//�T�C�Y�ɉ���������ύX
	col.radius = size / 2;
	
	return col;
}

// ---------------------------------------------------------------------------
// ���[�t�̃_���[�W���f����
// 
// ���[�t���U�����ꂽ�Ƃ��̏������s��
// ---------------------------------------------------------------------------
void Leaf::AddDamage(Player* player,int damage)
{
	//�t���ς̔�юU��G�t�F�N�g�̍Đ�
	new LeafEffect(transform.position, VECTOR3(size, size, size),10);

	player->AddLeaf(1);
	hp -= damage;

	//hp���Ȃ��Ȃ����Ƃ�
	if (hp <= 0)
	{
		player->AddCleanReaf();
		SceneBase* scene = SceneManager::CurrentScene(); // ���݂� PlayScene �̃C���X�^���X�Q�Ƃ��擾
		PlayScene* play = dynamic_cast<PlayScene*>(scene);
		play->LeafDestroyed(this); // PlayScene �ɒʒm
		
		DestroyMe(); //�폜

		VECTOR3 pos = VECTOR3(transform.position.x, transform.position.y + 3, transform.position.z);
	}

	//�ő�hp�Ƃ̔䗦�ŃT�C�Y������������
	size -= (maxScale - 0.5f) / (float)maxHp;

	//�T�C�Y�𔽉f
	transform.scale = VECTOR3(size, size, size);
}
