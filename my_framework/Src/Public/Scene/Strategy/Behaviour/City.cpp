#include "../../../../../framework.h"
#include "../../../../../environment.h"

#include "City.h"

using namespace nsStrategy;

void City::Start() {
	noDel_ptr<ImageRenderer> spr = gameObject->GetComponent<ImageRenderer>();
	spr->SetUpRenderer2D(CITY_SIZE, CITY_SIZE, pCity_sp);
	gameObject->AddComponent<Collider2D>();
	gameObject->GetComponent<Collider2D>()->SetUpCollider2D(false);

	pCityFrame = gameObject->CreateImageObject(transform->position.x, transform->position.y, spr->sizeX, spr->sizeY, pCityFrame_sp, transform);
	pCityFrame->GetComponent<ImageRenderer>()->SetRenderPriority((int)eRenderOrder::Object + 1);

	pCityPanel = gameObject->FindGameObject("cityPanel");
}

void City::Update() {

}

//�����蔻��
void City::OnTrigger2D(noDel_ptr<Collider2D> hitCol) {
	if (Input::Trg(InputConfig::input["decide"])) {
		pCityPanel->GetComponent<CityPanel>()->Open(noDel_ptr<City>(this));
	}
}
void City::OnTriggerEnter2D(noDel_ptr<Collider2D> hitCol) {
	FocusCity(true);
}
void City::OnTriggerExit2D(noDel_ptr<Collider2D> hitCol) {
	FocusCity(false);
}

//�����ύX
void City::ChangeBelongCountry(noDel_ptr<Country> country) {
	pCountry = country;
	gameObject->GetComponent<ImageRenderer>()->SetColor(country->gameObject->GetComponent<ImageRenderer>()->GetColor());
}

void City::SetSprite(noDel_ptr<Sprite> city, noDel_ptr<Sprite> frame) {
	pCity_sp = city;
	pCityFrame_sp = frame;
}

void City::FocusCity(bool trg) {
	transform->scale.x = trg ? 1.4f : 1.0f;
	transform->scale.y = trg ? 1.4f : 1.0f;
}

//�i�C�̃e�L�X�g�̎擾
std::wstring City::GetBussinessText() {
	if (bussiness < 0.8f) return L"��";
	else if (bussiness > 1.2f) return L"��";
	return L"��";
}