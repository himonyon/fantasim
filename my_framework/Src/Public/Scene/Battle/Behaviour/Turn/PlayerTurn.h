#pragma once
/*-----------------------------------------------------------

	BattleCharaクラス(Behaviourの派生)
	バトルで使用するキャラの挙動

-------------------------------------------------------------*/
namespace nsBattle {
	class PlayerTurn : public Turn {
	private:
		//コマンドパネル
		noDel_ptr<CommandPanel> pCommandPanel;
		//スキルセレクトパネル
		noDel_ptr<SelectSkillPanel> pSkillSelectPanel;
		//ターン開始時に表示するオブジェクト
		noDel_ptr<Animator> pTurnStartAnim;

		//検索オブジェクト----------------------
		noDel_ptr<GameObject> pStatusPanel;
		//カーソル
		noDel_ptr<Cursor> pCursor;

	private:
		void Awake() override;

		void Start() override;

		void Update() override;

		//ターン開始時処理
		void TurnInit() override;

		//キャラ選択
		void SelectChara();

		//キャラのカーソルが乗って時の挙動
		void ShowCharaInfo();

		//各状態の処理
		void FieldFunc();
		void BeforeCommandFunc();
		void AfterCommandFunc();
		void MoveFunc();
		void SkillFunc();
		void SelectSkillFunc();
		void BattleFunc();
	};
}
