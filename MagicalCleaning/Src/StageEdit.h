#pragma once
// StageEdit.h
#include "Object3D.h"

using namespace std;

class StageEdit : public Object3D {
public:
	StageEdit();
	~StageEdit();
	void Update() override;
	void Draw() override;
	void Save(int n);
	void Load(int n);
	void Create(int xsize, int ysize, int zsize);

private:
	vector<CFbxMesh*> meshes; // 可変の配列
	vector<vector<vector<int>>> map; // マップデータの二次元配列
	void DrawBox(VECTOR3 pos, DWORD color);
	//struct IVECTOR3 {
	//	int x, y, z;
	//	IVECTOR3() : x(0), y(0), z(0) {}
	//	IVECTOR3(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}
	//};
	//IVECTOR3 cursor;

	int cursorX, cursorY, cursorZ;
	int stageNumber;

	int xsize;
	int ysize;
	int zsize;
};