#include "../../framework.h"
#include "../../environment.h"

using namespace nsTitle;
using namespace nsStrategy;
using namespace nsBattle;

Scene* SceneManager::pScene = NULL;
Scene* SceneManager::pReservedScene = NULL;
eSceneTable SceneManager::currentScene = eSceneTable::None;
eSceneTable SceneManager::reservedScene = eSceneTable::Title; //�����V�[��

SceneManager::SceneManager() {
}
SceneManager::~SceneManager() {
}

void SceneManager::SwitchScene(eSceneTable scene) {
	reservedScene = scene;
}
//�V�[���̐؂�ւ�
void SceneManager::SwitchScene() {
	//�؂�ւ��\��̃`�F�b�N
	if (currentScene == reservedScene)
	{
		return;
	}

	//�V�[�����쐬���ď�����
	if (pReservedScene == nullptr) {
		//�V�[���̍쐬
		CreateReserveScene(reservedScene, true);
	}
	//���������I���΃V�[���؂�ւ�
	else {
		if (pReservedScene->isInitialized) {
			//�J�ڗ\��̃V�[���̎��(reservedScene)�����łɍ쐬���Ă���\��V�[���̎�ނƈႦ�Ώ������Ȃ�
			if (pReservedScene->GetSceneType() != (int)reservedScene) return;

			//�V�[���폜
			DeleteMainScene();

			//���݂̃V�[����ݒ�
			currentScene = reservedScene;

			//���C���V�[���ɗ\��V�[����n��
			pScene = pReservedScene;

			//�\��V�[����Null�ɂ���
			pReservedScene = nullptr;
		}
	}
}

//�\��V�[���̍쐬
void SceneManager::CreateReserveScene(eSceneTable scene, bool initialize) {
	if (pReservedScene != nullptr)	return;

	//�V�[���̍쐬
	switch (scene)
	{
	case eSceneTable::Title: pReservedScene = new SceneTitle(); break;
	case eSceneTable::Strategy:	pReservedScene = new SceneStrategy(); break;
	case eSceneTable::Battle:	pReservedScene = new SceneBattle(); break;
	}

	//�V���ɍ쐬�����V�[���̃I�u�W�F�N�g�}�l�[�W���[�ɃV�[���̎�ނ�n��
	pReservedScene->SetSceneType((int)scene);

	//�V�[���̏�����
	if(initialize) InitializeReserveScene();
}

//�\��V�[���̏�����
void SceneManager::InitializeReserveScene() {
	if (pReservedScene == NULL) return;
	if (pReservedScene->isInitialized) return;

	//�V�[���̏�����
	std::thread th(&Scene::Initialize, pReservedScene);

	th.detach();
}

//�V�[���̍폜
void SceneManager::DeleteScene() {
	if (pScene != NULL)
	{
		//�V�[���̏I��
		pScene->Terminate();
		delete pScene;
		pScene = NULL;
	}
	if (pReservedScene != NULL)
	{
		//�V�[���̏I��
		pReservedScene->Terminate();
		delete pReservedScene;
		pReservedScene = NULL;
	}
}
//�V�[���̍폜
void SceneManager::DeleteMainScene() {
	if (pScene != NULL)
	{
		//�V�[���̏I��
		pScene->Terminate();
		delete pScene;
		pScene = NULL;
	}
}
//�a���V�[���̍폜
void SceneManager::DeleteReserveScene() {
	if (pReservedScene != NULL)
	{
		//�V�[���̏I��
		pReservedScene->Terminate();
		delete pReservedScene;
		pReservedScene = NULL;
	}
}

//�V�[���̎擾
Scene* SceneManager::GetScene(int scene) {
	if (pScene != NULL && pScene->GetSceneType() == scene) return pScene;
	else if(pReservedScene != NULL && pReservedScene->GetSceneType() == scene) return pReservedScene;
	return pScene;
}