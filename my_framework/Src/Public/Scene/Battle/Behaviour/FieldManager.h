#pragma once
/*-----------------------------------------------------------

	FieldManagerクラス(Behaviourの派生)
	フィールド全体の管理

-------------------------------------------------------------*/
namespace nsBattle {
	class FieldManager : public Behaviour {
	private:
		//ステージ
		std::unordered_map<int, SquareRow*> umStageRows;

		//キャラクターのBattleCharaクラス取得
		std::vector<noDel_ptr<BattleChara>> vBattleChara;

		//キャラクター
		std::vector<noDel_ptr<GameObject>> vPlayerChara;
		std::vector<noDel_ptr<GameObject>> vEnemyChara;

		//ターンマネージャー
		noDel_ptr<PlayerTurn> pPlayerTurn;
		noDel_ptr<EnemyTurn> pEnemyTurn;

		//開始トリガー
		bool startTrg = false;
		

	public:
		static const float StageSize; //マスの大きさ
		static const float StageLeft; //左端のマスの座標
		static const float StageTop; //マスの一番上の座標
		static int SquareNum_X; //マスの数X
		static int SquareNum_Y; //マスの数Y

		//キャラクターステータス（前のシーンから引き継ぐ）
		std::vector<noDel_ptr<Charactor>> vPlayerCharaBace;
		std::vector<noDel_ptr<Charactor>> vEnemyCharaBace;

	public:
		~FieldManager();

		void ChangeTurn();
	private:
		//初期化
		void Start() override;

	private:
		//ステージとキャラクターの配置
		void InitStage();
		void InitCharactor();

	public:
		void SetTurnState(eTurnState, int backNum = 1);
	};
}
