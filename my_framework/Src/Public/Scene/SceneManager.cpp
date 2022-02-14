#include "../../framework.h"
#include "../../environment.h"

using namespace nsTitle;
using namespace nsStrategy;
using namespace nsBattle;

Scene* SceneManager::pScene = NULL;
Scene* SceneManager::pReservedScene = NULL;
eSceneTable SceneManager::currentScene = eSceneTable::None;
eSceneTable SceneManager::reservedScene = eSceneTable::Title; //初期シーン

SceneManager::SceneManager() {
}
SceneManager::~SceneManager() {
}

void SceneManager::SwitchScene(eSceneTable scene) {
	reservedScene = scene;
}
//シーンの切り替え
void SceneManager::SwitchScene() {
	//切り替え予約のチェック
	if (currentScene == reservedScene)
	{
		return;
	}

	//シーンを作成して初期化
	if (pReservedScene == nullptr) {
		//シーンの作成
		CreateReserveScene(reservedScene, true);
	}
	//初期化が終わればシーン切り替え
	else {
		if (pReservedScene->isInitialized) {
			//遷移予定のシーンの種類(reservedScene)がすでに作成している予約シーンの種類と違えば処理しない
			if (pReservedScene->GetSceneType() != (int)reservedScene) return;

			//シーン削除
			DeleteMainScene();

			//現在のシーンを設定
			currentScene = reservedScene;

			//メインシーンに予約シーンを渡す
			pScene = pReservedScene;

			//予約シーンはNullにする
			pReservedScene = nullptr;
		}
	}
}

//予約シーンの作成
void SceneManager::CreateReserveScene(eSceneTable scene, bool initialize) {
	if (pReservedScene != nullptr)	return;

	//シーンの作成
	switch (scene)
	{
	case eSceneTable::Title: pReservedScene = new SceneTitle(); break;
	case eSceneTable::Strategy:	pReservedScene = new SceneStrategy(); break;
	case eSceneTable::Battle:	pReservedScene = new SceneBattle(); break;
	}

	//新たに作成したシーンのオブジェクトマネージャーにシーンの種類を渡す
	pReservedScene->SetSceneType((int)scene);

	//シーンの初期化
	if(initialize) InitializeReserveScene();
}

//予約シーンの初期化
void SceneManager::InitializeReserveScene() {
	if (pReservedScene == NULL) return;
	if (pReservedScene->isInitialized) return;

	//シーンの初期化
	std::thread th(&Scene::Initialize, pReservedScene);

	th.detach();
}

//シーンの削除
void SceneManager::DeleteScene() {
	if (pScene != NULL)
	{
		//シーンの終了
		pScene->Terminate();
		delete pScene;
		pScene = NULL;
	}
	if (pReservedScene != NULL)
	{
		//シーンの終了
		pReservedScene->Terminate();
		delete pReservedScene;
		pReservedScene = NULL;
	}
}
//シーンの削除
void SceneManager::DeleteMainScene() {
	if (pScene != NULL)
	{
		//シーンの終了
		pScene->Terminate();
		delete pScene;
		pScene = NULL;
	}
}
//預託シーンの削除
void SceneManager::DeleteReserveScene() {
	if (pReservedScene != NULL)
	{
		//シーンの終了
		pReservedScene->Terminate();
		delete pReservedScene;
		pReservedScene = NULL;
	}
}

//シーンの取得
Scene* SceneManager::GetScene(int scene) {
	if (pScene != NULL && pScene->GetSceneType() == scene) return pScene;
	else if(pReservedScene != NULL && pReservedScene->GetSceneType() == scene) return pReservedScene;
	return pScene;
}