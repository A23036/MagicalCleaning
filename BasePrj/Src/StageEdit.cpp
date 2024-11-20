#include "StageEdit.h"
#include <fstream>
#include "CsvReader.h"
#include "BlockFileName.h"

StageEdit::StageEdit()
{
	stageNumber = 2;
	for (const std::string& f : files) {
		CFbxMesh* m = new CFbxMesh();
		std::string folder = "data/models/"; // 一旦C++の文字列にする
		m->Load((folder+f+".mesh").c_str()); // +で文字列をつなげることができる
		meshes.push_back(m);
	}

	// 最初のmapデータを作る
	for (int y = 0; y < 1; y++) {
		vector<vector<int>> m2;
		for (int z = 0; z < 3; z++) {
			vector<int> m;
			for (int x = 0; x < 4; x++) {
				m.push_back(0);
			}
			m2.push_back(m);
		}
		map.push_back(m2);
	}
	map[0][1][1] = 1;
	map[0][2][3] = 1;
	cursorX = 0;
	cursorZ = 0;

	GameDevice()->m_mView = XMMatrixLookAtLH(
		VECTOR3(0, 10, 5), // カメラ位置
		VECTOR3(0, 0, 0), // 注視点
		VECTOR3(0, 1, 0)); // 上ベクトル
}

StageEdit::~StageEdit()
{
	for (int i = 0; i < meshes.size(); i++) {
		delete meshes[i];
	}
}

void StageEdit::Update()
{
	CDirectInput* di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_TRG, DIK_D)) {
		cursorX = min(cursorX + 1, map[cursorY][cursorZ].size() - 1);
	}
	if (di->CheckKey(KD_TRG, DIK_A)) {
		cursorX = max(cursorX - 1, 0);
	}
	if (di->CheckKey(KD_TRG, DIK_S)) {
		cursorZ++;
		if (cursorZ >= map[cursorY].size()) {
			cursorZ = map[cursorY].size() - 1;
		}
	}
	if (di->CheckKey(KD_TRG, DIK_W)) {
		cursorZ--;
		if (cursorZ < 0) {
			cursorZ = 0;
		}
	}
	if (di->CheckKey(KD_TRG, DIK_1)) {
		cursorY = min(cursorY + 1, map.size() - 1);
	}
	if (di->CheckKey(KD_TRG, DIK_2)) {
		cursorY = max(cursorY - 1, 0);
	}
	if (di->CheckKey(KD_TRG, DIK_SPACE)) {
		map[cursorY][cursorZ][cursorX]++;
		if (map[cursorY][cursorZ][cursorX] >= meshes.size()) {
			map[cursorY][cursorZ][cursorX] = -1;
		}
	}
	if (di->CheckKey(KD_TRG, DIK_UP)) {
		map[cursorY][cursorZ][cursorX] = CHAR01;
	}
	if (di->CheckKey(KD_TRG, DIK_LEFT)) {
		map[cursorY][cursorZ][cursorX] = CHAR02;
	}
	if (di->CheckKey(KD_TRG, DIK_DOWN)) {
		map[cursorY][cursorZ][cursorX] = CHAR03;
	}
	if (di->CheckKey(KD_TRG, DIK_RIGHT)) {
		map[cursorY][cursorZ][cursorX] = CHAR04;
	}
	if (di->CheckKey(KD_TRG, DIK_RCONTROL) || di->CheckKey(KD_TRG, DIK_LCONTROL)) {
		map[cursorY][cursorZ][cursorX] = -1;
	}


//	if (di->CheckKey(KD_TRG, DIK_O)) {
//		Save();
//	}
//	if (di->CheckKey(KD_TRG, DIK_I)) {
//		Load();
//	}

	ImGui::Begin("MENU");
	ImGui::InputInt("Stage", &stageNumber);
	if (ImGui::Button("SAVE")) {
		Save(stageNumber);
	}
	if (ImGui::Button("LOAD")) {
		Load(stageNumber);
	}
	ImGui::InputInt("XSIZE", &xsize);
	ImGui::InputInt("YSIZE", &ysize);
	ImGui::InputInt("ZSIZE", &zsize);
	if (ImGui::Button("CREATE")) {
		Create(xsize, ysize, zsize);
	}

	//現在のマップ情報
	int x = map[0][0].size();
	int y = map.size();
	int z = map[0].size();
	ImGui::InputInt("curX", &x);
	ImGui::InputInt("curY", &y);
	ImGui::InputInt("curZ", &z);
//	ImGui::Begin("CURSOR");
//	ImGui::InputInt("X", &cursorX);
//	ImGui::InputInt("Z", &cursorZ);
	ImGui::End();
}

void StageEdit::Draw()
{
	GameDevice()->m_mView = XMMatrixLookAtLH(
		VECTOR3(cursorX , cursorY + 15, -cursorZ - 10), // カメラ位置
		VECTOR3(cursorX, cursorY, -cursorZ), // 注視点
		VECTOR3(0, 1, 0)); // 上ベクトル

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
	DrawBox(VECTOR3(cursorX, cursorY, -cursorZ), 0xff0000ff/*ABGR*/);
}

void StageEdit::Save(int n)
{
	char name[64];
	sprintf_s<64>(name, "data/stage/Stage%02d.csv", n);
	// ファイルを開く
	std::ofstream ofs(name); // 引数にファイル名
	// データを書く
	for (int y = 0; y < map.size(); y++) {
		for (int z = 0; z < map[y].size(); z++) {
			int xs = map[y][z].size();
			for (int x = 0; x < xs; x++) {
				ofs << map[y][z][x];
				if (x < xs - 1) {
					ofs << ",";
				}
			}
			ofs << std::endl;
		}
		ofs << std::endl;
	}
	// ファイルを閉じる
	ofs.close();
}

void StageEdit::Load(int n)
{
	char name[64];
	sprintf_s<64>(name, "data/stage/Stage%02d.csv", n);

	CsvReader* csv = new CsvReader(name);
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
				m.push_back(d);
			}
			m2.push_back(m);
		}
	}
	if (m2.size() > 0) {
		map.push_back(m2);
	}
	delete csv;
}

void StageEdit::Create(int xsize, int ysize, int zsize)
{
	map.clear();
	for (int y = 0; y < ysize; y++) {
		vector<vector<int>> m2;
		for (int z = 0; z < zsize; z++) {
			vector<int> m;
			for (int x = 0; x < xsize; x++) {
				if (y==0)
					m.push_back(0);
				else
					m.push_back(-1);
			}
			m2.push_back(m);
		}
		map.push_back(m2);
	}
	//外周をすべて-1にする
}


void StageEdit::DrawBox(VECTOR3 pos, DWORD color)
{
	CSprite spr;
	spr.DrawLine3D(pos + VECTOR3(-0.5f, -0.0f, -0.5f), pos + VECTOR3(0.5f, -0.0f, -0.5f), color);
	spr.DrawLine3D(pos + VECTOR3(0.5f, -0.0f, -0.5f), pos + VECTOR3(0.5f, 1.0f, -0.5f), color);
	spr.DrawLine3D(pos + VECTOR3(0.5f, 1.0f, -0.5f), pos + VECTOR3(-0.5f, 1.0f, -0.5f), color);
	spr.DrawLine3D(pos + VECTOR3(-0.5f, 1.0f, -0.5f), pos + VECTOR3(-0.5f, -0.0f, -0.5f), color);
	spr.DrawLine3D(pos + VECTOR3(-0.5f, -0.0f, 0.5f), pos + VECTOR3(0.5f, -0.0f, 0.5f), color);
	spr.DrawLine3D(pos + VECTOR3(0.5f, -0.0f, 0.5f), pos + VECTOR3(0.5f, 1.0f, 0.5f), color);
	spr.DrawLine3D(pos + VECTOR3(0.5f, 1.0f, 0.5f), pos + VECTOR3(-0.5f, 1.0f, 0.5f), color);
	spr.DrawLine3D(pos + VECTOR3(-0.5f, 1.0f, 0.5f), pos + VECTOR3(-0.5f, -0.0f, 0.5f), color);
	spr.DrawLine3D(pos + VECTOR3(-0.5f, -0.0f, 0.5f), pos + VECTOR3(-0.5f, -0.0f, -0.5f), color);
	spr.DrawLine3D(pos + VECTOR3(0.5f, -0.0f, 0.5f), pos + VECTOR3(0.5f, -0.0f, -0.5f), color);
	spr.DrawLine3D(pos + VECTOR3(-0.5f, 1.0f, 0.5f), pos + VECTOR3(-0.5f, 1.0f, -0.5f), color);
	spr.DrawLine3D(pos + VECTOR3(0.5f, 1.0f, 0.5f), pos + VECTOR3(0.5f, 1.0f, -0.5f), color);
}
