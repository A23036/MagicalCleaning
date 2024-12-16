#include "StageData.h"

std::string models[] = {
	"wallEarth01",
	"wallEarth02",
	"wallStone01",
	"boxWooden",
	"boxCardboard",
	"groundSpike01",
	"boxGift1",
	"boxQuestion",
	"chestA",
	"chestB",
	"bottlePotionHealth",
	"bottlePotionPoison",
	"coin",
	"coinProcJam",
	"crown",
	"hammer",
	"heart",
	"flagA",
	"key",
	"char01",
	"char02",
	"char03",
	"char04",
};

std::string folder = "data/";

StageData::StageData() : map(20, std::vector<std::vector<int>>(2, std::vector<int>(20)))
{
	size = IVECTOR3(20, 2, 20);
	for (auto s : models) {
		CFbxMesh* mesh = new CFbxMesh();
		mesh->Load((folder+"models/"+s +".mesh").c_str());
		meshes.push_back(mesh);
	}
	for (int x = 0; x < 20; x++) {
		for (int z = 0; z < 20; z++) {
			map[z][0][x] = 0;
			map[z][1][x] = -1;
		}
	}
}

StageData::~StageData()
{
}

void StageData::Create(IVECTOR3 _size)
{
	map.clear();
	size = _size;
	for (int z = 0; z < size.z; z++) {
		std::vector<std::vector<int>> v2;
		for (int y = 0; y < size.y; y++) {
			std::vector<int> v;
			for (int x = 0; x < size.x; x++) {
				v.push_back(-1);
			}
			v2.push_back(v);
		}
		map.push_back(v2);
	}
}

void StageData::DrawBox(VECTOR3 pos, DWORD color)
{
	CSprite spr;
	spr.DrawLine3D(pos + VECTOR3(-0.5f, -0.0f, -0.5f), pos + VECTOR3( 0.5f, -0.0f, -0.5f), color);
	spr.DrawLine3D(pos + VECTOR3( 0.5f, -0.0f, -0.5f), pos + VECTOR3( 0.5f,  1.0f, -0.5f), color);
	spr.DrawLine3D(pos + VECTOR3( 0.5f,  1.0f, -0.5f), pos + VECTOR3(-0.5f,  1.0f, -0.5f), color);
	spr.DrawLine3D(pos + VECTOR3(-0.5f,  1.0f, -0.5f), pos + VECTOR3(-0.5f, -0.0f, -0.5f), color);
	spr.DrawLine3D(pos + VECTOR3(-0.5f, -0.0f,  0.5f), pos + VECTOR3( 0.5f, -0.0f,  0.5f), color);
	spr.DrawLine3D(pos + VECTOR3( 0.5f, -0.0f,  0.5f), pos + VECTOR3( 0.5f,  1.0f,  0.5f), color);
	spr.DrawLine3D(pos + VECTOR3( 0.5f,  1.0f,  0.5f), pos + VECTOR3(-0.5f,  1.0f,  0.5f), color);
	spr.DrawLine3D(pos + VECTOR3(-0.5f,  1.0f,  0.5f), pos + VECTOR3(-0.5f, -0.0f,  0.5f), color);
	spr.DrawLine3D(pos + VECTOR3(-0.5f, -0.0f,  0.5f), pos + VECTOR3(-0.5f, -0.0f, -0.5f), color);
	spr.DrawLine3D(pos + VECTOR3( 0.5f, -0.0f,  0.5f), pos + VECTOR3( 0.5f, -0.0f, -0.5f), color);
	spr.DrawLine3D(pos + VECTOR3(-0.5f,  1.0f,  0.5f), pos + VECTOR3(-0.5f,  1.0f, -0.5f), color);
	spr.DrawLine3D(pos + VECTOR3( 0.5f,  1.0f,  0.5f), pos + VECTOR3( 0.5f,  1.0f, -0.5f), color);
}
