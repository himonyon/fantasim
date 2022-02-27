/*-----------------------------------------------------------

	InfoPanelクラス
		情報を表示するパネル

-------------------------------------------------------------*/
#pragma once

namespace nsStrategy {
	class InfoPanel : public Panel {
	private:
		//Findobject
		noDel_ptr<SoundManager> pSoundManager;

		//テキスト
		noDel_ptr<Font> pInfoText;

	private:
		void Awake() override;
		void Update() override;

	public:
		void Open(std::wstring text);
	};
}
