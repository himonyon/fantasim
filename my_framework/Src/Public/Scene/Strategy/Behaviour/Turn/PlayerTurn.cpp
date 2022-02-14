#include "../../../../../../framework.h"
#include "../../../../../../environment.h"

using namespace nsStrategy;

void PlayerTurn::Awake() {
	CreatePanels();

	//サウンドマネージャー
	pSoundManager = gameObject->FindGameObject("soundManager")->GetComponent<SoundManager>();

	//ターン状態を初期化
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
	//フィールド状態
	if (turnState->state == eTurnState::Field) FieldFunc();
	//コマンド選択状態
	if (turnState->state == eTurnState::Command) CommandFunc();
	//経済力向上コマンド
	if (turnState->state == eTurnState::InvEconomic) InvEconomicFunc();
	//政策コマンド
	if (turnState->state == eTurnState::InvPolitical) InvPoliticalFunc();
	//災害耐性コマンド
	if (turnState->state == eTurnState::InvDisaster) InvDisasterFunc();
	//支持率コマンド
	if (turnState->state == eTurnState::InvSupport) InvSupportFunc();
	//資金移動先選択コマンド
	if (turnState->state == eTurnState::SelMoneyMoveToCity) SelMoneyMoveToCityFunc();
	//資金移動コマンド
	if (turnState->state == eTurnState::MoveMoney) MoveMoneyFunc();
	//キャラ移動先選択コマンド
	if (turnState->state == eTurnState::SelCharaMoveToCity) SelCharaMoveToCityFunc();
	//キャラ移動コマンド
	if (turnState->state == eTurnState::MoveChara) MoveCharaFunc();
	//バトル先選択コマンド
	if (turnState->state == eTurnState::SelBattleCity) SelBattleCityFunc();
	//バトルコマンド
	if (turnState->state == eTurnState::Battle) BattleFunc();
	//キャラ強化コマンド
	if (turnState->state == eTurnState::CharaEnhance) CharaEnhance();
}

void PlayerTurn::CreatePanels() {
	//パネル画像
	noDel_ptr<Sprite> panel_sp = CreateSprite(new Sprite(L"Data/Image/Common/menu_button.spr", L"panel"));

	//街の情報と行動一覧画面作成
	noDel_ptr<GameObject> _pCityPanel = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, 700, 480, panel_sp, nullptr, "cityPanel");
	_pCityPanel->GetComponent<ImageRenderer>()->SetRenderPriority((int)eRenderOrder::UI);
	_pCityPanel->AddComponent<CityPanel>();
	pCityPanel = _pCityPanel->GetComponent<CityPanel>();
}

//各処理（パネルを開く）
void PlayerTurn::FieldFunc() {
	//コマンドメニューへ移行
	if (Input::Trg(InputConfig::input["decide"])) {
		if (pFocusCity != NULL) {
			pSoundManager->Play("decide"); //決定音
			//状態遷移
			SetTurnState(eTurnState::Command, 0);
			pSelectCity = pFocusCity;
		}
	}

	//ターン切り替え
	if (Input::Trg(InputConfig::input["option"])) {
		gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->ChangeTurn();
	}

	//セーブ
	if (Keyboard::Trg(DIK_O)) {
		gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->SaveGame();
	}
}
void PlayerTurn::CommandFunc() {
	//街のパネルを開く
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