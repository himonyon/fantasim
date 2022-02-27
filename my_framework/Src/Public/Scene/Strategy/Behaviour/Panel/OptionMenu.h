/*-----------------------------------------------------------

	OptionMenuクラス
		オプションメニュー

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

		//メニュー
		std::vector<std::shared_ptr<stOptionMenu>> vMenu;

		//選択カーソル
		noDel_ptr<GameObject> pSelecter;
		int selectNum = 0;

	private:
		void Awake() override;
		void Update() override;

		//コマンド作成
		void CreateCommand(const WCHAR* text, noDel_ptr<Sprite> bgImage, eOptionMenu type);

		//コマンド選択
		void MoveSelecter();
		void SelectMenu();

	public:
		void Open() override;
	};
}
