#pragma once
/*-----------------------------------------------------------

	Command�N���X(Behaviour�̔h��)
	�R�}���h�ꗗ�̕\���Ǝ��s

-------------------------------------------------------------*/
namespace nsBattle {
	enum class eCommand {
		Select,
		Move,
		Skill,
		Wait,
		Back,
	};

	enum class eCommandPanelType {
		MoveBefore,
		MoveAfter,
	};

	struct stCommand {
		noDel_ptr<GameObject> pBackGorund;
		noDel_ptr<GameObject> pText;
		eCommand type;

		void SetComEnable(bool flag) {
			pBackGorund->SetObjEnable(flag);
			pText->SetObjEnable(flag);
		}
	};

	class CommandPanel : public Panel {
	private:
		//�I�����ꂽ�R�}���h
		eCommand commandState = eCommand::Select;

		//�p�l���̃^�C�v
		eCommandPanelType panelType;

	private:
		//�I������Ă���L����
		noDel_ptr<PlayerChara> pSelectChara;

		//�R�}���h�ꗗ
		std::unordered_map<eCommand,std::shared_ptr<stCommand>> umCommand;
		//���p����R�}���h
		std::vector<std::shared_ptr<stCommand>> vCommand;

		//�I���J�[�\��
		noDel_ptr<GameObject> pSelecter;
		int selectNum = 0;

	private:
		void Awake() override;

		void Update() override;

		//�R�}���h�쐬
		void CreateCommand(const WCHAR* text, noDel_ptr<Sprite> bgImage, eCommand type);

		//�R�}���h�I��
		void MoveSelecter();
		void SelectCommand();

		//�R�}���h�ʒu�ݒ�
		void SetCommandPos();

		//�߂鏈��
		void BackFunc();

	public:
		void Open(noDel_ptr<PlayerChara> chara, eCommandPanelType type);
	};
}
