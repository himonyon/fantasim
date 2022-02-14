#pragma once
/*-----------------------------------------------------------

	BattleCharaクラス(Behaviourの派生)
	バトルで使用するキャラの挙動

-------------------------------------------------------------*/
namespace nsBattle {

	class EnemyTurn : public Turn {
	private:
		//一度だけ処理するトリガー
		bool onceTrg = false;

		//ターン開始時に表示するオブジェクト
		noDel_ptr<Animator> pTurnStartAnim;
		//スキルセレクトパネル
		noDel_ptr<SelectCounterPanel> pSelectCounterPanel;

		//敵の配列の何番目の操作をするか
		int enemyNum = 0;

	public:
		void Awake() override;

		void Update() override;

		void TurnInit() override;

		//ターゲットキャラの設定
		void SetTargetEnemy();
		void SetTargetFriend();

	private:
		//各状態の処理
		void SelectSkill();
		void BeforeSkillFunc();
		void AfterSkillFunc();
		void SelectCounterFunc();
		void MoveFunc();
		void BattleFunc();
	};
}
