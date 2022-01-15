#pragma once
/*-----------------------------------------------------------

	Command�N���X(Behaviour�̔h��)
	�R�}���h�ꗗ�̕\���Ǝ��s

-------------------------------------------------------------*/
namespace nsBattle {
	enum class eCommandState {
		Select,
		Move,
		Skill,
		Back,
	};

	struct stCommand {
		noDel_ptr<GameObject> pBackGorund;
		noDel_ptr<GameObject> pText;
		eCommandState type;
	};

	class CommandPanel : public Panel {
	private:
		//�I�����ꂽ�R�}���h
		eCommandState commandState = eCommandState::Select;

		//�J�[�\��
		noDel_ptr<Cursor> pCursor;

		//�X�L���I�����
		noDel_ptr<GameObject> pSkillSelectPanel;

	private:
		//�I������Ă���L����
		noDel_ptr<PlayerChara> pSelectChara;

		//�R�}���h�ꗗ
		std::vector<std::shared_ptr<stCommand>> vCommand;

		//�I���J�[�\��
		noDel_ptr<GameObject> pSelecter;
		int selectNum = 0;

	private:
		void Awake() override;

		void Update() override;

		//�R�}���h�쐬
		void CreateCommand(const WCHAR* text, noDel_ptr<Sprite> bgImage, eCommandState type);

		//�R�}���h�I��
		void MoveSelecter();
		void SelectCommand();

	public:
		void Open(noDel_ptr<PlayerChara> chara);
	};
}
