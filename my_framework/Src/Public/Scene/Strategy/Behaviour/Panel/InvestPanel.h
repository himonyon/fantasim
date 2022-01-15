/*-----------------------------------------------------------

	InvestPanelクラス
		投資で各ステータスを上昇させるパネル

-------------------------------------------------------------*/
#pragma once

namespace nsStrategy {
	class InvestPanel : public Panel {
	private:
		//Findobject
		noDel_ptr<GameObject> pCityPanel;

	private:
		//対象の国、街
		noDel_ptr<City> pCity;
		noDel_ptr<City> pCity2;

		//投資後かどうか
		bool isResult = false;

		//長押し判定
		int pressCount = 0;

		//実行体
		std::function<void(InvestPanel&)> InvestFunc;

		//投資金額
		int investMoney = 0;
		const int minInvestMoney = 100;
		int maxInvestMoney = 0;

		//投資前の資金と各ステータス値
		int beforeMoney = 0;
		int beforeMoney2 = 0;
		int beforeValue = 0;

		//テキスト
		noDel_ptr<GameObject> pDescText;
		noDel_ptr<GameObject> pPriceText;
		noDel_ptr<GameObject> pResultText;

	private:
		void Awake() override;
		void Update() override;

		void SetInvestEconomic();
		void SetInvestPolitical();
		void SetInvestDisaster();
		void SetInvestSupport();
		void SetMoveMoney();
		void InvestEconomic();
		void InvestPolitical();
		void InvestDisaster();
		void InvestSupport();
		void MoveMoney();

	public:
		void Open(noDel_ptr<City> city, int cmd);
		void Open(noDel_ptr<City> city, noDel_ptr<City> city2);
	};
}