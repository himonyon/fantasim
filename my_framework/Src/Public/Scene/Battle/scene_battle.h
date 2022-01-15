#pragma once
/*-----------------------------------------------------------

	�o�g���V�[���N���X(Scene�̔h��)

-------------------------------------------------------------*/

#include "Behaviour/Cursor.h"
#include "Behaviour/SelectSquare.h"
#include "Object/Square.h"
#include "Object/AStar.h"
#include "Behaviour/Charactor/BattleChara.h"
#include "Behaviour/Charactor/PlayerChara.h"
#include "Behaviour/Charactor/EnemyChara.h"
#include "Behaviour/Panel/StatusPanel.h"
#include "Behaviour/Panel/SelectSkillPanel.h"
#include "Behaviour/Panel/Command.h"
#include "Behaviour/Turn/Turn.h"
#include "Behaviour/Turn/PlayerTurn.h"
#include "Behaviour/Turn/EnemyTurn.h"
#include "Behaviour/FieldManager.h"

namespace nsBattle {
	enum class eRenderOrder {
		Backgorund,
		Object = 10,
		UI = 20,
	};

	class SceneBattle : public Scene {
	private:
		//�J����
		noDel_ptr<GameObject> pCam;

		//�J�[�\��
		noDel_ptr<GameObject> pCursor;

		//�w�i
		noDel_ptr<GameObject> pBg;

		//�t�B�[���h�}�b�v�}�l�[�W���[
		noDel_ptr<GameObject> pFieldManager;

		//�X�e�[�^�X�p�l��
		noDel_ptr<GameObject> pStatusPanel;

	public:
		//�L�����N�^�[�X�e�[�^�X�i�O�̃V�[����������p���j
		std::vector<noDel_ptr<Charactor>> vPlayerCharaBace;
		std::vector<noDel_ptr<Charactor>> vEnemyCharaBace;

	private:
		//������
		void Initialize();

		//�폜
		void Terminate();

		//����
		void Execute();

		//�`��
		void Render();
	};
}
