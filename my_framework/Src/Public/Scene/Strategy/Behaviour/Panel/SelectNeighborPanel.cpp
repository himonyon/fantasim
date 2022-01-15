#include "../../../../../../framework.h"
#include "../../../../../../environment.h"

#include "SelectNeighborPanel.h"

using namespace nsStrategy;

void SelectNeighborPanel::Awake() {
	pCityPanel = gameObject->FindGameObject("cityPanel");
	pInvestPanel = gameObject->FindGameObject("investPanel");
	pBattlePanel = gameObject->FindGameObject("battlePanel");
	pCharaSelectPanel = gameObject->FindGameObject("charaSelectPanel");

	noDel_ptr<Font> _font; //コンポーネント取得用

	pDescText = gameObject->CreateObject(0, 0, 0, transform);
	pDescText->transform->SetLocalPosition(0, -30.0f);
	pDescText->AddComponent<Font>();
	_font = pDescText->GetComponent<Font>();
	_font->Print(L"対象の国・街を選択してください");
	_font->SetTextAlignment(eTextAlignment::Center);
	_font->SetRenderPriority((int)eRenderOrder::FrontUI);

	pNameText = gameObject->CreateObject(0, 0, 0, transform);
	pNameText->transform->SetLocalPosition(0, 10.0f);
	pNameText->AddComponent<Font>();
	_font = pNameText->GetComponent<Font>();
	_font->SetTextAlignment(eTextAlignment::Center);
	_font->SetRenderPriority((int)eRenderOrder::FrontUI);

	pArrowText = gameObject->CreateObject(0, 0, 0, transform);
	pArrowText->transform->SetLocalPosition(0, 10.0f);
	pArrowText->AddComponent<Font>();
	_font = pArrowText->GetComponent<Font>();
	_font->Print(L"←　　　　　　　　　　      →");
	_font->SetTextAlignment(eTextAlignment::Center);
	_font->SetRenderPriority((int)eRenderOrder::FrontUI);

	gameObject->SetObjEnable(false);
}

void SelectNeighborPanel::Update() {

	if (Input::Trg(InputConfig::input["right"])) {
		vSelectableCities[selectNum]->FocusCity(false);
		selectNum++;
		if (selectNum == vSelectableCities.size()) selectNum = 0;
		vSelectableCities[selectNum]->FocusCity(true);
		pNameText->GetComponent<Font>()->Print(L"%s", vSelectableCities[selectNum]->GetName().c_str());
	}
	if (Input::Trg(InputConfig::input["left"])) {
		vSelectableCities[selectNum]->FocusCity(false);
		selectNum--;
		if (selectNum < 0) selectNum = (int)vSelectableCities.size() - 1;
		vSelectableCities[selectNum]->FocusCity(true);
		pNameText->GetComponent<Font>()->Print(L"%s", vSelectableCities[selectNum]->GetName().c_str());
	}

	if (Input::Trg(InputConfig::input["cancel"])) {
		vSelectableCities[selectNum]->FocusCity(false);
		Close();
		pCityPanel->GetComponent<CityPanel>()->Open(pCity);
	}
	if (Input::Trg(InputConfig::input["decide"])) {
		vSelectableCities[selectNum]->FocusCity(false);
		if(commandType == (int)eCommand::MoneyMove)
			pInvestPanel->GetComponent<InvestPanel>()->Open(pCity,vSelectableCities[selectNum]);
		if (commandType == (int)eCommand::Battle)
			pBattlePanel->GetComponent<BattlePanel>()->Open(pCity, vSelectableCities[selectNum]);
		if (commandType == (int)eCommand::CharaMove)
			pCharaSelectPanel->GetComponent<CharaSelectPanel>()->Open(pCity, vSelectableCities[selectNum]);
		Close();
	}
}

void SelectNeighborPanel::Open(noDel_ptr<City> city, int cmd) {
	pCity = city;
	commandType = cmd;

	//街にキャラクターがいない場合
	if (cmd == (int)eCommand::Battle || cmd == (int)eCommand::CharaMove) {
		if (pCity->vOwnChara.size() == 0) {
			Close();
			pCityPanel->GetComponent<CityPanel>()->Open(pCity);
			return;
		}
	}

	//対象の町を設定する
	for (auto& city : pCity->GetNeighbor()) {
		if (cmd == (int)eCommand::Battle) {
			if (city->pCountry != pCity->pCountry) vSelectableCities.emplace_back(city);
		}
		else {
			if (city->pCountry == pCity->pCountry) vSelectableCities.emplace_back(city);
		}
	}

	//対象が見つからない場合
	if (vSelectableCities.size() == 0) {
		Close();
		pCityPanel->GetComponent<CityPanel>()->Open(pCity);
	}
	//対象が見つかった場合
	else {
		pNameText->GetComponent<Font>()->Print(L"%s", vSelectableCities[selectNum]->GetName().c_str());
		vSelectableCities[selectNum]->FocusCity(true);
		Panel::Open();
	}
}

void SelectNeighborPanel::Close() {
	vSelectableCities.clear();
	selectNum = 0;

	Panel::Close();
}

