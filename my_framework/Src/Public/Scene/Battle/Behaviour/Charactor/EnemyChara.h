#pragma once
/*-----------------------------------------------------------

	BattleCharaクラス(BattleCharaの派生
	バトルで使用する敵キャラの挙動

-------------------------------------------------------------*/
namespace nsBattle {
	class EnemyChara : public BattleChara {
	private:
		std::function<bool(EnemyChara&)> fMoveFunc;

	private:
		//移動先の設定
		bool StartMove();
		//目標地点へ移動
		bool MoveToTarget();

	public:
		void Start() override;

		//範囲内で仕様できるスキルを返す
		void ChooseSkill(eSkillType);

		//設定されたスキルの処理
		bool Skill();

		//移動処理
		bool Move();
		
	};
}
