#include "Stage.h"
#include "CsvReader.h"
#include "BlockFileName.h"
#include "Player.h"
#include "Camera.h"
#include "Coin.h"
#include "Dust.h"
#include "DustBox.h"
#include "Collision.h"
#include <iostream>
#include <fstream>


using namespace std;

Stage::Stage()
{
	size = 7;
	chipX = 10;
	chipY = 5;
	chipZ = 10;

	//ランダムなマップチップ配列の作成
	//GenerateRandomMap(15,size);

	SetTag("STAGEOBJ");

	mesh = new CFbxMesh();
	mesh->Load("data/Map2/map50Field1.mesh");

	//meshCol = new MeshCollider();
	//meshCol->MakeFromMesh(mesh); //メッシュを作成

	mapCol = new CCollision;	// -- 2024.12.2
	mapCol->AddFbxLoad(mesh);	// -- 2024.12.2 


	this->SetPosition(0, 0, 0); //移動させる場合、mapCol->AddFbxLoad(mesh)に引数を追加
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
	
	cm = ObjectManager::FindGameObject<Camera>();

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
}

void Stage::Update()
{
	//現在のマップ情報
	/*
	ImGui::Begin("mapsize");
	int x = map.size();
	int y = map[0].size();
	int z = map[0][0].size();
	ImGui::InputInt("X", &x);
	ImGui::InputInt("Y", &y);
	ImGui::InputInt("Z", &z);
	ImGui::End();
	*/
}

void Stage::Draw()
{
	/*
	// 画面ごとにプレイヤー周辺のみマップ描画
	int num = ObjectManager::DrawCounter(); 
	std::string s = "Player" + std::to_string(num+1);
	Player* pl = ObjectManager::FindGameObjectWithTag<Player>(s);
	VECTOR3 pos = pl->Position();
	
	const int radius = 25; // プレイヤーからの描画範囲を指定

	// 描画範囲の計算
	int minX = max(0, static_cast<int>(pos.x) - radius);
	int maxX = min(static_cast<int>(map[0][0].size()) - 1, static_cast<int>(pos.x) + radius);
	int minY = max(0, static_cast<int>(pos.y) - radius);
	int maxY = min(static_cast<int>(map.size()) - 1, static_cast<int>(pos.y) + radius);
	int minZ = max(0, static_cast<int>(-pos.z) - radius);
	int maxZ = min(static_cast<int>(map[0].size()) - 1, static_cast<int>(-pos.z) + radius);
	
	int i = 0;
	int j = 0;
	for (int y = minY; y <= maxY; y++) {
		for (int z = minZ; z <= maxZ; z++) {
			for (int x = minX; x <= maxX; x++) {
				i++;
				VECTOR3 dist = pos - VECTOR3(x, y, -z);

				// 特定の範囲内だけ描画
				if (dist.LengthSquare() >= 500)
					continue;
				j++;
				// チップの描画処理
				int chip = map[y][z][x];
				if (chip >= 0) { // 負の時は穴
					meshes[chip]->Render(XMMatrixTranslation(x, y, -z));
				}
			}
		}
	}
	*/
	mesh->Render(transform.matrix());
}

void Stage::Load() 
{
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
}

void Stage::Load(int n)
{
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

	/*
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
	*/
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

void Stage::GenerateRandomMap(int totalMaps, int size)
{
	// 配列の中身を-1で初期化
  	mapChips.assign(size, vector<int>(size, -1));

	// プレイヤースタート位置 (固定配置)
	mapChips[0][size / 2]			= 30;
	mapChips[size / 2][size - 1]	= 31;
	mapChips[size - 1][size / 2]	= 32;
	mapChips[size / 2][0]			= 33;
	

	// ランダムなマップチップの配置
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (mapChips[j][i] != -1) {	//空でない場合continue
				continue;
			}
			mapChips[j][i] = Random(10, totalMaps + 10);
		}
	}

	// 結果をCSVファイルとして出力(確認用)
	SaveMapChips(mapChips, "data/Map/GeneratedMap.csv");
}

void Stage::SaveMapChips(const vector<vector<int>>& mapChips, const string& filePath) //マップチップ配列用
{
	ofstream file(filePath);

	if (!file.is_open()) {
		std::cerr << "Error: Could not open file " << filePath << " for writing." << std::endl;
		return;
	}

	// マップデータを書き込む
	for (const auto& row : mapChips) {
		for (size_t i = 0; i < row.size(); ++i) {
			file << row[i];
			if (i < row.size() - 1) {
				file << ",";
			}
		}
		file << "\n";
	}

	file.close();
}

void Stage::SaveMapChips(const vector<vector<vector<int>>>& mapChips, const string& filePath) //マップ用
{
	ofstream file(filePath);

	if (!file.is_open()) {
		std::cerr << "Error: Could not open file " << filePath << " for writing." << std::endl;
		return;
	}

	// 三次元配列の各層を順番に書き込む
	for (const auto& layer : mapChips) {
		for (const auto& row : layer) {
			for (size_t i = 0; i < row.size(); ++i) {
				file << row[i];
				if (i < row.size() - 1) {
					file << ",";  // 行の要素をカンマ区切り
				}
			}
			file << "\n";  // 行の終わりで改行
		}
		file << "\n";  // 層の終わりで空行（区切り）
	}

	file.close();
}

