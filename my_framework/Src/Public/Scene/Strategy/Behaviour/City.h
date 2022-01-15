/*-----------------------------------------------------------

	Cityクラス(Behaviourの派生)
		街の情報を管理

-------------------------------------------------------------*/
#pragma once

namespace nsStrategy {

	class Country;

	class City : public Behaviour {
	protected:
		//Findするオブジェクト
		noDel_ptr<GameObject> pCityPanel;

	protected:
		//街画像の枠
		noDel_ptr<GameObject> pCityFrame;

		//スプライト
		noDel_ptr<Sprite> pCity_sp;
		noDel_ptr<Sprite> pCityFrame_sp;

		//ID
		int id = 0;
		//名前
		std::wstring name = L"";
		//隣り合う領地
		std::vector<noDel_ptr<City>> vNeighbor; 

	public:
		//ステータス
		noDel_ptr<Country> pCountry = 0;
		int money = 0;
		int economic = 0;
		int political = 0;
		int disaster = 0;
		int support = 0;
		float bussiness = 0;

		//所有キャラ
		std::vector<noDel_ptr<Charactor>> vOwnChara;

	public:
		virtual ~City() {};

		void Start() override;
		void Update() override;
		void OnTriggerEnter2D(noDel_ptr<Collider2D> hitCol) override;
		void OnTrigger2D(noDel_ptr<Collider2D> hitCol) override;
		void OnTriggerExit2D(noDel_ptr<Collider2D> hitCol) override;

		void ChangeBelongCountry(noDel_ptr<Country> country);

		//画像代入用
		void SetSprite(noDel_ptr<Sprite> city, noDel_ptr<Sprite> frame);

		//画像のフォーカス
		void FocusCity(bool trg);

		//Setter/getter
		void SetID(int id) { this->id = id; }
		int GetID() { return id; }

		void SetName(std::wstring name) { this->name = name; }
		std::wstring& GetName(){ return name; }

		void SetNeighbor(noDel_ptr<City> city) { vNeighbor.emplace_back(city); }
		std::vector<noDel_ptr<City>> GetNeighbor() { return vNeighbor; }

		std::wstring GetBussinessText();
	};
}
