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
	vector<CFbxMesh*> meshes;		// �ς̔z��
	MeshCollider* boxCollider;
	vector<vector<vector<int>>> map;	// �}�b�v�f�[�^�̎O�����z��
	vector<vector<int>> mapChips;		// �}�b�v�`�b�v�f�[�^�̓񎟌��z��
	CsvReader* csv;
	Camera* cm;

	int size;	//�}�b�v�̃O���b�h�T�C�Y
	int chipX;	//�}�b�v�`�b�v��X�T�C�Y
	int chipY;	//�}�b�v�`�b�v��Y�T�C�Y
	int chipZ;	//�}�b�v�`�b�v��Z�T�C�Y
};
