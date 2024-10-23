#include "SceneFactory.h"
#include <windows.h>
#include <assert.h>
#include "TitleScene.h"
#include "PlayScene.h"
#include "EditScene.h"

SceneBase* SceneFactory::CreateFirst()
{
	return new TitleScene();
	return nullptr;
}

SceneBase * SceneFactory::Create(const std::string & name)
{
	if (name == "TitleScene") {
		return new TitleScene();
	}
	else if (name == "PlayScene") {
		return new PlayScene();
	}
	else if (name == "EditScene") {
		return new EditScene();
	}
	assert(false);
	return nullptr;
}
