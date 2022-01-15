#include "../../../../../../framework.h"
#include "../../../../../../environment.h"

#include "InvestPanel.h"

using namespace nsStrategy;

void InvestPanel::Awake() {
	pCityPanel = gameObject->FindGameObject("cityPanel");

	noDel_ptr<Font> _font; //コンポーネント取得用

	pDescText = gameObject->CreateObject(0, 0, 0, transform);
	pDescText->transform->SetLocalPosition(0, -40.0f);
	pDescText->AddComponent<Font>();
	_font = pDescText->GetComponent<Font>();
	_font->SetTextAlignment(eTextAlignment::Center);
	_font->SetRenderPriority((int)eRenderOrder::FrontUI);

	pPriceText = gameObject->CreateObject(0, 0, 0, transform);
	pPriceText->transform->SetLocalPosition(0, 10.0f);
	pPriceText->AddComponent<Font>();
	_font = pPriceText->GetComponent<Font>();
	_font->SetTextAlignment(eTextAlignment::Center);
	_font->SetRenderPriority((int)eRenderOrder::FrontUI);

	pResultText = gameObject->CreateObject(0, 0, 0, transform);
	pResultText->transform->SetLocalPosition(0, 0);
	pResultText->AddComponent<Font>();
	_font = pResultText->GetComponent<Font>();
	_font->SetTextAlignment(eTextAlignment::Center);
	_font->SetRenderPriority((int)eRenderOrder::FrontUI);


	gameObject->SetObjEnable(false);
}

void InvestPanel::Update() {
	//金額設定
	if (Input::On(InputConfig::input["up"]) || Input::On(InputConfig::input["down"])) {
		bool _isUp = false;
		bool _trg = false;
		if (Input::On(InputConfig::input["up"])) _isUp = true;
		if (pressCount == 0 || pressCount > 30) {
			if (pressCount % 5 == 0) _trg = true;
		}
		pressCount++;

		if (_trg) {
			//金額変更
			if (_isUp) {
				if (investMoney + 100 <= maxInvestMoney) {
					investMoney += 100;
					pPriceText->GetComponent<Font>()->Print(L"金額：%d (最大%d)", investMoney, maxInvestMoney);
				}
			}
			else {
				if (investMoney - 100 >= minInvestMoney) {
					investMoney -= 100;
					pPriceText->GetComponent<Font>()->Print(L"金額：%d (最大%d)", investMoney, maxInvestMoney);
				}
			}
		}
		
	}
	else {
		pressCount = 0;
	}

	//戻るボタンを決定ボタン処理
	if (Input::Trg(InputConfig::input["cancel"])) {
		Close();
		pCityPanel->SetObjEnable(true);
	}
	if (Input::Trg(InputConfig::input["decide"])) {
		if (isResult) {
			Close();
			pCityPanel->GetComponent<CityPanel>()->Open(pCity);
		}
		else {
			isResult = true;
			pPriceText->SetObjEnable(false);
			pResultText->SetObjEnable(true);
			InvestFunc(*this);
		}
	}
}


void InvestPanel::Open(noDel_ptr<City> city, int cmd) {
	pCity = city;
	isResult = false;

	investMoney = minInvestMoney;

	if (cmd == (int)eCommand::Economic) SetInvestEconomic();
	if (cmd == (int)eCommand::Political) SetInvestPolitical();
	if (cmd == (int)eCommand::Disaster) SetInvestDisaster();
	if (cmd == (int)eCommand::Support) SetInvestSupport();

	pDescText->SetObjEnable(true);
	pPriceText->SetObjEnable(true);
	pResultText->SetObjEnable(false);

	//所持金不足
	if (pCity->money < minInvestMoney) {
		pDescText->SetObjEnable(false);
		pPriceText->SetObjEnable(false);
		pResultText->SetObjEnable(true);
		pResultText->GetComponent<Font>()->Print(L"資金が足りません。");
		isResult = true;
	}

	Panel::Open();
}
void InvestPanel::Open(noDel_ptr<City> city, noDel_ptr<City> city2) {
	pCity = city;
	pCity2 = city2;
	isResult = false;

	investMoney = minInvestMoney;

	SetMoveMoney();

	pDescText->SetObjEnable(true);
	pPriceText->SetObjEnable(true);
	pResultText->SetObjEnable(false);

	//所持金不足
	if (pCity->money < minInvestMoney) {
		pDescText->SetObjEnable(false);
		pPriceText->SetObjEnable(false);
		pResultText->SetObjEnable(true);
		pResultText->GetComponent<Font>()->Print(L"資金が足りません。");
		isResult = true;
	}

	Panel::Open();
}

void InvestPanel::SetInvestEconomic() {
	//変更する値の元の値
	beforeMoney = pCity->money;
	beforeValue = pCity->economic;

	//最大値設定
	maxInvestMoney = 5000;
	if (pCity->money < maxInvestMoney) maxInvestMoney = pCity->money;

	//テキスト
	pDescText->GetComponent<Font>()->Print(L"投資する金額を設定してください");
	pPriceText->GetComponent<Font>()->Print(L"金額：%d (最大%d)", investMoney, maxInvestMoney);

	//関数ポインタを設定
	InvestFunc = &InvestPanel::InvestEconomic;
}
void InvestPanel::SetInvestPolitical() {
	//変更する値の元の値
	beforeMoney = pCity->money;
	beforeValue = pCity->political;

	//最大値設定
	maxInvestMoney = 5000;
	if (pCity->money < maxInvestMoney) maxInvestMoney = pCity->money;

	//テキスト
	pDescText->GetComponent<Font>()->Print(L"投資する金額を設定してください");
	pPriceText->GetComponent<Font>()->Print(L"金額：%d (最大%d)", investMoney, maxInvestMoney);

	//関数ポインタを設定
	InvestFunc = &InvestPanel::InvestPolitical;
}
void InvestPanel::SetInvestDisaster() {
	//変更する値の元の値
	beforeMoney = pCity->money;
	beforeValue = pCity->disaster;

	//最大値設定
	maxInvestMoney = 2000;
	if (pCity->money < maxInvestMoney) maxInvestMoney = pCity->money;

	//テキスト
	pDescText->GetComponent<Font>()->Print(L"投資する金額を設定してください");
	pPriceText->GetComponent<Font>()->Print(L"金額：%d (最大%d)", investMoney, maxInvestMoney);

	//関数ポインタを設定
	InvestFunc = &InvestPanel::InvestDisaster;
}
void InvestPanel::SetInvestSupport() {
	//変更する値の元の値
	beforeMoney = pCity->money;
	beforeValue = pCity->support;

	//最大値設定
	maxInvestMoney = 2000;
	if (pCity->money < maxInvestMoney) maxInvestMoney = pCity->money;

	//テキスト
	pDescText->GetComponent<Font>()->Print(L"投資する金額を設定してください");
	pPriceText->GetComponent<Font>()->Print(L"金額：%d (最大%d)", investMoney, maxInvestMoney);

	//関数ポインタを設定
	InvestFunc = &InvestPanel::InvestSupport;
}
void InvestPanel::SetMoveMoney() {
	//変更する値の元の値
	beforeMoney = pCity->money;
	beforeMoney2 = pCity2->money;
	beforeValue = pCity->support;

	//最大値設定
	maxInvestMoney = 5000;
	if (pCity->money < maxInvestMoney) maxInvestMoney = pCity->money;

	//テキスト
	pDescText->GetComponent<Font>()->Print(L"渡す金額を設定してください");
	pPriceText->GetComponent<Font>()->Print(L"金額：%d (最大%d)", investMoney, maxInvestMoney);

	//関数ポインタを設定
	InvestFunc = &InvestPanel::MoveMoney;
}

void InvestPanel::InvestEconomic() {
	pCity->money -= investMoney;
	pCity->economic += investMoney/10;

	pDescText->GetComponent<Font>()->Print(L"経済力が上昇しました！");
	pResultText->GetComponent<Font>()->Print(L"資金: %d --> %d\n経済力: %d --> %d",
		beforeMoney, pCity->money, beforeValue, pCity->economic);
}
void InvestPanel::InvestPolitical() {
	pCity->money -= investMoney;
	pCity->political += investMoney / 10;

	pDescText->GetComponent<Font>()->Print(L"政治力が上昇しました！");
	pResultText->GetComponent<Font>()->Print(L"資金: %d --> %d\n政治力: %d --> %d",
		beforeMoney, pCity->money, beforeValue, pCity->political);
}
void InvestPanel::InvestDisaster() {
	pCity->money -= investMoney;
	pCity->disaster += investMoney / 100;

	pDescText->GetComponent<Font>()->Print(L"災害耐性が上昇しました！");
	pResultText->GetComponent<Font>()->Print(L"資金: %d --> %d\n災害耐性: %d%% --> %d%%",
		beforeMoney, pCity->money, beforeValue, pCity->disaster);
}
void InvestPanel::InvestSupport() {
	pCity->money -= investMoney;
	pCity->support += investMoney / 100;

	pDescText->GetComponent<Font>()->Print(L"支持率が上昇しました！");
	pResultText->GetComponent<Font>()->Print(L"資金: %d --> %d\n支持率: %d%% --> %d%%",
		beforeMoney, pCity->money, beforeValue, pCity->support);
}
void InvestPanel::MoveMoney() {
	pCity->money -= investMoney;
	pCity2->money += investMoney;

	pDescText->GetComponent<Font>()->Print(L"支持率が上昇しました！");
	pResultText->GetComponent<Font>()->Print(L"資金(%s): %d --> %d\n資金(%s): %d --> %d",
		pCity->GetName().c_str(),beforeMoney, pCity->money, pCity2->GetName().c_str(),beforeMoney2, pCity2->money);
}
