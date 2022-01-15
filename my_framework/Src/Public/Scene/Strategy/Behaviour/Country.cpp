#include "../../../../../framework.h"
#include "../../../../../environment.h"

using namespace nsStrategy;

void Country::Start() {
	City::Start();

	gameObject->GetComponent<ImageRenderer>()->SetUpRenderer2D(COUNTRY_SIZE, COUNTRY_SIZE, pCountry_sp);

	pCityFrame->SetObjEnable(false);
}

void Country::Update() {
}

void Country::SetSprite(noDel_ptr<Sprite> country, noDel_ptr<Sprite> city, noDel_ptr<Sprite> frame) {
	pCountry_sp = country;
	pCity_sp = city;
	pCityFrame_sp = frame;
}

void Country::SetIsPlayer() {
	isPlayer = true;
	if (pPlayerText == 0) {
		pPlayerText = gameObject->CreateObject(transform->position.x, transform->position.y + 40.0f, 0);
		pPlayerText->AddComponent<Font>();
		noDel_ptr<Font> font = pPlayerText->GetComponent<Font>();
		font->SetRenderPriority((int)eRenderOrder::UI - 1);
		font->SetTextAlignment(eTextAlignment::Center);
		font->SetFontSize(20.0f);
		font->Print(L"Player");
	}
}