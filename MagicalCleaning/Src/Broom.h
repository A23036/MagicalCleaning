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
	virtual ~Broom();
	void Update() override;
	void Draw() override;
	void SetPos(MATRIX4X4 m);
private:
	Object3D* parent;	//親(プレイヤー)のポインタ
	MATRIX4X4 mat;
};