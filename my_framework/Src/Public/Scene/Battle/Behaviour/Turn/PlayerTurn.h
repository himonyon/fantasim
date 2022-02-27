#pragma once
/*-----------------------------------------------------------

	PalyerTurnクラス(Turnの派生)
	プレイヤーターンの管理

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
		//サウンドマネージャー
		noDel_ptr<SoundManager> pSoundManager;
		//カーソル
		noDel_ptr<Cursor> pCursor;

		//状態変更したか確認用
		eTurnState checkState = eTurnState::None;

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
