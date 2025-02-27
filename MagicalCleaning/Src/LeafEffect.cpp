#include "LeafEffect.h"

LeafEffect::LeafEffect(VECTOR3 pos,VECTOR3 scale, int num)
{
    ObjectManager::SetDrawOrder(this, -1000); //最初に描画

	// 画像ファイルをロード
	LoadTexture("data/Leaf/leafchip.png", VECTOR2(1, 1));
	//表示する座標
	transform.position = pos;
	transform.scale = scale;
	// チップを追加
    
	//Chip c;
	//chips.push_back(c); //追加
	//ChipControl con;
	//con.velocity = VECTOR3(0.01f, 0.01f, 0.0f);
	//controls.push_back(con);
    leafNum = num;
	lifeTime = 1.5f;
	frm = 0;
    isFinish = false;

	//useAddBlend = true; // 加算半透明
}

LeafEffect::~LeafEffect()
{
}

void LeafEffect::Update()
{
    if (frm * (1.0f / 60.0f) >= lifeTime) {
        chips.clear();
        controls.clear();
        isFinish = true;
        DestroyMe();
        return;
    }

    // 最初のフレームで複数のオブジェクトを生成
    if (frm == 0) {
        for (int i = 0; i < leafNum; i++) {
            // チップの設定
            Chip c;
            c.scale = 1.0f;
            c.alpha = 1.0f;
            chips.push_back(c);

            // 挙動の設定
            ChipControl con;
            float rad = (float)rand() / 32767 * XM_2PI; // ランダムな角度
            float len = (float)rand() / 32767 * 0.1f;   // ランダムな速度の大きさ
            con.velocity = VECTOR3(cosf(rad) * len, 0.15f, sinf(rad) * len);
            controls.push_back(con);
        }
    }
     
    // 減衰率
    const float damping = 0.98f;

    // 既存のチップを更新
    auto itrCon = controls.begin();
    for (auto itr = chips.begin(); itr != chips.end();) {
        // チップの位置を更新
        (*itr).offset += (*itrCon).velocity;

        // 速度の減衰
        (*itrCon).velocity *= damping;

        // 重力効果
        (*itrCon).velocity.y -= 0.005f;

        // 透明度を減少させる
        (*itr).alpha -= 0.01f;

        // 透明度がゼロ以下になったら削除
        if ((*itr).alpha <= 0.0f) {
            itr = chips.erase(itr); // リストから削除
            itrCon = controls.erase(itrCon);
        }
        else {
            itr++;
            itrCon++;
        }
    }

    frm++;
}
