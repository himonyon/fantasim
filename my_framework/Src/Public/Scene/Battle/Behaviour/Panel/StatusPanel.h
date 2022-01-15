#pragma once
/*-----------------------------------------------------------

	StatusPanelクラス(Panelの派生)
	ステータス表示パネルの管理

-------------------------------------------------------------*/
namespace nsBattle {
	class StatusPanel : public Panel {
	private:
		//属性と名前
		noDel_ptr<GameObject> pAttrObj;
		noDel_ptr<GameObject> pNameText;

		//HP,MPの表示用
		noDel_ptr<GameObject> pHPBar;
		noDel_ptr<GameObject> pMPBar;
		noDel_ptr<GameObject> pHPText;
		noDel_ptr<GameObject> pMPText;
		noDel_ptr<GameObject> pHPFrame;
		noDel_ptr<GameObject> pMPFrame;

	private:
		void Awake() override;

	public:
		void Open(noDel_ptr<BattleChara> cbattleCharahara);
	};
}
