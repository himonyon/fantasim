#pragma once
/*-----------------------------------------------------------

	EnemyTurn�N���X(Turn)
	�G�^�[���̊Ǘ�

-------------------------------------------------------------*/
namespace nsBattle {

	class EnemyTurn : public Turn {
	private:
		//��x������������g���K�[
		bool onceTrg = false;

		//�^�[���J�n���ɕ\������I�u�W�F�N�g
		noDel_ptr<Animator> pTurnStartAnim;
		//�X�L���Z���N�g�p�l��
		noDel_ptr<SelectCounterPanel> pSelectCounterPanel;

		//�G�̔z��̉��Ԗڂ̑�������邩
		int enemyNum = 0;

		//�V�~�����[�g���Ԍv��
		Timer timer;

	public:
		void Awake() override;

		void Update() override;

		void TurnInit() override;

		//�^�[�Q�b�g�L�����̐ݒ�
		void SetTargetEnemy();
		void SetTargetFriend();

	private:
		//�e��Ԃ̏���
		void SelectSkill();
		void BeforeSkillFunc();
		void AfterSkillFunc();
		void SelectCounterFunc();
		void MoveFunc();
		void BattleFunc();
	};
}
