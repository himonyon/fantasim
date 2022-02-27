#include "../../../../../../framework.h"
#include "../../../../../../environment.h"

using namespace nsStrategy;

void PlayerTurn::Awake() {
	CreatePanels();

	//�^�[����Ԃ�������
	SetTurnState(eTurnState::Field, 0);
}

void PlayerTurn::Start() {
	//�T�E���h�}�l�[�W���[
	pSoundManager = gameObject->FindGameObject("soundManager")->GetComponent<SoundManager>();
	//�������
	pOperation = gameObject->FindGameObject("operation")->GetComponent<Operation>();

	//�p�l��
	pInvestPanel = gameObject->FindGameObject("investPanel")->GetComponent<InvestPanel>();
	pSelectNeighborPanel = gameObject->FindGameObject("selectNeighborPanel")->GetComponent<SelectNeighborPanel>();
	pBattlePanel = gameObject->FindGameObject("battlePanel")->GetComponent<BattlePanel>();
	pCharaSelectPanel = gameObject->FindGameObject("charaSelectPanel")->GetComponent<CharaSelectPanel>();
	pCharaEnhancePanel = gameObject->FindGameObject("charaEnhancePanel")->GetComponent<CharaEnhancePanel>();
	pOptionMenu = gameObject->FindGameObject("optionMenu")->GetComponent<OptionMenu>();
	pOptionPanel = gameObject->FindGameObject("optionPanel")->GetComponent<OptionPanel>();
	pInfoPanel = gameObject->FindGameObject("infoPanel")->GetComponent<InfoPanel>();
	pEventPanel = gameObject->FindGameObject("eventPanel")->GetComponent<EventPanel>();
}

void PlayerTurn::TurnInit() {
	pEventPanel->Open();
	SetTurnState(eTurnState::Field, 0);
}

void PlayerTurn::Update() {
	if(Keyboard::Trg(DIK_F10)) Debug::open = true;

	//���y�̃��[�v
	if (pSoundManager->IsPlaying("bgm") == false) pSoundManager->Play("bgm");
	//���p�l�����J���Ă���Ώ������Ȃ�
	if (pInfoPanel->IsOpen()) return;
	//�C�x���g�p�l�����J���Ă���Ώ������Ȃ�
	if (pEventPanel->IsOpen()) return;

	//�t�B�[���h���
	if (turnState->state == eTurnState::Field) FieldFunc();
	//�Z�[�u
	if (turnState->state == eTurnState::Save) SaveFunc();
	//�^�[���G���h
	if (turnState->state == eTurnState::TurnEnd) TurnEndFunc();
	//�I�v�V�������j���[
	if (turnState->state == eTurnState::OptionMenu) OptionMenuFunc();
	//�t�B�[���h���
	if (turnState->state == eTurnState::Option) OptionFunc();
	//�R�}���h�I�����
	if (turnState->state == eTurnState::Command) CommandFunc();
	//�o�ϗ͌���R�}���h
	if (turnState->state == eTurnState::InvEconomic) InvEconomicFunc();
	//����R�}���h
	if (turnState->state == eTurnState::InvPolitical) InvPoliticalFunc();
	//�ЊQ�ϐ��R�}���h
	if (turnState->state == eTurnState::InvDisaster) InvDisasterFunc();
	//�x�����R�}���h
	if (turnState->state == eTurnState::InvSupport) InvSupportFunc();
	//�����ړ���I���R�}���h
	if (turnState->state == eTurnState::SelMoneyMoveToCity) SelMoneyMoveToCityFunc();
	//�����ړ��R�}���h
	if (turnState->state == eTurnState::MoveMoney) MoveMoneyFunc();
	//�L�����ړ���I���R�}���h
	if (turnState->state == eTurnState::SelCharaMoveToCity) SelCharaMoveToCityFunc();
	//�L�����ړ��R�}���h
	if (turnState->state == eTurnState::MoveChara) MoveCharaFunc();
	//�o�g����I���R�}���h
	if (turnState->state == eTurnState::SelBattleCity) SelBattleCityFunc();
	//�o�g���R�}���h
	if (turnState->state == eTurnState::Battle) BattleFunc();
	//�L���������R�}���h
	if (turnState->state == eTurnState::CharaEnhance) CharaEnhance();

	if (turnState->state != checkState) {
		checkState = turnState->state;
	}
}

void PlayerTurn::CreatePanels() {
	pCityPanel = gameObject->FindGameObject("cityPanel")->GetComponent<CityPanel>();
}

//�e����
void PlayerTurn::FieldFunc() {
	//��x��������
	if (turnState->state != checkState) {
		//��������e�L�X�g�ύX
		pOperation->ResetOperation();
		pOperation->AddOperation("decide", L"�X�I��");
		pOperation->AddOperation("option", L"���j���[");
	}

	//�R�}���h���j���[�ֈڍs
	if (Input::Trg(InputConfig::input["decide"])) {
		if (pFocusCity != NULL) {
			pSoundManager->Play("decide"); //���艹
			//��ԑJ��
			SetTurnState(eTurnState::Command, 0);
			pSelectCity = pFocusCity;
		}
	}

	//�I�v�V�������j���[�ֈڍs
	if (Input::Trg(InputConfig::input["option"])) {
		pSoundManager->Play("decide"); //���艹
		//��ԑJ��
		SetTurnState(eTurnState::OptionMenu, 0);
	}
}
void PlayerTurn::SaveFunc() {
	//�Z�[�u
	gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->SaveGame();
	//��ԑJ��
	SetTurnState(eTurnState::Field, 0);
	//���p�l���\��
	pInfoPanel->Open(L"�Z�[�u���܂���");
}
void PlayerTurn::TurnEndFunc() {
	//�^�[���I��
	gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->ChangeTurn();
}
void PlayerTurn::OptionMenuFunc() {
	//�I�v�V�������j���[���J��
	if (pOptionMenu->IsOpen() == false) pOptionMenu->Open();
}
void PlayerTurn::OptionFunc() {
	//�I�v�V�����p�l�����J��
	if (pOptionPanel->IsOpen() == false) pOptionPanel->Open();
}
void PlayerTurn::CommandFunc() {
	//�X�̃p�l�����J��
	if (pCityPanel->IsOpen() == false) pCityPanel->Open(pSelectCity);
}
void PlayerTurn::InvEconomicFunc() {
	if (pInvestPanel->IsOpen() == false) pInvestPanel->Open(pSelectCity, (int)eCommand::Economic);
}
void PlayerTurn::InvPoliticalFunc() {
	if (pInvestPanel->IsOpen() == false) pInvestPanel->Open(pSelectCity, (int)eCommand::Political);
}
void PlayerTurn::InvDisasterFunc() {
	if (pInvestPanel->IsOpen() == false) pInvestPanel->Open(pSelectCity, (int)eCommand::Disaster);
}
void PlayerTurn::InvSupportFunc() {
	if (pInvestPanel->IsOpen() == false) pInvestPanel->Open(pSelectCity, (int)eCommand::Support);
}
void PlayerTurn::SelMoneyMoveToCityFunc() {
	if (pSelectNeighborPanel->IsOpen() == false) pSelectNeighborPanel->Open(pSelectCity, (int)eCommand::MoneyMove);
}
void PlayerTurn::MoveMoneyFunc() {
	if (pInvestPanel->IsOpen() == false) pInvestPanel->Open(pSelectCity, pTargetCity);
}
void PlayerTurn::SelCharaMoveToCityFunc() {
	if (pSelectNeighborPanel->IsOpen() == false) pSelectNeighborPanel->Open(pSelectCity, (int)eCommand::CharaMove);
}
void PlayerTurn::MoveCharaFunc() {
	if (pCharaSelectPanel->IsOpen() == false) pCharaSelectPanel->Open(pSelectCity, pTargetCity);
}
void PlayerTurn::SelBattleCityFunc() {
	if (pSelectNeighborPanel->IsOpen() == false) pSelectNeighborPanel->Open(pSelectCity, (int)eCommand::Battle);
}
void PlayerTurn::BattleFunc() {
	if (pBattlePanel->IsOpen() == false) pBattlePanel->Open(pSelectCity, pTargetCity, true);
}
void PlayerTurn::CharaEnhance() {
	if (pCharaEnhancePanel->IsOpen() == false) pCharaEnhancePanel->Open(pSelectCity);
}