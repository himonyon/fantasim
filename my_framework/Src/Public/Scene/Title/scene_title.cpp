#include "../../../../framework.h"
#include "../../../../environment.h"

using namespace nsTitle;

//初期化
void SceneTitle::Initialize() {
	pBg = CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, SCREEN_WIDTH, SCREEN_HEIGHT, 
		CreateSprite(new Sprite(L"Data/Image/Title/title.spr")));
	pBg->AddComponent<TitleManager>();

	isInitialized = true;
}

void SceneTitle::Terminate() {
}

//処理
void SceneTitle::Execute() {
	Scene::Execute();
}

//描画
void SceneTitle::Render() {
	Scene::Render();
}
