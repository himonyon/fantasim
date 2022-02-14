#include "../../../../framework.h"
#include "../../../../environment.h"

using namespace nsStrategy;

//������
void SceneStrategy::Initialize() {
	//�Q�[���}�l�[�W���[�쐬
	noDel_ptr<GameObject> _pGameManager = CreateObject(0, 0, 0, nullptr, "gameManager");
	_pGameManager->AddComponent<GameManager>();

	isInitialized = true;
}

void SceneStrategy::Terminate() {
}

//����
void SceneStrategy::Execute() {
	Scene::Execute();
}

//�`��
void SceneStrategy::Render() {
	Scene::Render();
}
