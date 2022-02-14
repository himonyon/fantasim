/*-----------------------------------------------------------

	EnemyTurnクラス(Turn派生)
		敵のターン管理

-------------------------------------------------------------*/
#pragma once


namespace nsStrategy {

	class GameManager;

	class EnemyTurn : public Turn {
	private:
		//ゲームマネージャー
		noDel_ptr<GameManager> pGM;

		//バトルパネル
		noDel_ptr<BattlePanel> pBtPanel;

		//プレイヤーの国
		noDel_ptr<Country> pPlayerCountry;

		//現在処理している街
		int curCityID = 0;
		noDel_ptr<City> pTargetCity = NULL;

		//街単位の処理にかける時間
		Timer timer;

		//街の処理が終わったか確認するトリガー
		bool isSimulated = false;

		//処理をいったん止める
		bool stopSimulate = false;

	private:
		void Start() override;
		void Update() override;

		//シミュレート
		void Simulate();

		//各シミュレート処理
		bool Battle();
		bool CharaEnhance();
		bool CharaMove();
		bool MoneyMove();
		bool EconomicUp();
		bool PoliticalUp();
		bool DisasterUp();
		bool SupportUp();

	public:
		//ターン開始時の初期化
		void TurnInit();
		//途中からシミュレート再開
		void TurnInit(int id);
	};
}