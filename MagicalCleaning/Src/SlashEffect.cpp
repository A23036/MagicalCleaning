#include "SlashEffect.h"

SlashEffect::SlashEffect(VECTOR3 pos, float scale)
{
    ObjectManager::SetDrawOrder(this, -1001); //最初に描画

    //SetTag("EFFECT");
    mesh = new CFbxMesh();
    mesh->Load("data/Effect/SlashEffect.mesh");

    transform.position = pos;
    transform.scale = VECTOR3(scale,scale,scale);
    lifeTime = 0.25f;
    fadeTime = 0.25f;
    curTime = 0;
    rotSpeed = 0.3f;
    frm = 0;

    isFading = false;
    fadeProgress = 0.0f;
}

SlashEffect::~SlashEffect()
{
}

void SlashEffect::Update()
{
    curTime = frm * (1.0f / 60.0f);

    // フェード中の処理
    if (isFading) {
        fadeProgress += (1.0f / 60.0f) / fadeTime;

        // フェード
        float alpha = 1.0f - fadeProgress;
        mesh->m_vDiffuse.w = alpha;

        // フェードが完了したら削除
        if (fadeProgress >= 1.0f) {
            DestroyMe();
            return;
        }
    }
    
    else if (curTime >= lifeTime) {
        isFading = true; // フェード開始
    }

    //回転
    transform.rotation.y += rotSpeed;
    frm++;
}

void SlashEffect::SetPos(VECTOR3 pos)
{
    transform.position = pos;
}
