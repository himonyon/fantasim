#pragma once

namespace nsTitle {
	class TitleManager : public Behaviour {
	private:
		int stopFunc = false;

		int cursor = 0;
		std::unique_ptr<Sound> pOpening_sd;

		noDel_ptr<GameObject> pNewText;
		noDel_ptr<Font> pNewText_font;
		noDel_ptr<GameObject> pContinueText;
		noDel_ptr<Font> pContinueText_font;

	private:
		void Awake() override;

		void Update() override;

		//カーソル移動
		void MoveCursor();

		//遷移
		void TransScene();

		void LoadSetting();
	};
}

