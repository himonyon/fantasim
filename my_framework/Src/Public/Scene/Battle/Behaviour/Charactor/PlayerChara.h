#pragma once
/*-----------------------------------------------------------

	PlyaerCharaクラス(BattleCharaの派生
	バトルで使用するプレイヤーキャラの挙動

-------------------------------------------------------------*/
namespace nsBattle {
	class PlayerChara : public BattleChara {
	private:
		//移動処理の関数ポインタ
		std::function<bool(PlayerChara&)> fMoveFunc;
		//攻撃処理の関数ポインタ
		std::function<bool(PlayerChara&)> fSkillFunc;

		//移動前の位置
		noDel_ptr<Square> pBeforeSquare = {};

	private:
		//移動処理------------------------------------------------
		bool ShowMoveArea();
		bool SelectMoveSquare();
		bool MoveToTarget();

		//スキル処理--------------------------------------------
		bool ShowSkillRange();
		//攻撃時対象の選択
		bool SelectEnemy();
		//攻撃以外対象の選択
		bool SelectFriend();

	public:
		void Start() override;

		//移動系処理
		bool Move();
		
		//スキル系処理
		bool SkillFunc();

		//以前の位置に戻る
		void BackBeforePos();

		//移動前のマスを登録
		void SetBeforeSquare() { pBeforeSquare = pCurSquare; }

	};
}
