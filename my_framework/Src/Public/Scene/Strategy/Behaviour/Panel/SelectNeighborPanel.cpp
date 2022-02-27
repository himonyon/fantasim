#include "../../../../../../framework.h"
#include "../../../../../../environment.h"

using namespace nsStrategy;

void SelectNeighborPanel::Awake() {
	//�T�E���h�}�l�[�W���[
	pSoundManager = gameObject->FindGameObject("soundManager")->GetComponent<SoundManager>();

	noDel_ptr<Font> _font; //�R���|�[�l���g�擾�p

	pDescText = gameObject->CreateObject(0, 0, 0, transform);
	pDescText->transform->SetLocalPosition(0, -30.0f);
	pDescText->AddComponent<Font>();
	_font = pDescText->GetComponent<Font>();
	_font->Print(L"�Ώۂ̍��E�X��I�����Ă�������");
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
	_font->Print(L"���@�@�@�@�@�@�@�@�@�@      ��");
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
		pSoundManager->Play("cancel"); //cancel��
		vSelectableCities[selectNum]->FocusCity(false);
		gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->SetTurnState(eTurnState::Back);
		Close();
	}
	if (Input::Trg(InputConfig::input["decide"])) {
		pSoundManager->Play("decide"); //���艹
		//�Q�[���}�l�[�W���[
		noDel_ptr<GameManager> _pGM = gameObject->FindGameObject("gameManager")->GetComponent<GameManager>();
		//�t�H�[�J�X�I��
		vSelectableCities[selectNum]->FocusCity(false);
		//�^�[�Q�b�g�ɐݒ�
		_pGM->SetTargetCity(vSelectableCities[selectNum]);
		//�^�[���J��
		if(commandType == (int)eCommand::MoneyMove) _pGM->SetTurnState(eTurnState::MoveMoney);
		else if (commandType == (int)eCommand::Battle) _pGM->SetTurnState(eTurnState::Battle);
		else if (commandType == (int)eCommand::CharaMove)_pGM->SetTurnState(eTurnState::MoveChara);
		Close();
	}
}

void SelectNeighborPanel::Open(noDel_ptr<City> city, int cmd) {
	pCity = city;
	commandType = cmd;

	//�X�ɃL�����N�^�[�����Ȃ��ꍇ
	if (cmd == (int)eCommand::Battle || cmd == (int)eCommand::CharaMove) {
		if (pCity->vOwnChara.size() == 0) {
			Close();
			gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->SetTurnState(eTurnState::Back);
			return;
		}
	}

	//��������e�L�X�g�ύX
	noDel_ptr<Operation> _opr = gameObject->FindGameObject("operation")->GetComponent<Operation>();
	_opr->ResetOperation();
	_opr->AddOperation("decide", L"�I��");
	_opr->AddOperation("cancel", L"�߂�");

	//�Ώۂ̒���ݒ肷��
	for (auto& city : pCity->GetNeighbor()) {
		if (cmd == (int)eCommand::Battle) {
			if (city->pCountry != pCity->pCountry) vSelectableCities.emplace_back(city);
		}
		else {
			if (city->pCountry == pCity->pCountry) vSelectableCities.emplace_back(city);
		}
	}

	//�Ώۂ�������Ȃ��ꍇ
	if (vSelectableCities.size() == 0) {
		Close();
		gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->SetTurnState(eTurnState::Back);
	}
	//�Ώۂ����������ꍇ
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

