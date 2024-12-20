#pragma once
// Stage.h
#include "Object3D.h"
#include "CsvReader.h"
#include "MeshCollider.h"
#include "Collision.h"

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
	
	void GenerateRandomMap(int num);
	int GetMapNum() { return MapNum; };
	int GetMapData(int n) { return mapData[n]; };
	int GetMapRotData(int n) { return mapRotData[n]; };

	CCollision* MapCol() {	return mapCol;	}	// -- 2024.12.2
private:
	CFbxMesh* mesh;
	vector<CFbxMesh*> meshes;		// ���b�V���̔z��
	MeshCollider* boxCollider;
	vector<vector<vector<int>>> map;	// �}�b�v�f�[�^�̎O�����z��
	vector<vector<int>> mapChips;		// �}�b�v�`�b�v�f�[�^�̓񎟌��z��
	CsvReader* csv;

	CCollision* mapCol;	// -- 2024.12.2

	int MapNum = 4;
	int mapData[4];		//�z�u����}�b�v�i���o�[
	int mapRotData[4];	//�z�u����}�b�v�̉�]�f�[�^
};
