/*-----------------------------------------------------------

	CharaSelectPanelクラス
		キャラクターを選択する画面

-------------------------------------------------------------*/
#pragma once

namespace nsStrategy {
	class CharaSelectPanel : public Panel {
	private:
		//Findobject
		noDel_ptr<GameObject> pCityPanel;

	private:
		//対象の国、街
		noDel_ptr<City> pCity = 0;
		noDel_ptr<City> pCity2 = 0;

		noDel_ptr<GameObject> pDescText;
		noDel_ptr<GameObject> pBorderText;

		noDel_ptr<GameObject> pCharaText[OWN_CHARACTOR_NUM];

		noDel_ptr<GameObject> pDecideText;

		std::vector<noDel_ptr<Charactor>> vMoveCharas;

		//カーソル
		noDel_ptr<GameObject> pSelectCursor;
		int cursorNum = 0;
		int maxCursorNum = 0;

	private:
		void AddMoveChara();
		void DecideMoveChara();

	public:
		void Awake() override;
		void Update() override;

		void Open(noDel_ptr<City> city, noDel_ptr<City> city2);
		void Close() override;
	};
}
