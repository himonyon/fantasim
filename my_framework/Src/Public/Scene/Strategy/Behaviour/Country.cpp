#include "../../../../../framework.h"
#include "../../../../../environment.h"

using namespace nsStrategy;

void Country::Start() {
	City::Start();

	//�ח��ς݂Ȃ�X�̏����ŏI��
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

//�����ύX
void Country::ChangeBelongCountry(noDel_ptr<Country> country) {
	//�ύX�O�̍�
	noDel_ptr<Country> _befCountry = pCountry;

	if (country == pCountry) return; //�����Ȃ珈�����Ȃ�

	//���̊X�z�񂩂甲��
	if (pCountry != NULL) pCountry->PullOutCity(noDel_ptr<City>(this));

	//�����̕ύX
	pCountry = country;
	pCountry->SetOwnCity(noDel_ptr<City>(this));
	gameObject->GetComponent<ImageRenderer>()->SetColor(pCountry->gameObject->GetComponent<ImageRenderer>()->GetColor());

	//�v���C���[�̍��Ȃ�Q�[���I��
	if (isPlayer) gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->EndGame(false);

	if (isFalled) return;

	//�����̂ق��̊X��������ύX����
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

	//�Q�[���N���A�`�F�b�N
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

//�X�̔z�񂩂�X�𔲂�
void Country::PullOutCity(noDel_ptr<City> city) {
	if (vOwnCities.size() == 0) return;
	for (auto itr = vOwnCities.begin(); itr != vOwnCities.end(); itr++) {
		if (itr->get()->GetID() == city->GetID()) {
			itr = vOwnCities.erase(itr);
			return;
		}
	}
}
//�X��z��ɉ�����
void Country::SetOwnCity(noDel_ptr<City> city) {
	if (id == city->GetID()) return;
	vOwnCities.emplace_back(city);
}