/*-----------------------------------------------------------

	CursorƒNƒ‰ƒX(Behaviour‚Ì”h¶)
		‘‚Ìî•ñ‚ğŠÇ—

-------------------------------------------------------------*/
#pragma once

namespace nsStrategy {
	class Country : public City {
	private:
		noDel_ptr<GameObject> pPlayerText = 0;
		noDel_ptr<Sprite> pCountry_sp = 0;
		bool isPlayer = false;

	public:
		void Start() override;
		void Update() override;

		void SetSprite(noDel_ptr<Sprite> country, noDel_ptr<Sprite> city, noDel_ptr<Sprite> frame);

		void SetIsPlayer();
		bool IsPlayer() { return isPlayer; }
	};
}
