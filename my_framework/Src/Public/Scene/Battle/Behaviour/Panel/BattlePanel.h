#pragma once
/*-----------------------------------------------------------

	SelectSkillPanelクラス(Panelの派生)
	スキル選択画面の管理

-------------------------------------------------------------*/
namespace nsBattle {
	class BattlePanel : public Panel {
	private:
		std::function<bool(BattlePanel&)> fBattleFunc;

		//配列識別番号
		const int playerNum = 0;
		const int enemyNum = 1;

		//反撃しているか
		bool counterTurn = false;

		//与えるダメージ
		int damage = 0;
		//属性倍率テキスト
		std::wstring attrText;
		//回避トリガー
		bool evadeTrg = false;

		//処理するキャラクター
		noDel_ptr<BattleChara> pAttacker;
		noDel_ptr<BattleChara> pDefender;

		//画面上に映し出すキャラクター
		noDel_ptr<GameObject> pPlayerView;
		noDel_ptr<GameObject> pEnemyView;

		//各情報表示用のパネル
		noDel_ptr<GameObject> pCharaInfoPanel[2];
		noDel_ptr<GameObject> pBattleTextPanel;

		//属性と名前
		noDel_ptr<GameObject> pAttrObj[2];
		noDel_ptr<Font> pNameText[2];

		//HP,MPバーとテキスト
		noDel_ptr<GameObject> pHPBar[2];
		noDel_ptr<GameObject> pMPBar[2];
		noDel_ptr<Font> pHPText[2];
		noDel_ptr<Font> pMPText[2];
		noDel_ptr<GameObject> pHPFrame[2];
		noDel_ptr<GameObject> pMPFrame[2];

		noDel_ptr<Font> pBattleText;

		//アニメーション
		noDel_ptr<Animator> pAttack_anim;

		//バトル画面のタイマー管理
		Timer timer;

		//一度だけ実行したい処理のためのトリガー
		bool onceTrg = false;

	private:
		void Awake() override;

		void Update() override;

		void SetInfoPanel(int index);

		//HP,MPのバーの管理
		void SetBarPosition(noDel_ptr<ImageRenderer> pTarget, noDel_ptr<ImageRenderer> pFrame, int curVal, int maxVal);

		//開始処理
		bool InitFunc();

		//戦闘の処理
		bool Attack();
		bool Damage();

		//結果処理
		bool ResultFunc();

		//バトル演算
		void Battle(noDel_ptr<BattleChara> attacker, noDel_ptr<BattleChara> defender);

		//HP,MPバーの設定
		void SetStatusBar();

		//反撃時のアタッカー変更
		void ChangeAttacker();

	public:
		void Open(noDel_ptr<PlayerChara> player, noDel_ptr<EnemyChara> enemy, bool isPlayerAttack);
	};
}
