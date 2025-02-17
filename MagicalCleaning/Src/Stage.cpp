#include "Stage.h"
#include "CsvReader.h"
#include "MeshCollider.h"
#include "BlockFileName.h"
#include "Player.h"
#include "Leaf.h"
#include "Collision.h"
#include "SplitScreenLastDraw.h"
#include <iostream>
#include <fstream>


using namespace std;

Stage::Stage()
{
	SplitScreenLastDraw* ssld = ObjectManager::FindGameObject<SplitScreenLastDraw>();

	//ランダムなマップチップ配列の作成
	GenerateRandomMap(MapNum);

	ssld->LoadMap();

	mesh = new CFbxMesh();
	mesh->Load("data/Map2/MapChip/stage_wall.mesh");
	mapCol->AddFbxLoad(mesh, XMMatrixTranslation(0, 0, 0)); //移動させる場合、引数を追加
	meshes.push_back(mesh);
}

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
	
	//cm = ObjectManager::FindGameObject<Camera>();

	Load(stageNumber);
}

Stage::~Stage()
{
//	if (csv != nullptr) {
//		delete csv;
//		csv = nullptr;
//	}
	SAFE_DELETE(csv);
	SAFE_DELETE(mapCol);
	for (int i = 0; i < meshes.size(); i++){
		SAFE_DELETE(meshes[i]);
	}
}

void Stage::Update()
{
}

void Stage::Draw()
{
	
	int i = 0;
	for (CFbxMesh* m : meshes) {
		if (i == 4) {
			m->Render(XMMatrixTranslation(0, 0, 0));
		}
		else{
			m->Render(XMMatrixRotationY(mapRotData[i] * 90 * DegToRad) * XMMatrixTranslation(-25 + i % 2 * 50, 0, -25 + i / 2 * 50));
		}
		i++;
	}
}

void Stage::Load() 
{
	/*
	map.clear();
	int baseX = 0;
	int baseY = 0;

	char name[64];
	vector<vector<int>> m2;

	const int mapWidth = size * chipX;
	const int mapHeight= chipY;
	const int mapDepth = size * chipZ;
	
	vector<vector<vector<int>>> fullMap(mapHeight, vector<vector<int>>(mapDepth, vector<int>(mapWidth, -1)));


	// mapChipsを基に三次元マップデータを作成
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			sprintf_s<64>(name, "data/Map/Stage%02d.csv", mapChips[j][i]);
			csv = new CsvReader(name);
			int layerIndex = 0;  // Layer のインデックス

			// CSV データの行を順番に処理
			for (int line = 0; line < csv->GetLines(); line++) {
				if (csv->GetString(line, 0) == "") {
					// 空行は無視
					continue;
				}
				else {
					vector<int> m;
					for (int x = 0; x < csv->GetColumns(line); x++) {
						int d = csv->GetInt(line, x);

						// プレイヤーやアイテムの処理
						if (d == boxID::CHAR01) {
							Player* p = new Player(0);
							p->SetPosition(x, map.size(), -(int)m2.size());
							p->SetTag("Player1");
							d = -1;
						}
						else if (d == boxID::CHAR02) {
							Player* p = new Player(1);
							p->SetPosition(x, map.size(), -(int)m2.size());
							p->SetTag("Player2");
							d = -1;
						}
						else if (d == boxID::CHAR03) {
							Player* p = new Player(2);
							p->SetPosition(x, map.size(), -(int)m2.size());
							p->SetTag("Player3");
							d = -1;
						}
						else if (d == boxID::CHAR04) {
							Player* p = new Player(3);
							p->SetPosition(x, map.size(), -(int)m2.size());
							p->SetTag("Player4");
							d = -1;
						}
						else if (d == boxID::CHEST_A) {
							DustBox* b = new DustBox();
							b->SetPosition(x, map.size(), -(int)m2.size());
							d = -1;
						}
						else if (d == boxID::WOOD) {
							int r = Random(0, 2);
							Dust* du = new Dust(r, VECTOR3(x, map.size(), -(int)m2.size()));
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
			// `mapChips` の位置にデータを配置
				// (i, j) の位置に対応するマップチップを `fullMap` に格納
			for (int y = 0; y < m2.size(); y++) {
				for (int x = 0; x < m2[y].size(); x++) {
					// `mapChips[j][i]` を使って `fullMap` の位置にマップチップを配置
					fullMap[layerIndex][baseY + y][baseX + x] = m2[y][x];
				}
			}
			m2.clear();
			layerIndex++;
			delete csv;
		}

	}
	
	// 完成したマップデータを保存
	SaveMapChips(fullMap, "data/Map/CreatedMap.csv");
	*/
}

void Stage::Load(int n)
{
	/*
	char name[64];
	sprintf_s<64>(name, "data/stage/Stage%02d.csv", n);

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
					Player* p = new Player(0);
					p->SetPosition(x, map.size(), -(int)m2.size());
					p->SetTag("Player1");
					d = -1;
				}
				else if (d == boxID::CHAR02) {
					Player* p = new Player(1);
					p->SetPosition(x, map.size(), -(int)m2.size());
					p->SetTag("Player2");
					d = -1;
				}
				else if (d == boxID::CHAR03) {
					Player* p = new Player(2);
					p->SetPosition(x, map.size(), -(int)m2.size());
					p->SetTag("Player3");
					d = -1;
				}
				else if (d == boxID::CHAR04) {
					Player* p = new Player(3);
					p->SetPosition(x, map.size(), -(int)m2.size());
					p->SetTag("Player4");
					d = -1;
				}
				else if (d == boxID::CHEST_A) {
					DustBox* b = new DustBox();
					b->SetPosition(x, map.size(), -(int)m2.size());
					d = -1;
				}
				else if (d == boxID::WOOD) {
					int r = Random(0, 2);
					Dust* du = new Dust(r,VECTOR3(x, map.size(), -(int)m2.size()));
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
	*/
}

bool Stage::IsLandBlock(VECTOR3 pos)
{
	VECTOR3 to = pos;
	to.y -= 0.01f;
	pos.y += 1.0f;
	
	if (HitLineToMesh(pos,to)) {
		return true;
	}
	else {
		return false;
	}
	return true;
}

bool Stage::HitSphere(const SphereCollider& coll, VECTOR3* out)
{
	VECTOR3 pushVec = VECTOR3(0, 0, 0);
	VECTOR3 pushVecCorner = VECTOR3(0, 0, 0);
	bool pushed = false;
	for (int y = 0; y < map.size(); y++) {
		for (int z = 0; z < map[y].size(); z++) {
			for (int x = 0; x < map[y][z].size(); x++) {
				if (map[y][z][x] >= 0) {
					VECTOR3 dist = coll.center - VECTOR3(x, y, -z);
					if (dist.LengthSquare() >= (coll.radius + 1.5f) * (coll.radius + 1.5f))
						continue;
					MATRIX4X4 mat = XMMatrixTranslation(x, y, -z);
					std::list<MeshCollider::CollInfo> meshes = boxCollider->CheckCollisionSphereList(mat, coll.center, coll.radius);
					for (const MeshCollider::CollInfo& m : meshes) {
						VECTOR3 move = coll.center - m.hitPosition;
						VECTOR3 v = XMVector3Cross(move, m.normal);
						if (v.Length() <= 0.0001f) {
							float len = move.Length(); // 当たった点から中心への距離
							if (len > 0.0f) {
								move = move * ((coll.radius - len) / len);
							}
							VECTOR3 push = m.normal * Dot(move, m.normal); // 押し返したいベクトル
							// 今のpushVecと合成する
							VECTOR3 pushVecNorm = XMVector3Normalize(pushVec); // 合成済みベクトルの向き
							float dot = Dot(push, pushVecNorm);	// その成分の長さ
							//							if (dot < pushVec.Length()) {
							pushVec += push - pushVecNorm * dot; // その成分を減らしていい
							//							}
							//							else {
							//								pushVec = push;
							//							}
						}
						else {
							float len = move.Length(); // 当たった点から中心への距離
							move = move * ((coll.radius - len) / len);
							VECTOR3 push = m.normal * Dot(move, m.normal); // 押し返したいベクトル
							// 今のpushVecと合成する
							VECTOR3 pushVecNorm = XMVector3Normalize(pushVecCorner); // 合成済みベクトルの向き
							float dot = Dot(push, pushVecNorm);	// その成分の長さ
							//							if (dot < pushVecCorner.Length()) {
							pushVecCorner += push - pushVecNorm * dot; // その成分を減らしていい
							//							}
							//							else {
							//								pushVecCorner = push;
							//							}
						}
						pushed = true;
					}
				}
			}
		}
	}
	if (pushed && out != nullptr) {
		if (pushVec.LengthSquare() > 0.0f) {
			*out = pushVec;
		}
		else {
			*out = pushVecCorner;
		}
	}
	return pushed;
}

void Stage::GenerateRandomMap(int num)
{
	mapCol = new CCollision;

	// ランダムなマップチップの配置
	for (int i = 0; i < 4; i++) {
		mesh = new CFbxMesh();
		mapData[i] = Random(1, num);	//マップデータからランダム選択
		mapRotData[i] = Random(0, 3);	//回転量の設定

		std::string f = "data/Map2/MapChip/stage0"+ std::to_string(mapData[i]);
		mesh->Load((f + ".mesh").c_str());

		//回転量、配置を考慮してデータ配列に格納
		mapCol->AddFbxLoad(mesh, XMMatrixRotationY(mapRotData[i] * 90 * DegToRad) * XMMatrixTranslation(-25 + i%2*50, 0, -25 + i/2*50));
		meshes.push_back(mesh);
	}
}
