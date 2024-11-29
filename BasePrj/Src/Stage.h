#pragma once
// Stage.h
#include "Object3D.h"
#include "CsvReader.h"
#include "MeshCollider.h"
#include "Camera.h"

using namespace std;

class Stage : public Object3D {
public:
	Stage();
	Stage(int stageNumber);
	~Stage();
	void Update() override;
	void Draw() override;
	void Load();
	void Load(int n);
	bool IsLandBlock(VECTOR3 pos);
	bool HitSphere(const SphereCollider& coll, VECTOR3 *push);
	void GenerateRandomMap(int totalMaps, int size);
	void SaveMapChips(const vector<vector<int>>& mapChips, const string& filePath);
	void SaveMapChips(const vector<vector<vector<int>>>& mapChips, const string& filePath);
	
private:
	CFbxMesh* mesh;
	vector<CFbxMesh*> meshes;		// 可変の配列
	MeshCollider* boxCollider;
	vector<vector<vector<int>>> map;	// マップデータの三次元配列
	vector<vector<int>> mapChips;		// マップチップデータの二次元配列
	CsvReader* csv;
	Camera* cm;

	int size;	//マップのグリッドサイズ
	int chipX;	//マップチップのXサイズ
	int chipY;	//マップチップのYサイズ
	int chipZ;	//マップチップのZサイズ
};
