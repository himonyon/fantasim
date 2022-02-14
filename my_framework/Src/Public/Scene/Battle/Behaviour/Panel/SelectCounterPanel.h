#pragma once
/*-----------------------------------------------------------

	SelectSkillPanelクラス(Panelの派生)
	スキル選択画面の管理

-------------------------------------------------------------*/
namespace nsBattle {
	class SelectCounterPanel : public Panel {
	private:
		//選択されたキャラクタ
		noDel_ptr<PlayerChara> pSelectChara;

		//反撃できる距離
		int range;

		//選択カーソル
		noDel_ptr<GameObject> pSelecter;
		int selectNum = 0;

		//攻撃スキル一覧
		std::vector <noDel_ptr<Skill>> vAtSkills;

		//スキル名テキスト
		noDel_ptr<Font> pSkillTexts[MAX_OWN_SKILL + 1];

		//説明用テキスト
		noDel_ptr<GameObject> pTitleText;
		noDel_ptr<GameObject> pBorder;
		static const int DescNum = 5;
		noDel_ptr<Font> pDescText[DescNum];

	private:
		void Awake() override;

		void Update() override;

		//カーソル移動
		void MoveCursor();

		//説明文をセットする
		void SetDescText(noDel_ptr<Skill> skill);
		void SetAttackDescText(noDel_ptr<Skill> skill);

		//スキル選択
		bool DecideSkill();

	public:
		void Open(noDel_ptr<PlayerChara> chara, int range);
	};
}
