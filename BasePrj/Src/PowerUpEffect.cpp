#include "PowerUpEffect.h"

PowerUpEffect::PowerUpEffect(Object3D* parentModel,VECTOR3 pos)
{
    ObjectManager::SetDrawOrder(this, -1000); //�ŏ��ɕ`��

    parent = parentModel;

    //SetTag("EFFECT");
    mesh = new CFbxMesh();
    mesh->Load("data/Effect/PowerUpEffect.mesh");

    transform.position = pos;
    lifeTime = 0.25f;
    fadeTime = 0.25f;
    curTime = 0;
    rotSpeed = 0.3f;
    upSpeed = 0.03f;
    frm = 0;

    isFading = false;
    fadeProgress = 0.0f;
}

PowerUpEffect::~PowerUpEffect()
{
}

void PowerUpEffect::Update()
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
    transform.position.x = parent->Position().x;
    transform.position.z = parent->Position().z;

    transform.rotation.y -= rotSpeed;
    transform.position.y += upSpeed;
    frm++;
}
