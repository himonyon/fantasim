#include "../../../../framework.h"
#include "../../../../environment.h"

using namespace nsTitle;

//‰Šú‰»
void SceneTitle::Initialize() {
	pBg = CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, SCREEN_WIDTH, SCREEN_HEIGHT, 
		CreateSprite(new Sprite(L"Data/Image/Title/title.spr")));
	pBg->AddComponent<TitleManager>();

	isInitialized = true;
}

void SceneTitle::Terminate() {
}

//ˆ—
void SceneTitle::Execute() {
	Scene::Execute();
}

//•`‰æ
void SceneTitle::Render() {
	Scene::Render();
}
