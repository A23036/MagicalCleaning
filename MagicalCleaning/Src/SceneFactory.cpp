#include "SceneFactory.h"
#include <windows.h>
#include <assert.h>
#include "TitleScene.h"
#include "PlayScene.h"
#include "EditScene.h"
#include "SplitScreen.h"
#include "SelectScene.h"
#include "ResultScene.h"

SceneBase* SceneFactory::CreateFirst()
{
	SplitScreen* ss = SingleInstantiate <SplitScreen>();	// SplitScreen�͑S�Ă̍ŏ��ɍ쐬�����B��������Ȃ��BNoDestroy�B
	ss->SetSingleScreen();
	//SingleInstantiate <DataCarrier>();	// DataCarrier�͑S�Ă̍ŏ��ɍ쐬�����B��������Ȃ��BNoDestroy�B

	return new TitleScene();
	return nullptr;
}

SceneBase * SceneFactory::Create(const std::string & name)
{
	if (name == "TitleScene") {
		ObjectManager::FindGameObject<SplitScreen>()->SetSingleScreen();
		return new TitleScene();
	}
	else if (name == "PlayScene") {
		ObjectManager::FindGameObject<SplitScreen>()->SetMultiScreen();
		//ObjectManager::FindGameObject<SplitScreen>()->SetSingleScreen();
		return new PlayScene();
	}
	else if (name == "EditScene") {
		ObjectManager::FindGameObject<SplitScreen>()->SetSingleScreen();
		return new EditScene();
	}
	else if (name == "SelectScene") {
		ObjectManager::FindGameObject<SplitScreen>()->SetSingleScreen();
		return new SelectScene();
	}
	else if (name == "ResultScene") {
		ObjectManager::FindGameObject<SplitScreen>()->SetSingleScreen();
		return new ResultScene();
	}
	assert(false);
	return nullptr;
}
