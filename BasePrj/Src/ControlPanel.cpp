#include "ControlPanel.h"
#include "StageEdit.h"

ControlPanel::ControlPanel()
{
	strcpy_s<256>(filename, "Stage1.csv");
}

ControlPanel::~ControlPanel()
{
}

void ControlPanel::Update()
{
	StageEdit* stage = ObjectManager::FindGameObject<StageEdit>();

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(300, 200));
	ImGui::Begin("Edit Control Panel");
	ImGui::InputText("Filename", filename, 32);
	if (ImGui::Button("Load")) {
		stage->Load(filename);
	}
	if (ImGui::Button("Save")) {
		stage->Save(filename);
	}
	ImGui::NewLine();
	ImGui::SliderInt3("Size", &size.x, 1, 40);
	if (ImGui::Button("Create")) {
		stage->Create(size);
	}
	ImGui::End();
}

void ControlPanel::Draw()
{
}
