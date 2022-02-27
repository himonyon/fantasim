/*-----------------------------------------------------------

	CityPanelクラス
		引数で渡された街の情報を表示するパネル

-------------------------------------------------------------*/
#pragma once
#include "SelectNeighborPanel.h"
#include "InvestPanel.h"
#include "BattlePanel.h"
#include "CharaSelectPanel.h"
#include "CharaEnhancePanel.h"
#include "OptionPanel.h"
#include "OptionMenu.h"
#include "InfoPanel.h"
#include "EventPanel.h"

namespace nsStrategy {

	enum class eCommand {
		Economic,
		Political,
		Disaster,
		Support,
		MoneyMove,
		CharaMove,
		CharaEnhance,
		Battle,
		Max
	};

	class CityPanel : public Panel {
	private:
		//Findする変数
		noDel_ptr<SoundManager> pSoundManager;
		noDel_ptr<GameObject> pMapCursor;

	private:
		//表示する街
		noDel_ptr<City> pCity;
		//カーソル
		noDel_ptr<GameObject> pSelectCursor;
		int cursorNum = 0;

		bool isPlayerCity = false; //開かれたパネルがプレイヤーの所有物か

		//テキスト---------------------------------------------------------
		//街の基本ステータス表示パネル
		noDel_ptr<GameObject> pCityInfoPanel; 
		noDel_ptr<GameObject> pCityNameText;
		noDel_ptr<GameObject> pCityStatusText;

		//キャラ一覧表示パネル
		noDel_ptr<GameObject> pCharaInfoPanel; 
		noDel_ptr<GameObject> pCharaInfoText[OWN_CHARACTOR_NUM];
		noDel_ptr<GameObject> pAttrObj[OWN_CHARACTOR_NUM];

		//行動一覧表示パネル
		noDel_ptr<GameObject> pBorderText[2];
		noDel_ptr<GameObject> pCommandPanel;
		noDel_ptr<GameObject> pCommandTitleText;
		noDel_ptr<GameObject> pCommand[(int)eCommand::Max];
		noDel_ptr<GameObject> pCommandTexts[(int)eCommand::Max];

	private:
		void Awake() override;
		void Update() override;

		void Close() override;

		//街の情報をセットする
		void SetCityStatus(float top, float left);
		void SetCommandPanel(float top, float left);

		//セレクトカーソル移動
		void MoveSelectCursor();

		//他のパネルへ遷移
		void TransOtherState();

	public:
		//パネルを開く
		void Open(noDel_ptr<City> city);

		//プレイヤーの操作
		void PlayerCityFunc();
	};
}
