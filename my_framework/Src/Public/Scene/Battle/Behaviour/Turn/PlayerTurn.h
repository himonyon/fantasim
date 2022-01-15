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

		//検索オブジェクト----------------------
		noDel_ptr<GameObject> pStatusPanel;
		//カーソル
		noDel_ptr<Cursor> pCursor;

	private:
		void Start() override;

		void Update() override;

		void SelectChara();

		//通常行動
		void NormalFunc();

		//キャラのカーソルが乗って時の挙動
		void ShowCharaInfo();

	};
}
