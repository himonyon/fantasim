#pragma once

#define TERRITORY_NUM (27)
#define COUNTRY_NUM (6)
#define CITY_NUM (TERRITORY_NUM - COUNTRY_NUM)
#define CITY_SIZE (32.0f)
#define COUNTRY_SIZE (64.0f)
#define CHARACTOR_NUM (60)
#define OWN_CHARACTOR_NUM (5)

#include "Behaviour/Cursor.h"
#include "Behaviour/City.h"
#include "Behaviour/Country.h"
#include "Behaviour/Panel/SelectCountryPanel.h"
#include "Behaviour/Panel/CityPanel.h"

namespace nsStrategy {
	class SceneStrategy : public Scene {
	private:
		//ステージ
		noDel_ptr<GameObject> pBg;
		noDel_ptr<GameObject> pTerritories[TERRITORY_NUM];
		std::vector<noDel_ptr<GameObject>> vpRoad;

		//カーソル
		noDel_ptr<GameObject> pCursor;

		//パネル
		noDel_ptr<GameObject> pSelectCountryPanel;
		noDel_ptr<GameObject> pCityPanel;

		//キャラクター
		std::unordered_map<int, Charactor*> umCharactors;

	private:
		//初期化
		void Initialize();

		//削除
		void Terminate();

		//処理
		void Execute();

		//描画
		void Render();

		void Init();


		//領地の初期化
		void CreateCharactor();
		void CreateTerritory();
		void CreatePanels();
	};
}
