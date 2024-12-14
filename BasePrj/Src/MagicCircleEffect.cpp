#include "MagicCircleEffect.h"

MagicCircleEffect::MagicCircleEffect(VECTOR3 pos, int num)
{
    ObjectManager::SetDrawOrder(this, -998); //最初に描画

    ec = ObjectManager::FindGameObject<EasingCalc>();

    mesh = new CFbxMesh();
    mesh->Load("data/Effect/MagicCircleEffect.mesh");
    
    switch (num) { //プレイヤーカラーごとに色を変更
    case 0:
        color = VECTOR4(0.2156, 0.1921, 0.9607, 1);
        break;
    case 1:
        color = VECTOR4(0.9607, 0.1921, 0.9333, 1);
        break;
    case 2:
        color = VECTOR4(0.9333, 0.9607, 0.1921, 1);
        break;
    case 3:
        color = VECTOR4(0.9607, 0.2156, 0.1921, 1);
        break;
    case 4:
        color = VECTOR4(0.1921, 0.9607, 0.2156, 1);
        break;
    default:
        break;
    }
    mesh->m_vDiffuse = color;

    transform.position = pos;
    transform.scale = VECTOR3(0, 0, 0);
    curTime = 0;
    rotSpeed = 0.2f;
    frm = 0;

    isSpawn = true;
    isFading = false;

    SpawnTime = 1.0f;
    FadeTime = 0.5f;
    RotStart = 0;
    RotGoal = 720 *  DegToRad;
    ScaleStart = 0.0f;
    ScaleGoal = 4.0f;
}

MagicCircleEffect::~MagicCircleEffect()
{
    SAFE_DELETE(mesh);
}

void MagicCircleEffect::Update()
{
    curTime = frm * (1.0f / 60.0f);

    if (isFinish) {
        UpdateFinish(); 
    }

    if (isSpawn) {
        UpdateSpawn();
    }
    else {
        UpdateRotate();
    }

    frm++;
}

void MagicCircleEffect::UpdateSpawn()
{
    // アニメーションの進行度計算
    float timeRate = curTime / SpawnTime;
    float rate = ec->easeOutExpo(timeRate);

    float rotation = (RotGoal - RotStart) * rate - RotStart;	// 回転
    float scale = (ScaleGoal - ScaleStart) * rate - ScaleStart;	// 拡大倍率

    transform.scale = VECTOR3(scale, scale, scale);
    transform.rotation.y = rotation;

    if (curTime >= SpawnTime) {
        isSpawn = false;
    }
}

void MagicCircleEffect::UpdateRotate()
{
    transform.rotation.y += rotSpeed * DegToRad;
}

void MagicCircleEffect::UpdateFinish()
{
    // アニメーションの進行度計算
    float timeRate = curTime / FadeTime;
    float rate = timeRate;

    float alpha = -1 * rate + 1;	// 透明度

    mesh->m_vDiffuse = VECTOR4(1,1,1,alpha);
    
    if (curTime >= FadeTime) {
        DestroyMe();
    }
}

void MagicCircleEffect::SetIsFinish()
{
    isFinish = true;
    frm = 0;
    curTime = 0;
}
