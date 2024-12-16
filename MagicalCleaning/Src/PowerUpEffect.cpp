#include "PowerUpEffect.h"

PowerUpEffect::PowerUpEffect(Object3D* parentModel,VECTOR3 pos,int num)
{
    ObjectManager::SetDrawOrder(this, -1000); //最初に描画

    parent = parentModel;

    //SetTag("EFFECT");
    mesh = new CFbxMesh();
    mesh->Load("data/Effect/PowerUpEffect.mesh");
    
    switch (num) { //強化能力ごとに色を変更
    case 0:
        mesh->m_vDiffuse = VECTOR4(0.2156, 0.1921, 0.9607, 1);
        break;
    case 1:
        mesh->m_vDiffuse = VECTOR4(0.9607, 0.1921, 0.9333, 1);
        break;
    case 2:
        mesh->m_vDiffuse = VECTOR4(0.9333, 0.9607, 0.1921, 1);
        break;
    case 3:
        mesh->m_vDiffuse = VECTOR4(0.9607, 0.2156, 0.1921, 1);
        break;
    case 4:
        mesh->m_vDiffuse = VECTOR4(0.1921, 0.9607, 0.2156, 1);
        break;
    default:
        break;
    }

    transform.position = pos;
    lifeTime = 0.25f;
    fadeTime = 0.25f;
    curTime = 0;
    rotSpeed = 0.2f;
    upSpeed = 0.03f;
    frm = 0;

    isFading = false;
    fadeProgress = 0.0f;
}

PowerUpEffect::~PowerUpEffect()
{
    SAFE_DELETE(mesh);
}

void PowerUpEffect::Update()
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
    transform.position.x = parent->Position().x;
    transform.position.z = parent->Position().z;

    transform.rotation.y -= rotSpeed;
    transform.position.y += upSpeed;
    frm++;
}
