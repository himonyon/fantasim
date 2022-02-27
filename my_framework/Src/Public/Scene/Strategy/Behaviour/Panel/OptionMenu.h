/*-----------------------------------------------------------

	OptionMenu�N���X
		�I�v�V�������j���[

-------------------------------------------------------------*/
#pragma once

namespace nsStrategy {
	enum class eOptionMenu {
		TurnEnd,
		Option,
		Save,
	};

	class OptionMenu : public Panel {
	private:
		//Findobject
		noDel_ptr<SoundManager> pSoundManager;

		struct stOptionMenu {
			noDel_ptr<GameObject> pBackGorund;
			noDel_ptr<GameObject> pText;
			eOptionMenu type = eOptionMenu::TurnEnd;
		};

		//���j���[
		std::vector<std::shared_ptr<stOptionMenu>> vMenu;

		//�I���J�[�\��
		noDel_ptr<GameObject> pSelecter;
		int selectNum = 0;

	private:
		void Awake() override;
		void Update() override;

		//�R�}���h�쐬
		void CreateCommand(const WCHAR* text, noDel_ptr<Sprite> bgImage, eOptionMenu type);

		//�R�}���h�I��
		void MoveSelecter();
		void SelectMenu();

	public:
		void Open() override;
	};
}
