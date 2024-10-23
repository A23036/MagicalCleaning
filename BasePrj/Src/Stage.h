#pragma once
// Stage.h
#include "Object3D.h"
#include "CsvReader.h"
#include "MeshCollider.h"

using namespace std;

class Stage : public Object3D {
public:
	Stage(int stageNumber);
	~Stage();
	void Update() override;
	void Draw() override;
	void Load(int n);
	bool IsLandBlock(VECTOR3 pos);
	bool HitSphere(const SphereCollider& coll, VECTOR3 *push);

private:
	CFbxMesh* mesh;
	vector<CFbxMesh*> meshes; // 可変の配列
	MeshCollider* boxCollider;
	vector<vector<vector<int>>> map; // マップデータの三次元配列
	CsvReader* csv;
};
