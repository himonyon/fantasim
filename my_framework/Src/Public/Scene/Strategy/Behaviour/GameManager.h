/*-----------------------------------------------------------

	GameManagerクラス(Behaviourの派生)
	ストラテジーシーン全体の管理

-------------------------------------------------------------*/
#pragma once
namespace nsStrategy {
	struct stBattleData {
		bool data = false;
		bool isPlayerTurn = true;
		bool isWin = true;
		noDel_ptr<City> pP_City;;
		noDel_ptr<City> pE_City;;
	};

	class GameManager : public Behaviour {
	private:
		//セーブ管理
		std::unique_ptr<Save> saveManager;

		//バトル後のデータ
		stBattleData battleData;

		//ステージ
		noDel_ptr<GameObject> pBg;
		noDel_ptr<City> pCities[TERRITORY_NUM];
		std::vector<noDel_ptr<GameObject>> vpRoad;

		//カーソル
		noDel_ptr<GameObject> pCursor;

		//パネル
		noDel_ptr<SelectCountryPanel> pSelectCountryPanel;

		//キャラクター
		std::unordered_map<int, Charactor*> umCharactors;

		//ターン
		noDel_ptr<PlayerTurn> pPlayerTurn;
		noDel_ptr<EnemyTurn> pEnemyTurn;

		//サウンド
		noDel_ptr<SoundManager> pSoundManager;

		//操作説明
		noDel_ptr<Operation> pOperation;

		//ゲーム終了フラグ
		bool isGameEnd = false;

		//ゲーム内の年月
		noDel_ptr<Font> pDateText;
		int year = 1000;
		int month = 1;

	public:
		//デストラクタ
		~GameManager();

		//初期化
		void Awake(void) override;

		//ターン切り替え
		void ChangeTurn();

		//選択せれた街をターンに渡す
		void SetFocusCity(noDel_ptr<City> city);

		//ターゲットにする街のセット
		void SetTargetCity(noDel_ptr<City> city);

		//ゲーム開始フラグ
		void GameStart();

		//ゲーム終了（クリア・ゲームオーバー）
		void EndGame(bool isClear); //終了フラグを立てる
		bool IsEndGame();//終了確認
		void CheckClear(); //クリア確認

		//国の取得
		noDel_ptr<City> GetCity(int id);

		//ターン状態セット
		void SetTurnState(eTurnState state, int backNum = 1);

		//セーブ
		void SaveGame();
		//戦闘する街の保存(p_id = プレイヤーの街ID e_id = 敵の街ID)
		void SaveBattleInfo(int p_id, int e_id);

		//年月更新
		void UpdateMonth();
		//年月取得
		int GetYear() { return year; }
		int GetMonth() { return month; }

	private:
		//領地の初期化
		void CreateCharactor(bool saveData);
		void CreateTerritory();

		//サウンド作成
		void CreateSound();

		//ロード(基本情報)
		bool LoadBaceDate();

		//ロード(バトル情報)
		bool LoadBattleInfo();
	};
}