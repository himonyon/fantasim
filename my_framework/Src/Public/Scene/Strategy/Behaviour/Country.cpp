#include "../../../../../framework.h"
#include "../../../../../environment.h"

using namespace nsStrategy;

void Country::Start() {
	City::Start();

	//陥落済みなら街の処理で終了
	if (isFalled)return;

	gameObject->GetComponent<ImageRenderer>()->SetUpImageRenderer(COUNTRY_SIZE, COUNTRY_SIZE, pCountry_sp);

	 pCityFrame->SetObjEnable(false);
}

void Country::Update() {
}

void Country::SetSprite(noDel_ptr<Sprite> country, noDel_ptr<Sprite> city, noDel_ptr<Sprite> frame) {
	pCountry_sp = country;
	pCity_sp = city;
	pCityFrame_sp = frame;
}

//所属変更
void Country::ChangeBelongCountry(noDel_ptr<Country> country) {
	//変更前の国
	noDel_ptr<Country> _befCountry = pCountry;

	if (country == pCountry) return; //同国なら処理しない

	//国の街配列から抜く
	if (pCountry != NULL) pCountry->PullOutCity(noDel_ptr<City>(this));

	//所属の変更
	pCountry = country;
	pCountry->SetOwnCity(noDel_ptr<City>(this));
	gameObject->GetComponent<ImageRenderer>()->SetColor(pCountry->gameObject->GetComponent<ImageRenderer>()->GetColor());

	//プレイヤーの国ならゲーム終了
	if (isPlayer) gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->EndGame(false);

	if (isFalled) return;

	//自国のほかの街も所属を変更する
	if (vOwnCities.size() != 0) {
		for (auto itr = vOwnCities.begin(); itr != vOwnCities.end();) {
			itr->get()->pCountry = country;
			country->SetOwnCity(noDel_ptr<City>(itr->get()));
			itr->get()->gameObject->GetComponent<ImageRenderer>()->
				SetColor(pCountry->gameObject->GetComponent<ImageRenderer>()->GetColor());
			itr = vOwnCities.erase(itr);
		}
	}
	
	if (!isFalled) {
		isFalled = true;
		gameObject->GetComponent<ImageRenderer>()->SetUpImageRenderer(CITY_SIZE, CITY_SIZE, pCity_sp);
		if(pCityFrame != NULL) pCityFrame->SetObjEnable(true);
	}

	//ゲームクリアチェック
	gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->CheckClear();
}

void Country::SetIsPlayer() {
	isPlayer = true;
	if (pPlayerText == 0) {
		pPlayerText = gameObject->CreateObject(transform->position.x, transform->position.y + 40.0f, 0);
		pPlayerText->AddComponent<Font>();
		noDel_ptr<Font> font = pPlayerText->GetComponent<Font>();
		font->gameObject->SetRenderOrder((int)eRenderOrder::UI - 1);
		font->SetTextAlignment(eTextAlignment::Center);
		font->SetFontSize(20.0f);
		font->Print(L"Player");
	}
}

//街の配列から街を抜く
void Country::PullOutCity(noDel_ptr<City> city) {
	if (vOwnCities.size() == 0) return;
	for (auto itr = vOwnCities.begin(); itr != vOwnCities.end(); itr++) {
		if (itr->get()->GetID() == city->GetID()) {
			itr = vOwnCities.erase(itr);
			return;
		}
	}
}
//街を配列に加える
void Country::SetOwnCity(noDel_ptr<City> city) {
	if (id == city->GetID()) return;
	vOwnCities.emplace_back(city);
}