#pragma once
#include "Object3D.h"

// ---------------------------------------------------------------------------
// 箒処理
// 
// ゲームプレイ中にプレイヤーの持つ箒の処理を行うクラス
// ---------------------------------------------------------------------------
class Broom : public Object3D {
public:
	Broom(Object3D* parentModel, int num);
	~Broom();
	void Update() override;
	void Draw() override;
	void SetPos(MATRIX4X4 m);
private:
	Object3D* parent;	//親のポインタ
	MATRIX4X4 mat;
};