#include "ReafEffect.h"

ReafEffect::ReafEffect(VECTOR3 pos,VECTOR3 scale)
{
    ObjectManager::SetDrawOrder(this, -1000); //�ŏ��ɕ`��

	// �摜�t�@�C�������[�h
	LoadTexture("data/Reaf/reafchip.png", VECTOR2(1, 1));
	//�\��������W
	transform.position = pos;
	transform.scale = scale;
	// �`�b�v��ǉ�
    
	Chip c;
	chips.push_back(c); //�ǉ�
	ChipControl con;
	con.velocity = VECTOR3(0.01f, 0.01f, 0.0f);
	controls.push_back(con);
	lifeTime = 2.0f;
	frm = 0;

	//useAddBlend = true; // ���Z������
}

ReafEffect::~ReafEffect()
{
}

void ReafEffect::Update()
{
    if (frm * (1.0f / 60.0f) >= lifeTime) {
        DestroyMe();
        return;
    }

    // �ŏ��̃t���[���ŕ����̃I�u�W�F�N�g�𐶐�
    if (frm == 0) {
        const int spawnCount = 10; // ��x�ɐ������鐔
        for (int i = 0; i < spawnCount; i++) {
            // �`�b�v�̐ݒ�
            Chip c;
            c.scale = 1.0f;
            c.alpha = 1.0f;
            chips.push_back(c);

            // �����̐ݒ�
            ChipControl con;
            float rad = (float)rand() / 32767 * XM_2PI; // �����_���Ȋp�x
            float len = (float)rand() / 32767 * 0.1f;   // �����_���ȑ��x�̑傫��
            con.velocity = VECTOR3(cosf(rad) * len, 0.15f, sinf(rad) * len);
            controls.push_back(con);
        }
    }

    // ���������`�i���X�Ɍ����j
    const float damping = 0.98f;

    // �����̃`�b�v���X�V
    auto itrCon = controls.begin();
    for (auto itr = chips.begin(); itr != chips.end();) {
        // �`�b�v�̈ʒu���X�V
        (*itr).offset += (*itrCon).velocity;

        // ���x�̌���
        (*itrCon).velocity *= damping;

        // �d�͌���
        (*itrCon).velocity.y -= 0.005f;

        // �����x������������
        (*itr).alpha -= 0.01f;

        // �����x���[���ȉ��ɂȂ�����폜
        if ((*itr).alpha <= 0.0f) {
            itr = chips.erase(itr); // ���X�g����폜
            itrCon = controls.erase(itrCon);
        }
        else {
            itr++;
            itrCon++;
        }
    }

    frm++;
}
