#pragma once
/*-----------------------------------------------------------

	Commandクラス(Behaviourの派生)
	コマンド一覧の表示と実行

-------------------------------------------------------------*/
namespace nsBattle {
	enum class eCommand {
		Select,
		Move,
		Skill,
		Wait,
		Back,
	};

	enum class eCommandPanelType {
		MoveBefore,
		MoveAfter,
	};

	struct stCommand {
		noDel_ptr<GameObject> pBackGorund;
		noDel_ptr<GameObject> pText;
		eCommand type;

		void SetComEnable(bool flag) {
			pBackGorund->SetObjEnable(flag);
			pText->SetObjEnable(flag);
		}
	};

	class CommandPanel : public Panel {
	private:
		//選択されたコマンド
		eCommand commandState = eCommand::Select;

		//パネルのタイプ
		eCommandPanelType panelType;

	private:
		//選択されているキャラ
		noDel_ptr<PlayerChara> pSelectChara;

		//コマンド一覧
		std::unordered_map<eCommand,std::shared_ptr<stCommand>> umCommand;
		//利用するコマンド
		std::vector<std::shared_ptr<stCommand>> vCommand;

		//選択カーソル
		noDel_ptr<GameObject> pSelecter;
		int selectNum = 0;

	private:
		void Awake() override;

		void Update() override;

		//コマンド作成
		void CreateCommand(const WCHAR* text, noDel_ptr<Sprite> bgImage, eCommand type);

		//コマンド選択
		void MoveSelecter();
		void SelectCommand();

		//コマンド位置設定
		void SetCommandPos();

		//戻る処理
		void BackFunc();

	public:
		void Open(noDel_ptr<PlayerChara> chara, eCommandPanelType type);
	};
}
