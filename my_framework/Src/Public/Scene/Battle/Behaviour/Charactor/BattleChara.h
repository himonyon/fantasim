#pragma once
/*-----------------------------------------------------------

	BattleCharaクラス(Behaviourの派生)
	バトルで使用するキャラの挙動

-------------------------------------------------------------*/
namespace nsBattle {
	enum class eCharaState {
		None,
		Move,
		Attack,
		Heal,
		Buff,
	};

	class BattleChara : public Behaviour {
	protected:
		//経路探索インスタンス
		static Astar pSearcher;
	
		//キャラの状態
		eCharaState charaState = eCharaState::None;

		//踏んでいるマス
		noDel_ptr<Square> pCurSquare;

		//移動時のカウント
		int moveCount = 0;

	public:
		//キャラが選択されたか
		static noDel_ptr<BattleChara> pSelectChara;

		noDel_ptr<Charactor> pCharaInfo;

		//カーソルが当たっているか
		bool isCursorHovered = false;

	protected:
		//当たり判定
		void OnTriggerEnter2D(noDel_ptr<Collider2D> hitCollider) override;
		void OnTrigger2D(noDel_ptr<Collider2D> hitCollider) override;
		void OnTriggerExit2D(noDel_ptr<Collider2D> hitCollider) override;

	protected:
		//キャラの移動
		bool Move(noDel_ptr<Square> start);

	public:
		//Setter,Getter
		void SetCurrentSquare(noDel_ptr<Square> square) {
			if (pCurSquare != NULL) pCurSquare->SetUnMove(false);
			pCurSquare = square;
			pCurSquare->SetUnMove(true);
		}
		noDel_ptr<Square> GetCurrentSquare() { return pCurSquare; }

	};
}
