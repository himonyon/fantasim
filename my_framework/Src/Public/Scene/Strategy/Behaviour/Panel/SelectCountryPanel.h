/*-----------------------------------------------------------

	CityƒNƒ‰ƒX(Behaviour‚Ì”h¶)
		ŠX‚Ìî•ñ‚ğŠÇ—

-------------------------------------------------------------*/
#pragma once

namespace nsStrategy {
	class SelectCountryPanel : public Panel {
	private:
		//Findobject
		noDel_ptr<SoundManager> pSoundManager;

		//Find•Ï”
		noDel_ptr<GameObject> pCursor = 0;
		noDel_ptr<Country> countries[COUNTRY_NUM] = { 0 };

	private:
		noDel_ptr<GameObject> pText[4] = { 0 };
		noDel_ptr<Font> pText_font[4] = { 0 };

		int cursor = 0;
		stColor4 textCol = { 0 };

	public:
		void Awake() override;
		void Update() override;

		void SetTextState(noDel_ptr<Country>);
	};
}
