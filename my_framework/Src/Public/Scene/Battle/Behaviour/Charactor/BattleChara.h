#pragma once
/*-----------------------------------------------------------

	BattleCharaクラス(Behaviourの派生)：プレイヤー、敵キャラクラスの基底クラス
	バトルで使用するキャラの挙動

-------------------------------------------------------------*/
namespace nsBattle {
	class BattleChara : public Behaviour {
	private:
		static bool OnHitFunc;

	protected:
		//回復・バフアニメーション
		noDel_ptr<Animator> pHealAnimator;

		//経路探索インスタンス
		static Astar pSearcher;

		//踏んでいるマス
		noDel_ptr<Square> pCurSquare;

		//移動時のカウント
		int moveCount = 0;

		//行動終了したか
		bool actionEnable = true;

	public:
		//当たり判定::キャラが選択されたか
		static noDel_ptr<BattleChara> pCollisionChara;

		//キャラのステータス
		noDel_ptr<Charactor> pCharaInfo;

		//ターゲットにするキャラクタ
		noDel_ptr<BattleChara> pTargetChara;
		//標的までの距離
		int rangeToTarget = 0;

		//カーソルが当たっているか
		bool isCursorHovered = false;

		//選択したスキル
		noDel_ptr<Skill> pSelectSkill;
	protected:
		//当たり判定
		void OnTriggerEnter2D(noDel_ptr<Collider2D> hitCollider) override;
		void OnTrigger2D(noDel_ptr<Collider2D> hitCollider) override;
		void OnTriggerExit2D(noDel_ptr<Collider2D> hitCollider) override;

	protected:
		void Awake() override;

		//キャラの移動
		bool Move(noDel_ptr<Square> start);

		//回復
		void Heal();

		//バフ付与
		void Buff();

	public:
		//死亡処理
		void Death();
		bool IsDeath() { return !gameObject->IsObjEnable(); }

		//バフのカウントを減らす
		void DecreaseBuffCount();

		//Setter,Getter
		void SetCurrentSquare(noDel_ptr<Square> square);
		noDel_ptr<Square> GetCurrentSquare() { return pCurSquare; }

		void SetActionEnable(bool flag);
		bool IsActionEnable() { return actionEnable; }

		//当たり判定処理を開始、中止する
		static void SetOnHitFunc(bool flag) { OnHitFunc = flag; if (!flag) pCollisionChara == NULL; }
	};
}
