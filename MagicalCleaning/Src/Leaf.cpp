#include "Leaf.h"
#include "Stage.h"
#include "Player.h"
#include "PlayScene.h"
#include "LeafEffect.h"

//定数定義
namespace {
	//リーフサイズ
	static const float SMALL_SIZE  = 0.5f;
	static const float MIDIUM_SIZE = 1.0f;
	static const float BIG_SIZE    = 2.0f;

	//リーフHP
	static const int SMALL_HP  = 1;
	static const int MIDIUM_HP = 5;
	static const int BIG_HP    = 10;

	//落下速度
	static const float FALL_SPEED = 0.2f;

	//アイテムリーフカラー
	static const VECTOR4 ITEM_COLOR = VECTOR4(2.0f, 0.2f, 0.2f, 1.0);
}

// ---------------------------------------------------------------------------
// コンストラクタ
// ---------------------------------------------------------------------------
Leaf::Leaf(int number, VECTOR3 pos)
{
	SetTag("STAGEOBJ");
	mesh = new CFbxMesh();

	mesh->Load("data/Leaf/leaf.mesh");
	transform.position = pos;

	switch (number) { //サイズごとの設定
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
// デストラクタ
// ---------------------------------------------------------------------------
Leaf::~Leaf()
{
}

// ---------------------------------------------------------------------------
// 開始時に呼ばれる関数
// 
// オブジェクトを生成後、最初にUpdate()の前に呼ばれる
// ---------------------------------------------------------------------------
void Leaf::Start()
{
	st = ObjectManager::FindGameObject<Stage>();
}

// ---------------------------------------------------------------------------
// リーフの更新処理
// 
// 生成時の落下処理を行う
// ---------------------------------------------------------------------------
void Leaf::Update()
{
	prevPos = transform.position;

	if (st->MapCol()->IsCollisionMoveGravity(prevPos, transform.position) == clFall) {
		transform.position.y -= FALL_SPEED;
	}

}

// ---------------------------------------------------------------------------
// リーフの描画処理
// ---------------------------------------------------------------------------
void Leaf::Draw()
{
	mesh->Render(transform.matrix());
}

// ---------------------------------------------------------------------------
// リーフの球体コライダー処理
// 
// リーフのコライダーを設定する
// ---------------------------------------------------------------------------
SphereCollider Leaf::Collider(int n)
{
	SphereCollider col;
	col.center = transform.position;
	
	//サイズに応じた判定変更
	col.radius = size / 2;
	
	return col;
}

// ---------------------------------------------------------------------------
// リーフのダメージ反映処理
// 
// リーフが攻撃されたときの処理を行う
// ---------------------------------------------------------------------------
void Leaf::AddDamage(Player* player,int damage)
{
	//葉っぱの飛び散るエフェクトの再生
	new LeafEffect(transform.position, VECTOR3(size, size, size),10);

	player->AddLeaf(1);
	hp -= damage;

	//hpがなくなったとき
	if (hp <= 0)
	{
		player->AddCleanReaf();
		SceneBase* scene = SceneManager::CurrentScene(); // 現在の PlayScene のインスタンス参照を取得
		PlayScene* play = dynamic_cast<PlayScene*>(scene);
		play->LeafDestroyed(this); // PlayScene に通知
		
		DestroyMe(); //削除

		VECTOR3 pos = VECTOR3(transform.position.x, transform.position.y + 3, transform.position.z);
	}

	//最大hpとの比率でサイズを小さくする
	size -= (maxScale - 0.5f) / (float)maxHp;

	//サイズを反映
	transform.scale = VECTOR3(size, size, size);
}
