/*-----------------------------------------------------------

	PlayerTurnクラス(Turn派生)
		プレイヤーターン管理

-------------------------------------------------------------*/
#pragma once

namespace nsStrategy {
	class PlayerTurn : public Turn {
	private:
		//サウンドマネージャー
		noDel_ptr<SoundManager> pSoundManager;

		//遷移先のパネル
		noDel_ptr<CityPanel> pCityPanel;
		noDel_ptr<InvestPanel> pInvestPanel;
		noDel_ptr<SelectNeighborPanel> pSelectNeighborPanel;
		noDel_ptr<BattlePanel> pBattlePanel;
		noDel_ptr<CharaSelectPanel> pCharaSelectPanel;
		noDel_ptr<CharaEnhancePanel> pCharaEnhancePanel;

		//選択された街
		noDel_ptr<City> pSelectCity;

	private:
		void Awake() override;

		void Start() override;

		void Update() override;

		//必要なパネルの作成
		void CreatePanels();

		//各処理
		void FieldFunc();
		void CommandFunc();
		void InvEconomicFunc();
		void InvPoliticalFunc();
		void InvDisasterFunc();
		void InvSupportFunc();
		void SelMoneyMoveToCityFunc();
		void MoveMoneyFunc();
		void SelCharaMoveToCityFunc();
		void MoveCharaFunc();
		void SelBattleCityFunc();
		void BattleFunc();
		void CharaEnhance();
	};
}