/*-----------------------------------------------------------

	SelectNeighborPanelクラス
		近接している国または街を選択するパネルクラス

-------------------------------------------------------------*/
#pragma once

namespace nsStrategy {
	class SelectNeighborPanel : public Panel {
	private:
		//Findobject
		noDel_ptr<GameObject> pCityPanel;
		noDel_ptr<GameObject> pInvestPanel;
		noDel_ptr<GameObject> pBattlePanel;
		noDel_ptr<GameObject> pCharaSelectPanel;

	private:
		//対象の国、街
		noDel_ptr<City> pCity;

		int commandType = 0;

		//隣接していて対象となった国・街
		std::vector<noDel_ptr<City>> vSelectableCities;

		//選択している近隣領地の数字
		int selectNum = 0;

		//テキスト
		noDel_ptr<GameObject> pDescText;
		noDel_ptr<GameObject> pNameText;
		noDel_ptr<GameObject> pArrowText;

	private:
		void Awake() override;
		void Update() override;

	public:
		void Open(noDel_ptr<City> city, int cmd);
		void Close() override;
	};
}
