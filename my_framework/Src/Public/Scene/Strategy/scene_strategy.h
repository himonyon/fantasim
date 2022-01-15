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
		//�X�e�[�W
		noDel_ptr<GameObject> pBg;
		noDel_ptr<GameObject> pTerritories[TERRITORY_NUM];
		std::vector<noDel_ptr<GameObject>> vpRoad;

		//�J�[�\��
		noDel_ptr<GameObject> pCursor;

		//�p�l��
		noDel_ptr<GameObject> pSelectCountryPanel;
		noDel_ptr<GameObject> pCityPanel;

		//�L�����N�^�[
		std::unordered_map<int, Charactor*> umCharactors;

	private:
		//������
		void Initialize();

		//�폜
		void Terminate();

		//����
		void Execute();

		//�`��
		void Render();

		void Init();


		//�̒n�̏�����
		void CreateCharactor();
		void CreateTerritory();
		void CreatePanels();
	};
}
