#include "../../../../framework.h"
#include "../../../../environment.h"

using namespace nsStrategy;

//初期化
void SceneStrategy::Initialize() {
	Init();
}

void SceneStrategy::Init() {
	pCursor = CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, 32.0f, 32.0f,
		CreateSprite(new Sprite(L"Data/Image/Strategy/cursor.spr")), nullptr, "cursor");
	pCursor->GetComponent<ImageRenderer>()->SetRenderPriority((int)eRenderOrder::Object + 2);
	pCursor->AddComponent<Physics2D>();
	pCursor->AddComponent<Collider2D>();
	pCursor->GetComponent<Collider2D>()->SetUpCollider2D(false);
	pCursor->AddComponent<Cursor>();

	CreateCharactor();
	CreateTerritory();
	CreatePanels();

	isInitialized = true;
}

void SceneStrategy::Terminate() {
	//キャラクター削除
	for (auto& chara : umCharactors) {
		delete chara.second;
	}
	umCharactors.clear();
}

//処理
void SceneStrategy::Execute() {
	Scene::Execute();
}

//描画
void SceneStrategy::Render() {
	Scene::Render();
}

void SceneStrategy::CreateCharactor() {
	for (int i = 0; i < CHARACTOR_NUM; i++) {
		Charactor* chara = new Charactor(i);
		umCharactors[chara->GetID()] = chara;
	}
}

void SceneStrategy::CreateTerritory() {
	//背景の作成
	pBg = CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, SCREEN_WIDTH, SCREEN_HEIGHT,
		CreateSprite(new Sprite(L"Data/Image/Strategy/bg.spr")));
	pBg->GetComponent<ImageRenderer>()->SetRenderPriority((int)eRenderOrder::Background);

	//国の色
	stColor4 _colors[COUNTRY_NUM] = {
		{0,0,0,1},
		{1,0.5f,1,1},
		{0,0.5f,1,1},
		{1,1,0.3f,1},
		{1,0,0,1},
		{1,0.6f,0,1},
	};

	//全領地共通で利用する画像
	noDel_ptr<Sprite> pCountry_sp = CreateSprite(new Sprite(L"Data/Image/Strategy/country.spr"));
	noDel_ptr<Sprite> pCity_sp = CreateSprite(new Sprite(L"Data/Image/Strategy/circle_icon.spr"));
	noDel_ptr<Sprite> pCityFrame_sp = CreateSprite(new Sprite(L"Data/Image/Strategy/circle_frame.spr"));
	noDel_ptr<Sprite> pRoad_sp = CreateSprite(new Sprite(L"Data/Image/Strategy/road.spr"));

	//国と街のゲームオブジェクトの作成
	for (int i = 0; i < TERRITORY_NUM; i++) {
		std::string _name = "city" + std::to_string(i);
		pTerritories[i] = CreateObject(0, 0, 0, nullptr, _name);
		pTerritories[i]->AddComponent<ImageRenderer>();
		noDel_ptr<ImageRenderer> spr = pTerritories[i]->GetComponent<ImageRenderer>();
		spr->SetRenderPriority((int)eRenderOrder::Object);;
		if (i < COUNTRY_NUM) {
			spr->SetColor(_colors[i]);
			pTerritories[i]->AddComponent<Country>();
			pTerritories[i]->GetComponent<Country>()->SetSprite(pCountry_sp, pCity_sp, pCityFrame_sp);
		}
		else {
			pTerritories[i]->AddComponent<City>();
			pTerritories[i]->GetComponent<City>()->SetSprite(pCity_sp, pCityFrame_sp);
		}
	}
	
	//ファイルから各街のデータを読み込む
	FILE* fp;
	if (0 == fopen_s(&fp, "Data/Status/Strategy/baseData.txt", "r")) {
		for (int i = 0; i < TERRITORY_NUM; i++) {
			//街のコンポーネント取得
			noDel_ptr<City> city = pTerritories[i]->GetComponent<City>();
			
			//各ステータス設定
			char _key[256] = { 0 };
			while (true)
			{
				//キーワード読み込み
				fscanf_s(fp, "%s", _key, (int)sizeof(_key));
				
				if (strcmp(_key, "name") == 0) {
					fscanf_s(fp, "%s", _key, (int)sizeof(_key));
					WCHAR temp[32] = L"";
					//名前を変換して格納
					MultiByteToWideChar(CP_ACP,	MB_PRECOMPOSED,(LPCCH)_key,_countof(_key),temp,	32);
					city->SetName(temp);
				}
				else if (strcmp(_key, "id") == 0) {
					int _id = 0;
					fscanf_s(fp, "%d", &_id);
					city->SetID(_id);
				}
				else if (strcmp(_key, "country") == 0) {
					int _country = 0;
					fscanf_s(fp, "%d", &_country);
					if (i < COUNTRY_NUM) city->pCountry = pTerritories[_country]->GetComponent<Country>();
					else city->ChangeBelongCountry(pTerritories[_country]->GetComponent<Country>());
				}
				else if (strcmp(_key, "position") == 0) {
					float posX = 0, posY = 0;
					fscanf_s(fp, "%f %f", &city->transform->position.x, &city->transform->position.y);
				}
				else if (strcmp(_key, "neighbor") == 0) {
					while (true) {
						fscanf_s(fp, "%s", _key, (int)sizeof(_key));
						if (strcmp(_key, "money") == 0) {
							fscanf_s(fp, "%d", &city->money);
							break;
						}
						else city->SetNeighbor(pTerritories[atoi(_key) - 1]->GetComponent<City>());
					}
				}
				else if (strcmp(_key, "economic") == 0) {
					fscanf_s(fp, "%d", &city->economic);
				}
				else if (strcmp(_key, "political") == 0) {
					fscanf_s(fp, "%d", &city->political);
				}
				else if (strcmp(_key, "disaster") == 0) {
					fscanf_s(fp, "%d", &city->disaster);
				}
				else if (strcmp(_key, "support") == 0) {
					fscanf_s(fp, "%d", &city->support);
				}
				else if (strcmp(_key, "business") == 0) {
					fscanf_s(fp, "%f", &city->bussiness);
				}
				else if (strcmp(_key, "chara") == 0) {
					while (!feof(fp)) {
						fscanf_s(fp, "%s", _key, (int)sizeof(_key));
						if (strcmp(_key, "new") == 0) break;
						int _charaID = atoi(_key);
						city->vOwnChara.emplace_back(noDel_ptr<Charactor>(umCharactors[_charaID]));
					}
					break;
				}
			}
		}
		fclose(fp);
	}

	//道の表示設定
	for (int i = 0; i < TERRITORY_NUM; i++) {
		noDel_ptr<City> city1 = pTerritories[i]->GetComponent<City>();
		for (int k = 0; k < city1->GetNeighbor().size(); k++) {
			noDel_ptr<City> city2 = city1->GetNeighbor()[k];
			if (city1->GetID() > city2->GetID()) continue;
			float _x = city1->transform->position.x; float _x2 = city2->transform->position.x;
			float _y = city1->transform->position.y; float _y2 = city2->transform->position.y;

			float _roadPosX = _x - (_x - _x2) / 2;
			float _roadPosY = _y - (_y - _y2) / 2;
			float _roadSizeX = sqrtf(powf(_x2 - _x, 2) + powf(_y2 - _y, 2));
			float _roadRot = atan2f(_y2 - _y, _x2 - _x);
			noDel_ptr<GameObject> road = CreateImageObject(_roadPosX, _roadPosY, _roadSizeX, 12.0f,pRoad_sp);
			noDel_ptr<ImageRenderer> spr = road->GetComponent<ImageRenderer>();
			spr->SetRenderPriority((int)eRenderOrder::BackObject);;
			spr->SetColor(0.6f, 0.3f, 0.0f, 0.8f);
			road->transform->rotation.z = _roadRot;
			vpRoad.emplace_back(road);
		}
	}
}

void SceneStrategy::CreatePanels() {
	//パネル画像
	noDel_ptr<Sprite> panel_sp = CreateSprite(new Sprite(L"Data/Image/Common/menu_button.spr", L"panel"));

	//国選択画面
	pSelectCountryPanel = CreateImageObject(400, 200, 300, 280, panel_sp);
	pSelectCountryPanel->GetComponent<ImageRenderer>()->SetRenderPriority((int)eRenderOrder::UI);
	pSelectCountryPanel->AddComponent<SelectCountryPanel>();

	//街の情報と行動一覧画面作成
	pCityPanel = CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, 700, 480, panel_sp, nullptr, "cityPanel");
	pCityPanel->GetComponent<ImageRenderer>()->SetRenderPriority((int)eRenderOrder::UI);
	pCityPanel->AddComponent<CityPanel>();
}
