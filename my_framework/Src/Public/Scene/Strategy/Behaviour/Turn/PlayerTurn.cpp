#include "../../../../../../framework.h"
#include "../../../../../../environment.h"

using namespace nsStrategy;

void PlayerTurn::Awake() {
	CreatePanels();

	//ターン状態を初期化
	SetTurnState(eTurnState::Field, 0);
}

void PlayerTurn::Start() {
	//サウンドマネージャー
	pSoundManager = gameObject->FindGameObject("soundManager")->GetComponent<SoundManager>();
	//操作説明
	pOperation = gameObject->FindGameObject("operation")->GetComponent<Operation>();

	//パネル
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

	//音楽のループ
	if (pSoundManager->IsPlaying("bgm") == false) pSoundManager->Play("bgm");
	//情報パネルが開いていれば処理しない
	if (pInfoPanel->IsOpen()) return;
	//イベントパネルが開いていれば処理しない
	if (pEventPanel->IsOpen()) return;

	//フィールド状態
	if (turnState->state == eTurnState::Field) FieldFunc();
	//セーブ
	if (turnState->state == eTurnState::Save) SaveFunc();
	//ターンエンド
	if (turnState->state == eTurnState::TurnEnd) TurnEndFunc();
	//オプションメニュー
	if (turnState->state == eTurnState::OptionMenu) OptionMenuFunc();
	//フィールド状態
	if (turnState->state == eTurnState::Option) OptionFunc();
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

	if (turnState->state != checkState) {
		checkState = turnState->state;
	}
}

void PlayerTurn::CreatePanels() {
	pCityPanel = gameObject->FindGameObject("cityPanel")->GetComponent<CityPanel>();
}

//各処理
void PlayerTurn::FieldFunc() {
	//一度だけ処理
	if (turnState->state != checkState) {
		//操作説明テキスト変更
		pOperation->ResetOperation();
		pOperation->AddOperation("decide", L"街選択");
		pOperation->AddOperation("option", L"メニュー");
	}

	//コマンドメニューへ移行
	if (Input::Trg(InputConfig::input["decide"])) {
		if (pFocusCity != NULL) {
			pSoundManager->Play("decide"); //決定音
			//状態遷移
			SetTurnState(eTurnState::Command, 0);
			pSelectCity = pFocusCity;
		}
	}

	//オプションメニューへ移行
	if (Input::Trg(InputConfig::input["option"])) {
		pSoundManager->Play("decide"); //決定音
		//状態遷移
		SetTurnState(eTurnState::OptionMenu, 0);
	}
}
void PlayerTurn::SaveFunc() {
	//セーブ
	gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->SaveGame();
	//状態遷移
	SetTurnState(eTurnState::Field, 0);
	//情報パネル表示
	pInfoPanel->Open(L"セーブしました");
}
void PlayerTurn::TurnEndFunc() {
	//ターン終了
	gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->ChangeTurn();
}
void PlayerTurn::OptionMenuFunc() {
	//オプションメニューを開く
	if (pOptionMenu->IsOpen() == false) pOptionMenu->Open();
}
void PlayerTurn::OptionFunc() {
	//オプションパネルを開く
	if (pOptionPanel->IsOpen() == false) pOptionPanel->Open();
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