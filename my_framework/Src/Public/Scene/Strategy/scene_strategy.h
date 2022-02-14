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
#include "Behaviour/Turn/Turn.h"
#include "Behaviour/Turn/PlayerTurn.h"
#include "Behaviour/Turn/EnemyTurn.h"
#include "Save.h"
#include "Behaviour/GameManager.h"

namespace nsStrategy {
	class SceneStrategy : public Scene {
	private:
		//‰Šú‰»
		void Initialize();

		//íœ
		void Terminate();

		//ˆ—
		void Execute();

		//•`‰æ
		void Render();
	};
}
