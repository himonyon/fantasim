#include "../../../../../framework.h"
#include "../../../../../environment.h"

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
void City::OnTriggerEnter2D(noDel_ptr<Collider2D> hitCol) {
	gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->SetFocusCity(noDel_ptr<City>(this));
	FocusCity(true);
}
void City::OnTriggerExit2D(noDel_ptr<Collider2D> hitCol) {
	gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->SetFocusCity(NULL);
	FocusCity(false);
}

//�����ύX
void City::ChangeBelongCountry(noDel_ptr<Country> country) {
	//���̊X�z�񂩂甲��
	if(pCountry != NULL)pCountry->PullOutCity(noDel_ptr<City>(this));

	//�����̕ύX
	pCountry = country;
	pCountry->SetOwnCity(noDel_ptr<City>(this));
	gameObject->GetComponent<ImageRenderer>()->SetColor(pCountry->gameObject->GetComponent<ImageRenderer>()->GetColor());
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