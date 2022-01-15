#pragma once
/*-----------------------------------------------------------

	BattleCharaクラス(BattleCharaの派生
	バトルで使用するプレイヤーキャラの挙動

-------------------------------------------------------------*/
namespace nsBattle {
	class PlayerChara : public BattleChara {
	private:
		std::function<bool(PlayerChara&)> fCharaFunc;

	public:
		void Update() override;

		//移動系処理
		bool SelectMoveSquare();
		bool Move();

		//移動常態に遷移
		void StartMoveFunc();

		//攻撃処理遷移
		void StartAttackFunc(noDel_ptr<AttackSkill> skill);
		bool SelectEnemy();

		//回復スキル処理遷移
		void StartHealFunc(noDel_ptr<AttackSkill> skill);
		bool SelectFriend();

		//バフスキル処理遷移
		void StartBuffFunc(noDel_ptr<AttackSkill> skill);
	};
}
