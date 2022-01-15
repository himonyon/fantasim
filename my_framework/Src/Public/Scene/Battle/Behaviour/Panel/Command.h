#pragma once
/*-----------------------------------------------------------

	Commandクラス(Behaviourの派生)
	コマンド一覧の表示と実行

-------------------------------------------------------------*/
namespace nsBattle {
	enum class eCommandState {
		Select,
		Move,
		Skill,
		Back,
	};

	struct stCommand {
		noDel_ptr<GameObject> pBackGorund;
		noDel_ptr<GameObject> pText;
		eCommandState type;
	};

	class CommandPanel : public Panel {
	private:
		//選択されたコマンド
		eCommandState commandState = eCommandState::Select;

		//カーソル
		noDel_ptr<Cursor> pCursor;

		//スキル選択画面
		noDel_ptr<GameObject> pSkillSelectPanel;

	private:
		//選択されているキャラ
		noDel_ptr<PlayerChara> pSelectChara;

		//コマンド一覧
		std::vector<std::shared_ptr<stCommand>> vCommand;

		//選択カーソル
		noDel_ptr<GameObject> pSelecter;
		int selectNum = 0;

	private:
		void Awake() override;

		void Update() override;

		//コマンド作成
		void CreateCommand(const WCHAR* text, noDel_ptr<Sprite> bgImage, eCommandState type);

		//コマンド選択
		void MoveSelecter();
		void SelectCommand();

	public:
		void Open(noDel_ptr<PlayerChara> chara);
	};
}
