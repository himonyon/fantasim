/*-----------------------------------------------------------

	BattlePanelクラス
		バトルに移行する前の確認画面

-------------------------------------------------------------*/
#pragma once

namespace nsStrategy {
	class BattlePanel : public Panel {
	private:
		//Findobject
		noDel_ptr<SoundManager> pSoundManager;

	private:
		//対象の国、街
		noDel_ptr<City> pPlayerCity;
		noDel_ptr<City> pEnemyCity;

		noDel_ptr<GameObject> pPlayerCityNameText;
		noDel_ptr<GameObject> pEnemyCityNameText;

		noDel_ptr<GameObject> pStartText;

		noDel_ptr<GameObject> pPlayerCharaText[OWN_CHARACTOR_NUM];
		noDel_ptr<GameObject> pEnemyCharaText[OWN_CHARACTOR_NUM];

		//カーソル
		noDel_ptr<GameObject> pSelectCursor;

		//遷移中のトリガー
		bool isSwitchingScene = false;

		//戻る処理が可能か
		bool cancelEnable = true;

	public:
		void Awake() override;
		void Update() override;

		void Open(noDel_ptr<City> p_city, noDel_ptr<City> e_city, bool cancel);
		void Close() override;

	private:
		//キャラがいないか確認（いなければ勝利してパネル閉じる）
		bool CheckNoChara();
	};
}
