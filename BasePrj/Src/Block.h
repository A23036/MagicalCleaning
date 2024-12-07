#pragma once
#include "Object3D.h"
#include "Collision.h"

class Block : public Object3D {
public:
	Block(int num,VECTOR3 pos);
	~Block();
	void Start() override;
	void Update();
	CCollision* BlockCol() { return blockCol; }
private:
	CFbxMesh* meshCol;
	CCollision* blockCol;

	enum Blocks {
		Block1 = 0,
		Block2,
	};
};