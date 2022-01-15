/*-----------------------------------------------------------

	�V�[���Ǘ��N���X
		�V�[���S�̂��Ǘ�����
		�V�[���J�ڂ�j�����s��

-------------------------------------------------------------*/
#pragma once
//�V�[���N���X��`�t�@�C��
#include "scene.h"
#include "Title/scene_title.h"
#include "Strategy/scene_strategy.h"
#include "Battle/scene_battle.h"

//�V�[���̎��
enum class eSceneTable
{
	None,
	Title,
	Strategy,
	Battle,
	SceneMax,
};

class SceneManager {
private:
	static Scene* pScene;
	static Scene* pReservedScene;

	static eSceneTable currentScene;
	static eSceneTable reservedScene;

public:
	SceneManager();
	~SceneManager();

	//�\��V�[���̍쐬(�����������s�\)
	static void CreateReserveScene(eSceneTable scene, bool initialize);

	//�\��V�[���̏�����
	static void InitializeReserveScene();

	//�V�[���̐؂�ւ��\��
	static void SwitchScene(eSceneTable scene);
	//�V�[���̐؂�ւ�
	static void SwitchScene();

	//�V�[���̍폜
	static void DeleteScene();
	static void DeleteMainScene();
	static void DeleteReserveScene();

	//Getter,Setter
	/// <summary>
	/// �V�[���̎��(eSceneTable)�ƈ�v����l�����V�[����Ԃ�
	/// </summary>
	static Scene* GetScene(int scene);

	//�`��ΏۃV�[��(pScene)��Ԃ�
	static Scene* GetMainScene() { return pScene; }
};