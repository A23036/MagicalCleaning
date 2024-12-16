#pragma once
#include "Object3D.h"

struct IVECTOR3 {
	int x, y, z;
	IVECTOR3() : x(0), y(0), z(0) {}
	IVECTOR3(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}
	IVECTOR3(VECTOR3 v) {
		x = (int)v.x; y = (int)v.y; z = (int)v.z;
	}
};

class StageData : public Object3D {
public:
	StageData();
	~StageData();
	void Create(IVECTOR3 _size);
	const IVECTOR3& GetSize() const {
		return size;
	}
private:
	IVECTOR3 size;
	// mapデータを三次元の配列として持つ
	std::vector<std::vector<std::vector<int>>> map;
	std::vector<CFbxMesh*> meshes;

	void DrawBox(VECTOR3 pos, DWORD color);
	float rotY;
	int blinkTimer;
};