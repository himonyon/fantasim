#include "../../../../../../framework.h"
#include "../../../../../../environment.h"

#include "InvestPanel.h"

using namespace nsStrategy;

void InvestPanel::Awake() {
	pCityPanel = gameObject->FindGameObject("cityPanel");

	noDel_ptr<Font> _font; //�R���|�[�l���g�擾�p

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
	//���z�ݒ�
	if (Input::On(InputConfig::input["up"]) || Input::On(InputConfig::input["down"])) {
		bool _isUp = false;
		bool _trg = false;
		if (Input::On(InputConfig::input["up"])) _isUp = true;
		if (pressCount == 0 || pressCount > 30) {
			if (pressCount % 5 == 0) _trg = true;
		}
		pressCount++;

		if (_trg) {
			//���z�ύX
			if (_isUp) {
				if (investMoney + 100 <= maxInvestMoney) {
					investMoney += 100;
					pPriceText->GetComponent<Font>()->Print(L"���z�F%d (�ő�%d)", investMoney, maxInvestMoney);
				}
			}
			else {
				if (investMoney - 100 >= minInvestMoney) {
					investMoney -= 100;
					pPriceText->GetComponent<Font>()->Print(L"���z�F%d (�ő�%d)", investMoney, maxInvestMoney);
				}
			}
		}
		
	}
	else {
		pressCount = 0;
	}

	//�߂�{�^��������{�^������
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

	//�������s��
	if (pCity->money < minInvestMoney) {
		pDescText->SetObjEnable(false);
		pPriceText->SetObjEnable(false);
		pResultText->SetObjEnable(true);
		pResultText->GetComponent<Font>()->Print(L"����������܂���B");
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

	//�������s��
	if (pCity->money < minInvestMoney) {
		pDescText->SetObjEnable(false);
		pPriceText->SetObjEnable(false);
		pResultText->SetObjEnable(true);
		pResultText->GetComponent<Font>()->Print(L"����������܂���B");
		isResult = true;
	}

	Panel::Open();
}

void InvestPanel::SetInvestEconomic() {
	//�ύX����l�̌��̒l
	beforeMoney = pCity->money;
	beforeValue = pCity->economic;

	//�ő�l�ݒ�
	maxInvestMoney = 5000;
	if (pCity->money < maxInvestMoney) maxInvestMoney = pCity->money;

	//�e�L�X�g
	pDescText->GetComponent<Font>()->Print(L"����������z��ݒ肵�Ă�������");
	pPriceText->GetComponent<Font>()->Print(L"���z�F%d (�ő�%d)", investMoney, maxInvestMoney);

	//�֐��|�C���^��ݒ�
	InvestFunc = &InvestPanel::InvestEconomic;
}
void InvestPanel::SetInvestPolitical() {
	//�ύX����l�̌��̒l
	beforeMoney = pCity->money;
	beforeValue = pCity->political;

	//�ő�l�ݒ�
	maxInvestMoney = 5000;
	if (pCity->money < maxInvestMoney) maxInvestMoney = pCity->money;

	//�e�L�X�g
	pDescText->GetComponent<Font>()->Print(L"����������z��ݒ肵�Ă�������");
	pPriceText->GetComponent<Font>()->Print(L"���z�F%d (�ő�%d)", investMoney, maxInvestMoney);

	//�֐��|�C���^��ݒ�
	InvestFunc = &InvestPanel::InvestPolitical;
}
void InvestPanel::SetInvestDisaster() {
	//�ύX����l�̌��̒l
	beforeMoney = pCity->money;
	beforeValue = pCity->disaster;

	//�ő�l�ݒ�
	maxInvestMoney = 2000;
	if (pCity->money < maxInvestMoney) maxInvestMoney = pCity->money;

	//�e�L�X�g
	pDescText->GetComponent<Font>()->Print(L"����������z��ݒ肵�Ă�������");
	pPriceText->GetComponent<Font>()->Print(L"���z�F%d (�ő�%d)", investMoney, maxInvestMoney);

	//�֐��|�C���^��ݒ�
	InvestFunc = &InvestPanel::InvestDisaster;
}
void InvestPanel::SetInvestSupport() {
	//�ύX����l�̌��̒l
	beforeMoney = pCity->money;
	beforeValue = pCity->support;

	//�ő�l�ݒ�
	maxInvestMoney = 2000;
	if (pCity->money < maxInvestMoney) maxInvestMoney = pCity->money;

	//�e�L�X�g
	pDescText->GetComponent<Font>()->Print(L"����������z��ݒ肵�Ă�������");
	pPriceText->GetComponent<Font>()->Print(L"���z�F%d (�ő�%d)", investMoney, maxInvestMoney);

	//�֐��|�C���^��ݒ�
	InvestFunc = &InvestPanel::InvestSupport;
}
void InvestPanel::SetMoveMoney() {
	//�ύX����l�̌��̒l
	beforeMoney = pCity->money;
	beforeMoney2 = pCity2->money;
	beforeValue = pCity->support;

	//�ő�l�ݒ�
	maxInvestMoney = 5000;
	if (pCity->money < maxInvestMoney) maxInvestMoney = pCity->money;

	//�e�L�X�g
	pDescText->GetComponent<Font>()->Print(L"�n�����z��ݒ肵�Ă�������");
	pPriceText->GetComponent<Font>()->Print(L"���z�F%d (�ő�%d)", investMoney, maxInvestMoney);

	//�֐��|�C���^��ݒ�
	InvestFunc = &InvestPanel::MoveMoney;
}

void InvestPanel::InvestEconomic() {
	pCity->money -= investMoney;
	pCity->economic += investMoney/10;

	pDescText->GetComponent<Font>()->Print(L"�o�ϗ͂��㏸���܂����I");
	pResultText->GetComponent<Font>()->Print(L"����: %d --> %d\n�o�ϗ�: %d --> %d",
		beforeMoney, pCity->money, beforeValue, pCity->economic);
}
void InvestPanel::InvestPolitical() {
	pCity->money -= investMoney;
	pCity->political += investMoney / 10;

	pDescText->GetComponent<Font>()->Print(L"�����͂��㏸���܂����I");
	pResultText->GetComponent<Font>()->Print(L"����: %d --> %d\n������: %d --> %d",
		beforeMoney, pCity->money, beforeValue, pCity->political);
}
void InvestPanel::InvestDisaster() {
	pCity->money -= investMoney;
	pCity->disaster += investMoney / 100;

	pDescText->GetComponent<Font>()->Print(L"�ЊQ�ϐ����㏸���܂����I");
	pResultText->GetComponent<Font>()->Print(L"����: %d --> %d\n�ЊQ�ϐ�: %d%% --> %d%%",
		beforeMoney, pCity->money, beforeValue, pCity->disaster);
}
void InvestPanel::InvestSupport() {
	pCity->money -= investMoney;
	pCity->support += investMoney / 100;

	pDescText->GetComponent<Font>()->Print(L"�x�������㏸���܂����I");
	pResultText->GetComponent<Font>()->Print(L"����: %d --> %d\n�x����: %d%% --> %d%%",
		beforeMoney, pCity->money, beforeValue, pCity->support);
}
void InvestPanel::MoveMoney() {
	pCity->money -= investMoney;
	pCity2->money += investMoney;

	pDescText->GetComponent<Font>()->Print(L"�x�������㏸���܂����I");
	pResultText->GetComponent<Font>()->Print(L"����(%s): %d --> %d\n����(%s): %d --> %d",
		pCity->GetName().c_str(),beforeMoney, pCity->money, pCity2->GetName().c_str(),beforeMoney2, pCity2->money);
}
