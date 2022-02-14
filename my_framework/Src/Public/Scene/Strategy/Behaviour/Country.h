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
		//‘®‚µ‚Ä‚¢‚éŠX
		bool isPlayer = false;
		//Š×—‚µ‚½‚©
		bool isFalled = false;

	public:
		std::vector<noDel_ptr<City>> vOwnCities;

	public:
		void Start() override;
		void Update() override;

		void SetSprite(noDel_ptr<Sprite> country, noDel_ptr<Sprite> city, noDel_ptr<Sprite> frame);

		//Š‘®•ÏX
		void ChangeBelongCountry(noDel_ptr<Country> country) override;

		void SetIsPlayer();
		bool IsPlayer() { return isPlayer; }

		//ŠX‚Ì”z—ñ‚©‚çŠX‚ğ”²‚­
		void PullOutCity(noDel_ptr<City> city);
		//ŠX‚ğ”z—ñ‚É‰Á‚¦‚é
		void SetOwnCity(noDel_ptr<City> city);

		//Š×—‚µ‚Ä‚¢‚é‚©
		bool IsFallen() { return isFalled; }
		void SetFallen() { isFalled = true; }
	};
}
