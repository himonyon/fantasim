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
		//操作説明
		noDel_ptr<Operation> pOperation;

		//遷移先のパネル
		noDel_ptr<CityPanel> pCityPanel;
		noDel_ptr<InvestPanel> pInvestPanel;
		noDel_ptr<SelectNeighborPanel> pSelectNeighborPanel;
		noDel_ptr<BattlePanel> pBattlePanel;
		noDel_ptr<CharaSelectPanel> pCharaSelectPanel;
		noDel_ptr<CharaEnhancePanel> pCharaEnhancePanel;
		noDel_ptr<OptionMenu> pOptionMenu;
		noDel_ptr<OptionPanel> pOptionPanel;
		noDel_ptr<EventPanel> pEventPanel;

		//選択された街
		noDel_ptr<City> pSelectCity;

		//変更されたか確認用
		eTurnState checkState = eTurnState::None;

	public:
		void TurnInit();

	private:
		void Awake() override;

		void Start() override;

		void Update() override;

		//必要なパネルの作成
		void CreatePanels();

		//各処理-----------------------------
		//フィールド画面
		void FieldFunc();
		//ターン終了
		void TurnEndFunc();
		//セーブ
		void SaveFunc();
		//オプションメニュー
		void OptionMenuFunc();
		//オプション
		void OptionFunc();
		//街のコマンド処理
		void CommandFunc();
		//投資コマンド系
		void InvEconomicFunc();
		void InvPoliticalFunc();
		void InvDisasterFunc();
		void InvSupportFunc();
		//資金移動コマンド
		void SelMoneyMoveToCityFunc();
		void MoveMoneyFunc();
		//キャラ移動コマンド
		void SelCharaMoveToCityFunc();
		void MoveCharaFunc();
		//戦闘コマンド
		void SelBattleCityFunc();
		void BattleFunc();
		//キャラ強化
		void CharaEnhance();
	};
}