#pragma once
/*-----------------------------------------------------------

	Turnクラス(Behaviourの派生) : 各ターンの基底クラス
	ターンの管理

-------------------------------------------------------------*/
namespace nsBattle {
	//ターン状態
	enum class eTurnState {
		None,
		Back,
		Field,
		BeforeCommand,
		AfterCommand,
		Move,
		Skill,
		SelectSkill,
		Battle,
		EnemyAttack,
		EnemyAfterAttack,
		EnemyMove,
	};
	//現在の状態と一つ前の状態を持った構造体
	struct stTurnState {
		eTurnState state = eTurnState::None;
		std::weak_ptr<stTurnState> bef;

		stTurnState() {};
		stTurnState(eTurnState st) {
			state = st;
		}
	};

	class Turn : public Behaviour {
	protected:
		//現在の進行状態
		std::shared_ptr<stTurnState> curState;

		//ターン状態一覧
		std::unordered_map<eTurnState, std::shared_ptr<stTurnState>> umStates;


	public:
		//状態セット
		void SetTurnState(eTurnState state, int backNum = 1);


	protected:
		//選択中のキャラクタ
		noDel_ptr<BattleChara> pSelectChara;

		//敵一覧
		std::vector <noDel_ptr<PlayerChara>> vPlayer;
		//プレイヤー一覧
		std::vector <noDel_ptr<EnemyChara>> vEnemy;
		
		//ゲーム終了フラグ
		bool isGameEnd = false;

	public:
		void Start() override;

		//キャラクターのBattleCharaクラス取得
		std::vector<noDel_ptr<BattleChara>> vBattleChara;

	protected:
		//カメラ移動
		void MoveCamera();

		//全滅判定
		void CheckEndGame();

		//シーン遷移
		void TransScene();


	public:
		virtual void TurnInit();

		//ゲーム終了判定
		void CheckGameEnd();
	};
}
