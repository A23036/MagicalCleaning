#include "JumpEffect.h"

JumpEffect::JumpEffect(VECTOR3 pos)
{
    ObjectManager::SetDrawOrder(this, -1000); //最初に描画

    mesh = new CFbxMesh();
    mesh->Load("data/Effect/MagicCircleEffect.mesh");

    transform.position = pos;
    lifeTime = 0.25f;
    fadeTime = 0.25f;
    curTime = 0;
    frm = 0;

    fadeProgress = 0.0f;
}

JumpEffect::~JumpEffect()
{
}

void JumpEffect::Update()
{
    // フェード中の処理
   fadeProgress += (1.0f / 60.0f) / fadeTime;

   // フェード
   float alpha = 1.0f - fadeProgress;
    mesh->m_vDiffuse.w = alpha;

    transform.scale *= 1.05f;
   // フェードが完了したら削除
   if (fadeProgress >= 1.0f) {
       DestroyMe();
   }
}
