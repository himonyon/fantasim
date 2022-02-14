#include "../../../../framework.h"
#include "../../../../environment.h"

using namespace nsStrategy;

//初期化
void SceneStrategy::Initialize() {
	//ゲームマネージャー作成
	noDel_ptr<GameObject> _pGameManager = CreateObject(0, 0, 0, nullptr, "gameManager");
	_pGameManager->AddComponent<GameManager>();

	isInitialized = true;
}

void SceneStrategy::Terminate() {
}

//処理
void SceneStrategy::Execute() {
	Scene::Execute();
}

//描画
void SceneStrategy::Render() {
	Scene::Render();
}
