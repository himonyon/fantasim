#pragma once
/*-----------------------------------------------------------

	BattleChara�N���X(Behaviour�̔h��)
	�o�g���Ŏg�p����L�����̋���

-------------------------------------------------------------*/
namespace nsBattle {
	class PlayerTurn : public Turn {
	private:
		//�R�}���h�p�l��
		noDel_ptr<CommandPanel> pCommandPanel;
		//�X�L���Z���N�g�p�l��
		noDel_ptr<SelectSkillPanel> pSkillSelectPanel;
		//�^�[���J�n���ɕ\������I�u�W�F�N�g
		noDel_ptr<Animator> pTurnStartAnim;

		//�����I�u�W�F�N�g----------------------
		noDel_ptr<GameObject> pStatusPanel;
		//�J�[�\��
		noDel_ptr<Cursor> pCursor;

	private:
		void Awake() override;

		void Start() override;

		void Update() override;

		//�^�[���J�n������
		void TurnInit() override;

		//�L�����I��
		void SelectChara();

		//�L�����̃J�[�\��������Ď��̋���
		void ShowCharaInfo();

		//�e��Ԃ̏���
		void FieldFunc();
		void BeforeCommandFunc();
		void AfterCommandFunc();
		void MoveFunc();
		void SkillFunc();
		void SelectSkillFunc();
		void BattleFunc();
	};
}
