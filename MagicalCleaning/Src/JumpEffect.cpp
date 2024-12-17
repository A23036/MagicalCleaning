#include "JumpEffect.h"

JumpEffect::JumpEffect(VECTOR3 pos)
{
    ObjectManager::SetDrawOrder(this, -1000); //�ŏ��ɕ`��

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
    // �t�F�[�h���̏���
   fadeProgress += (1.0f / 60.0f) / fadeTime;

   // �t�F�[�h
   float alpha = 1.0f - fadeProgress;
    mesh->m_vDiffuse.w = alpha;

    transform.scale *= 1.05f;
   // �t�F�[�h������������폜
   if (fadeProgress >= 1.0f) {
       DestroyMe();
   }
}
