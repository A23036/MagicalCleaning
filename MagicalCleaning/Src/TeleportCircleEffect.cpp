#include "TeleportCircleEffect.h"

TeleportCircleEffect::TeleportCircleEffect(VECTOR3 pos, int num)
{
    ObjectManager::SetDrawOrder(this, -998); //最初に描画

    ec = ObjectManager::FindGameObject<EasingCalc>();

    mesh = new CFbxMesh();
    mesh->Load("data/Effect/TeleportCircleEffect.mesh");

    switch (num) { //プレイヤーカラーごとに色を変更
    case 0:
        color = VECTOR4(255.0f / 255.0f, 51.0f / 255.0f, 102 / 255.0f, 1);
        break;
    case 1:
        color = VECTOR4(44.0f / 255.0f, 169.0f / 255.0f, 224.0f / 255.0f, 1);
        break;
    case 2:
        color = VECTOR4(254.0f / 255.0f, 242.0f / 255.0f, 99.0f / 255.0f, 1);
        break;
    case 3:
        color = VECTOR4(2.0f / 255.0f, 135.0f / 255.0f, 96.0f / 255.0f, 1);
        break;
    case 4:
        color = VECTOR4(160.0f / 255.0f, 84.0f / 255.0f, 179.0f / 255.0f, 1);
        break;
    case 5:
        color = VECTOR4(29.0f / 255.0f, 9.0f / 255.0f, 33.0f / 255.0f, 1);
        break;
    default:
        break;
    }
    mesh->m_vDiffuse = color;

    pos.y += 0.01f;
    transform.position = pos;
    transform.scale = VECTOR3(0, 0, 0);
    curTime = 0;
    rotSpeed = 1.0f;
    frm = 0;

    isSpawn = true;

    SpawnTime = 1.0f;
    RotStart = 0;
    RotGoal = 720 * DegToRad;
    ScaleStart = 0.0f;
    ScaleGoal = 1.0f;
}

TeleportCircleEffect::~TeleportCircleEffect()
{
    SAFE_DELETE(mesh);
}

void TeleportCircleEffect::Update()
{
    curTime = frm * (1.0f / 60.0f);

    if (isSpawn) {
        UpdateSpawn();
    }
    else {
        UpdateRotate();
    }

    frm++;
}

void TeleportCircleEffect::UpdateSpawn()
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

void TeleportCircleEffect::UpdateRotate()
{
    transform.rotation.y += rotSpeed * DegToRad;
}

void TeleportCircleEffect::SetIsFinish()
{
    DestroyMe();
}
