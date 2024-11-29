#include "SlashEffect.h"

SlashEffect::SlashEffect(VECTOR3 pos, float scale)
{
    ObjectManager::SetDrawOrder(this, -1000); //Å‰‚É•`‰æ

    //SetTag("EFFECT");
    mesh = new CFbxMesh();
    mesh->Load("data/Effect/SlashEffect.mesh");

    transform.position = pos;
    transform.scale = VECTOR3(scale,scale,scale);
    lifeTime = 0.5f;
    curTime = 0;
    rotSpeed = 0.3f;
    frm = 0;
}

SlashEffect::~SlashEffect()
{
}

void SlashEffect::Update()
{
    curTime = frm * (1.0f / 60.0f);
    if (curTime >= lifeTime) {
        DestroyMe();
        return;
    }

    transform.rotation.y += rotSpeed;
    frm++;
}
