#pragma once
/*-----------------------------------------------------------

	PalyerTurn�N���X(Turn�̔h��)
	�v���C���[�^�[���̊Ǘ�

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
		//�T�E���h�}�l�[�W���[
		noDel_ptr<SoundManager> pSoundManager;
		//�J�[�\��
		noDel_ptr<Cursor> pCursor;

		//��ԕύX�������m�F�p
		eTurnState checkState = eTurnState::None;

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
