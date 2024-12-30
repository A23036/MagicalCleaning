#include "SlashEffect.h"

SlashEffect::SlashEffect(VECTOR3 pos, float scale)
{
    ObjectManager::SetDrawOrder(this, -1001); //�ŏ��ɕ`��

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

    // �t�F�[�h���̏���
    if (isFading) {
        fadeProgress += (1.0f / 60.0f) / fadeTime;

        // �t�F�[�h
        float alpha = 1.0f - fadeProgress;
        mesh->m_vDiffuse.w = alpha;

        // �t�F�[�h������������폜
        if (fadeProgress >= 1.0f) {
            DestroyMe();
            return;
        }
    }
    
    else if (curTime >= lifeTime) {
        isFading = true; // �t�F�[�h�J�n
    }

    //��]
    transform.rotation.y += rotSpeed;
    frm++;
}

void SlashEffect::SetPos(VECTOR3 pos)
{
    transform.position = pos;
}
