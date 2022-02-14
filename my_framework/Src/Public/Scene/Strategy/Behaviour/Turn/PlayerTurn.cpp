#include "../../../../../../framework.h"
#include "../../../../../../environment.h"

using namespace nsStrategy;

void PlayerTurn::Awake() {
	CreatePanels();

	//�T�E���h�}�l�[�W���[
	pSoundManager = gameObject->FindGameObject("soundManager")->GetComponent<SoundManager>();

	//�^�[����Ԃ�������
	SetTurnState(eTurnState::Field, 0);
}

void PlayerTurn::Start() {
	pInvestPanel = gameObject->FindGameObject("investPanel")->GetComponent<InvestPanel>();
	pSelectNeighborPanel = gameObject->FindGameObject("selectNeighborPanel")->GetComponent<SelectNeighborPanel>();
	pBattlePanel = gameObject->FindGameObject("battlePanel")->GetComponent<BattlePanel>();
	pCharaSelectPanel = gameObject->FindGameObject("charaSelectPanel")->GetComponent<CharaSelectPanel>();
	pCharaEnhancePanel = gameObject->FindGameObject("charaEnhancePanel")->GetComponent<CharaEnhancePanel>();
}

void PlayerTurn::Update() {
	//�t�B�[���h���
	if (turnState->state == eTurnState::Field) FieldFunc();
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
}

void PlayerTurn::CreatePanels() {
	//�p�l���摜
	noDel_ptr<Sprite> panel_sp = CreateSprite(new Sprite(L"Data/Image/Common/menu_button.spr", L"panel"));

	//�X�̏��ƍs���ꗗ��ʍ쐬
	noDel_ptr<GameObject> _pCityPanel = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, 700, 480, panel_sp, nullptr, "cityPanel");
	_pCityPanel->GetComponent<ImageRenderer>()->SetRenderPriority((int)eRenderOrder::UI);
	_pCityPanel->AddComponent<CityPanel>();
	pCityPanel = _pCityPanel->GetComponent<CityPanel>();
}

//�e�����i�p�l�����J���j
void PlayerTurn::FieldFunc() {
	//�R�}���h���j���[�ֈڍs
	if (Input::Trg(InputConfig::input["decide"])) {
		if (pFocusCity != NULL) {
			pSoundManager->Play("decide"); //���艹
			//��ԑJ��
			SetTurnState(eTurnState::Command, 0);
			pSelectCity = pFocusCity;
		}
	}

	//�^�[���؂�ւ�
	if (Input::Trg(InputConfig::input["option"])) {
		gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->ChangeTurn();
	}

	//�Z�[�u
	if (Keyboard::Trg(DIK_O)) {
		gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->SaveGame();
	}
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