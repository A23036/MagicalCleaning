#include "Stage.h"
#include "CsvReader.h"
#include "BlockFileName.h"
#include "Player.h"
#include "Coin.h"

Stage::Stage(int stageNumber)
{
	for (const std::string& f : files) {
		CFbxMesh* m = new CFbxMesh();
		std::string folder = "data/models/"; // 一旦C++の文字列にする
		m->Load((folder + f + ".mesh").c_str()); // +で文字列をつなげることができる
		meshes.push_back(m);
	}
	boxCollider = new MeshCollider();
	boxCollider->MakeFromFile("data/models/boxCol.mesh");

	Load(stageNumber);
}

Stage::~Stage()
{
//	if (csv != nullptr) {
//		delete csv;
//		csv = nullptr;
//	}
	SAFE_DELETE(csv);
}

void Stage::Update()
{
}

void Stage::Draw()
{
	for (int y = 0; y < map.size(); y++) {
		for (int z = 0; z < map[y].size(); z++) {
			for (int x = 0; x < map[y][z].size(); x++) {
				int chip = map[y][z][x];
				if (chip >= 0) { // 負の時は穴
					meshes[chip]->Render(XMMatrixTranslation(x, y, -z));
				}
			}
		}
	}
}

void Stage::Load(int n)
{
	char name[64];
	sprintf_s<64>(name, "data/Stage%02d.csv", n);

	SAFE_DELETE(csv);
	csv = new CsvReader(name);
	map.clear();

	std::vector<std::vector<int>> m2;
	m2.clear();
	for (int line = 0; line < csv->GetLines(); line++) {
		if (csv->GetString(line, 0) == "") {
			map.push_back(m2);
			m2.clear();
		}
		else {
			std::vector<int> m;
			for (int x = 0; x < csv->GetColumns(line); x++) {
				int d = csv->GetInt(line, x);
				if (d == boxID::CHAR01) {
					Player* p = new Player();
					p->SetPosition(x, map.size(), -(int)m2.size());
					d = -1;
				}
				else if (d == boxID::COIN) {
					Coin* c = new Coin();
					c->SetPosition(x, map.size(), -(int)m2.size());
					d = -1;
				} 
				m.push_back(d);
			}
			m2.push_back(m);
		}
	}
	if (m2.size() > 0) {
		map.push_back(m2);
	}
}

bool Stage::IsLandBlock(VECTOR3 pos)
{
	if (csv == nullptr) {
		return false;
	}
	if (-pos.z + 0.5f < 0.0f) {
		return false;
	}
	int chipZ = (int)(-pos.z + 0.5f);
	if (pos.x + 0.5f < 0.0f) {
		return false;
	}
	int chipX = (int)(pos.x+0.5f);

	if (pos.y < 0.0f) {
		return false;
	}
	int chipY = (int)pos.y;
	if (chipY >= map.size()) {
		return false;
	}

	if (chipZ >= map[chipY].size()) {
		return false;
	}
	if (chipX >= map[chipY][chipZ].size()) {
		return false;
	}
	if (map[chipY][chipZ][chipX]>=0) {
		return true;
	}
	return false;
}

bool Stage::HitSphere(const SphereCollider& coll, VECTOR3* out)
{
	VECTOR3 pushVec = VECTOR3(0, 0, 0);
	VECTOR3 pushVecCorner = VECTOR3(0, 0, 0);
	bool pushed = false;
	bool pushedCorner = false;

	for (int y = 0; y < map.size(); y++) {
		for (int z = 0; z < map[y].size(); z++) {
			for (int x = 0; x < map[y][z].size(); x++) {
				if (map[y][z][x] >= 0) {
					// マップの座標に基づく変換行列を作成
					MATRIX4X4 mat = XMMatrixTranslation(x, y, -z);

					// 衝突判定を行う
					std::list<MeshCollider::CollInfo> meshes = boxCollider->CheckCollisionSphereList(mat, coll.center, coll.radius);

					// 衝突したすべてのメッシュに対して処理を行う
					for (const MeshCollider::CollInfo& m : meshes) {
						VECTOR3 move = coll.center - m.hitPosition;
						float len = move.Length(); // 当たった点から中心への距離

						if (len < coll.radius) {
							move = move * ((coll.radius - len) / len);
							VECTOR3 push = m.normal * Dot(move, m.normal); // 押し返しベクトルを計算

							// 押し返しベクトルの合成
							if (m.normal.Length() > 0.0f) {
								VECTOR3 pushVecNorm = XMVector3Normalize(pushVec); // 合成済みベクトルの向き
								float dot = Dot(push, pushVecNorm);	// その成分の長さ

								// 合成ベクトルに加える
								pushVec += push - pushVecNorm * dot;
							}
							else {
								VECTOR3 pushVecCornerNorm = XMVector3Normalize(pushVecCorner); // 合成済みベクトルの向き
								float dotCorner = Dot(push, pushVecCornerNorm); // その成分の長さ

								// コーナーの押し返しベクトルに加える
								pushVecCorner += push - pushVecCornerNorm * dotCorner;
							}

							pushedCorner = true;
						}

						pushed = true;
					}
				}
			}
		}
	}

	// 衝突があった場合に押し返しベクトルをoutに代入
	if (pushed && out != nullptr) {
		if (pushedCorner) {
			*out = pushVec;
		}
		else {
			*out = pushVecCorner;
		}
	}

	return pushed;
}

