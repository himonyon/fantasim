/*-----------------------------------------------------------

	InfoPanel�N���X
		����\������p�l��

-------------------------------------------------------------*/
#pragma once

namespace nsStrategy {
	class InfoPanel : public Panel {
	private:
		//Findobject
		noDel_ptr<SoundManager> pSoundManager;

		//�e�L�X�g
		noDel_ptr<Font> pInfoText;

	private:
		void Awake() override;
		void Update() override;

	public:
		void Open(std::wstring text);
	};
}
