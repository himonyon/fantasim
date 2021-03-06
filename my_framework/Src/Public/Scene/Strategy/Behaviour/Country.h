/*-----------------------------------------------------------

	Countryクラス(Cityの派生)
		国の情報を管理

-------------------------------------------------------------*/
#pragma once

namespace nsStrategy {
	class Country : public City {
	private:
		noDel_ptr<GameObject> pPlayerText = 0;
		noDel_ptr<Sprite> pCountry_sp = 0;
		//属している街
		bool isPlayer = false;
		//陥落したか
		bool isFalled = false;

	public:
		std::vector<noDel_ptr<City>> vOwnCities;

	public:
		void Start() override;
		void Update() override;

		void SetSprite(noDel_ptr<Sprite> country, noDel_ptr<Sprite> city, noDel_ptr<Sprite> frame);

		//所属変更
		void ChangeBelongCountry(noDel_ptr<Country> country) override;

		void SetIsPlayer();
		bool IsPlayer() { return isPlayer; }

		//街の配列から街を抜く
		void PullOutCity(noDel_ptr<City> city);
		//街を配列に加える
		void SetOwnCity(noDel_ptr<City> city);

		//陥落しているか
		bool IsFallen() { return isFalled; }
		void SetFallen() { isFalled = true; }
	};
}
