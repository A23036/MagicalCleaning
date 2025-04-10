#pragma once
#include "Object3D.h"

// ---------------------------------------------------------------------------
// ⴏ���
// 
// �Q�[���v���C���Ƀv���C���[�̎���ⴂ̏������s���N���X
// ---------------------------------------------------------------------------
class Broom : public Object3D {
public:
	Broom(Object3D* parentModel, int num);
	~Broom();
	void Update() override;
	void Draw() override;
	void SetPos(MATRIX4X4 m);
private:
	Object3D* parent;	//�e�̃|�C���^
	MATRIX4X4 mat;
};