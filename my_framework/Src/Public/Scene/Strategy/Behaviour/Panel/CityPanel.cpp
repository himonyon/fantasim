#include "../../../../../../framework.h"
#include "../../../../../../environment.h"

using namespace nsStrategy;

void CityPanel::Awake() {
	//サウンドマネージャー
	pSoundManager = gameObject->FindGameObject("soundManager")->GetComponent<SoundManager>();

	//マップカーソル
	pMapCursor = gameObject->FindGameObject("cursor");

	//スプライト情報取得
	noDel_ptr<ImageRenderer> spr = gameObject->GetComponent<ImageRenderer>();

	//遷移先パネル
	//投資パネル
	noDel_ptr<GameObject> _pInvestPanel = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, 400, 150, 
		spr->pRenderSprite,	nullptr, "investPanel");
	_pInvestPanel->AddComponent<InvestPanel>();
	_pInvestPanel->SetRenderOrder((int)eRenderOrder::UI);
	//バトルパネル
	noDel_ptr<GameObject> _pBattlePanel = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, 400, 300, spr->pRenderSprite,
		nullptr, "battlePanel");
	_pBattlePanel->AddComponent<BattlePanel>();
	_pBattlePanel->SetRenderOrder((int)eRenderOrder::UI);
	//キャラ選択パネル
	noDel_ptr<GameObject> _pCharaSelectPanel = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, 300, 300,
		spr->pRenderSprite, nullptr, "charaSelectPanel");
	_pCharaSelectPanel->AddComponent<CharaSelectPanel>();
	_pCharaSelectPanel->SetRenderOrder((int)eRenderOrder::UI);
	//近隣選択パネル
	noDel_ptr<GameObject> _pSelectNeighborPanel = gameObject->CreateImageObject(200, 70, 350, 100, spr->pRenderSprite,
		nullptr, "selectNeighborPanel");
	_pSelectNeighborPanel->AddComponent<SelectNeighborPanel>();
	_pSelectNeighborPanel->SetRenderOrder((int)eRenderOrder::UI);
	//キャラ強化パネル
	noDel_ptr<GameObject> _pCharaEnhancePanel = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER,
		600, 500, spr->pRenderSprite,nullptr, "charaEnhancePanel");
	_pCharaEnhancePanel->AddComponent<CharaEnhancePanel>();
	_pCharaEnhancePanel->SetRenderOrder((int)eRenderOrder::UI);
	//情報表示パネル
	noDel_ptr<GameObject> _pInfoPanel = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER,
		300, 150, spr->pRenderSprite, nullptr, "infoPanel");
	_pInfoPanel->AddComponent<InfoPanel>();
	_pInfoPanel->SetRenderOrder((int)eRenderOrder::UI);
	//オプションメニュー
	noDel_ptr<GameObject> _pOptionMenu = gameObject->CreateObject(0,0,0, nullptr, "optionMenu");
	_pOptionMenu->AddComponent<OptionMenu>();
	//オプションパネル
	noDel_ptr<GameObject> _pOptionPanel = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER,
		300, 150, spr->pRenderSprite, nullptr, "optionPanel");
	_pOptionPanel->AddComponent<OptionPanel>();
	_pOptionPanel->SetRenderOrder((int)eRenderOrder::UI);
	//イベントパネル
	noDel_ptr<GameObject> _pEventPanel = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER,
		700, 600, spr->pRenderSprite, nullptr, "eventPanel");
	_pEventPanel->AddComponent<EventPanel>();
	_pEventPanel->SetRenderOrder((int)eRenderOrder::UI);

	//街情報テキスト
	pCityInfoPanel = gameObject->CreateObject(0, 0, 0, transform);

	pCityNameText = gameObject->CreateObject(0, 0, 0, transform);
	pCityNameText->AddComponent<Font>();
	pCityNameText->SetRenderOrder((int)eRenderOrder::FrontUI);

	pCityStatusText = gameObject->CreateObject(0, 0, 0, pCityInfoPanel->transform);
	pCityStatusText->AddComponent<Font>();
	pCityStatusText->SetRenderOrder((int)eRenderOrder::FrontUI);

	//キャラクター一覧表示テキスト
	pCharaInfoPanel = gameObject->CreateObject(0, 0, 0, transform);
	for (int i = 0; i < OWN_CHARACTOR_NUM; i++) {
		pCharaInfoText[i] = gameObject->CreateObject(0, 0, 0, pCharaInfoPanel->transform);
		pCharaInfoText[i]->AddComponent<Font>();
		pCharaInfoText[i]->SetRenderOrder((int)eRenderOrder::FrontUI);

		pAttrObj[i] = gameObject->CreateObject(0, 0, 0, pCharaInfoText[i]->transform);
		pAttrObj[i]->AddComponent<ImageRenderer>();
		pAttrObj[i]->SetRenderOrder((int)eRenderOrder::FrontObject);
	}

	//行動一覧テキスト
	pCommandPanel = gameObject->CreateObject(0, 0, 0, transform);

	//セレクトカーソル
	pSelectCursor = gameObject->CreateImageObject(0, 0, 100, 30, CreateSprite(
		new Sprite(L"Data/Image/Common/square.spr")),pCommandPanel->transform);
	pSelectCursor->SetRenderOrder((int)eRenderOrder::FrontObject);

	//ボーダー
	for (int i = 0; i < 2; i++) {
		pBorderText[i] = gameObject->CreateObject(0, 0, 0, pCommandPanel->transform);
		pBorderText[i]->AddComponent<Font>();
		pBorderText[i]->SetRenderOrder((int)eRenderOrder::FrontUI);
		pBorderText[i]->GetComponent<Font>()->Print(L"---------------------------------------------------------------------\n");
	}

	//コマンド＆コマンド説明テキスト
	pCommandTitleText = gameObject->CreateObject(0, 0, 0, pCommandPanel->transform);
	pCommandTitleText->AddComponent<Font>();
	pCommandTitleText->SetRenderOrder((int)eRenderOrder::FrontUI);
	pCommandTitleText->GetComponent<Font>()->Print(L"・コマンド");

	for (int i = 0; i < (int)eCommand::Max; i++) {
		pCommand[i] = gameObject->CreateObject(0, 0, 0, pCommandPanel->transform);
		pCommandTexts[i] = gameObject->CreateObject(0, 0, 0, pCommandPanel->transform);
		pCommand[i]->AddComponent<Font>();
		pCommandTexts[i]->AddComponent<Font>();
		pCommand[i]->SetRenderOrder((int)eRenderOrder::FrontUI);
		pCommandTexts[i]->SetRenderOrder((int)eRenderOrder::FrontUI);
	}

	pCommand[(int)eCommand::Economic]->GetComponent<Font>()->Print(L"生産力向上");
	pCommandTexts[(int)eCommand::Economic]->GetComponent<Font>()->Print(L"経済力が上昇。4月/10月の収入額に影響");
	pCommand[(int)eCommand::Political]->GetComponent<Font>()->Print(L"政策");
	pCommandTexts[(int)eCommand::Political]->GetComponent<Font>()->Print(L"政治力が上昇。コマンド実行上限数に影響");
	pCommand[(int)eCommand::Disaster]->GetComponent<Font>()->Print(L"災害対策");
	pCommandTexts[(int)eCommand::Disaster]->GetComponent<Font>()->Print(L"災害耐性が上昇。災害時の各ステータス減少値に影響");
	pCommand[(int)eCommand::Support]->GetComponent<Font>()->Print(L"演説");
	pCommandTexts[(int)eCommand::Support]->GetComponent<Font>()->Print(L"支持率上昇。暴動時の各ステータス減少値に影響");
	pCommand[(int)eCommand::MoneyMove]->GetComponent<Font>()->Print(L"資金移動");
	pCommandTexts[(int)eCommand::MoneyMove]->GetComponent<Font>()->Print(L"近接している自国、または街に資金を移動する");
	pCommand[(int)eCommand::CharaMove]->GetComponent<Font>()->Print(L"キャラ派遣");
	pCommandTexts[(int)eCommand::CharaMove]->GetComponent<Font>()->Print(L"近接している自国、または街へ指定したキャラクターを移動させる");
	pCommand[(int)eCommand::Battle]->GetComponent<Font>()->Print(L"戦闘");
	pCommandTexts[(int)eCommand::Battle]->GetComponent<Font>()->Print(L"他の国、または街へ戦闘を仕掛ける");
	pCommand[(int)eCommand::CharaEnhance]->GetComponent<Font>()->Print(L"キャラ強化");
	pCommandTexts[(int)eCommand::CharaEnhance]->GetComponent<Font>()->Print(L"この国、街にいるキャラクターを強化する");

	//初期状態で隠しておく
	gameObject->SetObjEnable(false);
}

void CityPanel::Update() {
	if (Input::Trg(InputConfig::input["cancel"])) {
		pSoundManager->Play("cancel"); //cancel音
		Close();
		gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->SetTurnState(eTurnState::Field);
		pMapCursor->SetObjEnable(true);
	}
	if (Input::Trg(InputConfig::input["changeTab"])) {
		pCityInfoPanel->SetObjEnable(pCityInfoPanel->IsObjEnable() ? false : true);
		pCharaInfoPanel->SetObjEnable(pCharaInfoPanel->IsObjEnable() ? false : true);
	}

	if (isPlayerCity) {
		PlayerCityFunc();
	}
}

void CityPanel::Close() {
	Panel::Close();
}

void CityPanel::Open(noDel_ptr<City> city) {
	pMapCursor->SetObjEnable(false);

	pCity = city;

	//操作説明テキスト変更
	noDel_ptr<Operation> _opr = gameObject->FindGameObject("operation")->GetComponent<Operation>();
	_opr->ResetOperation();
	_opr->AddOperation("decide", L"選択");
	_opr->AddOperation("cancel", L"戻る");
	_opr->AddOperation("changeTab", L"表示切替");

	noDel_ptr<ImageRenderer> spr = gameObject->GetComponent<ImageRenderer>();

	if (pCity->pCountry->IsPlayer()) {
		spr->size.y = 450.0f;
		isPlayerCity = true;
	}
	else {
		spr->size.y = 180.0f;
		isPlayerCity = false;
	}

	float _top = transform->position.y - spr->size.y / 2;
	float _left = transform->position.x - spr->size.x / 2;

	SetCityStatus(_top, _left);

	Panel::Open();
}

void CityPanel::PlayerCityFunc() {
	MoveSelectCursor();
	TransOtherState();
}

void CityPanel::SetCityStatus(float top, float left) {
	float _leftPadding = 40.0f;
	float _topPadding = isPlayerCity ? 50.0f : 30.0f;

	//テキスト情報
	float _textPadding = 40.0f;
	//街の名前
	pCityNameText->transform->SetPosition(left + _leftPadding, top + _topPadding);
	noDel_ptr<Font> _cityName_font = pCityNameText->GetComponent<Font>();
	_cityName_font->Print(L"%s : %s", pCity->pCountry->GetName().c_str(), pCity->GetName().c_str());
	_cityName_font->SetColor(ColorConvert16(pCity->gameObject->GetComponent<ImageRenderer>()->GetColor()));

	//街のステータス
	pCityStatusText->transform->SetPosition(left + _leftPadding, top + _topPadding + _textPadding);
	pCityStatusText->GetComponent<Font>()->Print(L"所有額: %d  経済力: %d  政策力: %d\n"
		"災害耐性: %d%%  支持率: %d%%  景気: %s", pCity->money, pCity->economic, pCity->political,
		pCity->disaster, pCity->support, pCity->GetBussinessText().c_str());

	//所持キャラの表示
	const int _rowNum = 2;
	for (int i = 0; i < OWN_CHARACTOR_NUM; i++) {
		//所持キャラがいなければ表示をしない
		if (i >= pCity->vOwnChara.size()) {
			pCharaInfoText[i]->SetObjEnable(false);
			pAttrObj[i]->SetObjEnable(false);
			continue;
		}

		pCharaInfoText[i]->SetObjEnable(true);
		pAttrObj[i]->SetObjEnable(true);
		_textPadding = 30.0f;
		const float _x = left + 30.0f + (_leftPadding + (i % _rowNum) * 300.0f);
		const float _y = top + (_topPadding + ((i / _rowNum) + 1) * _textPadding);
		pCharaInfoText[i]->transform->SetPosition(_x, _y);
		pAttrObj[i]->transform->SetLocalPosition(-20.0f, 12.0f);
		pAttrObj[i]->GetComponent<ImageRenderer>()->SetUpImageRenderer(25.0f,25.0f,
			noDel_ptr<Sprite>(pCity->vOwnChara[i]->GetAttr()->GetSprite()));
		pCharaInfoText[i]->GetComponent<Font>()->Print(L"%s 戦闘力:%d", pCity->vOwnChara[i]->GetName().c_str(),
			pCity->vOwnChara[i]->power);
	}

	pCityInfoPanel->SetObjEnable(true);
	pCharaInfoPanel->SetObjEnable(false);

	//プレイヤーの所有地ならコマンドを表示する
	if (pCity->vOwnChara.size() > _rowNum * 2) top = pCharaInfoText[_rowNum*2]->transform->position.y;
	else top = pCityStatusText->transform->position.y + 20.0f;

	if (isPlayerCity) SetCommandPanel(top, left);
	else pCommandPanel->SetObjEnable(false);
}

void CityPanel::SetCommandPanel(float top, float left) {
	float _leftPadding = 40.0f;
	float _topPadding = 40.0f;

	pBorderText[0]->transform->SetPosition(left + _leftPadding, top + _topPadding);
	pCommandTitleText->transform->SetPosition(left + _leftPadding, top + _topPadding + 30.0f);
	pCommandTitleText->GetComponent<Font>()->Print(L"・コマンド  残行動回数: %d/%d", 
		pCity->GetActEnableNum() - pCity->GetActCount(), pCity->GetActEnableNum());

	_topPadding = 60.0f;

	//行動可能かどうか
	bool _actEnable = true;
	if (pCity->GetActCount() >= pCity->GetActEnableNum()) _actEnable = false;

	//コマンド
	const int _rowNum = 4;
	const float _textPadding = 50.0f;
	for (int i = 0; i < sizeof(pCommand) / sizeof(pCommand[0]); i++) {
		const float _x = left + (_leftPadding + (i % _rowNum) * 150.0f);
		const float _y = top + (_topPadding + ((i / _rowNum) + 1) * _textPadding);
		pCommand[i]->transform->SetPosition(_x, _y);
		if (_actEnable) pCommand[i]->GetComponent<Font>()->SetColor(0xffffffff);
		else pCommand[i]->GetComponent<Font>()->SetColor(0x22ffffff);
	}

	//セレクトカーソル
	cursorNum = 0;
	pSelectCursor->transform->SetPosition(pCommand[(int)eCommand::Economic]->transform->position.x + 45.0f,
		pCommand[(int)eCommand::Economic]->transform->position.y + 12.5f);

	//コマンドテキスト
 	pBorderText[1]->transform->SetPosition(left + _leftPadding, 
		pCommand[_rowNum]->transform->position.y + _textPadding);
	for (int i = 0; i < sizeof(pCommand) / sizeof(pCommand[0]); i++) {
		if(i == (int)eCommand::Economic) pCommandTexts[i]->SetObjEnable(true);
		else pCommandTexts[i]->SetObjEnable(false);
		pCommandTexts[i]->transform->SetPosition(left + _leftPadding, pBorderText[1]->transform->position.y + 40.0f);
	}

	pCommandPanel->SetObjEnable(true);
}

//セレクトカーソル移動
void CityPanel::MoveSelectCursor() {
	//コマンドの選択
	const int _col = 2; //コマンドの行数
	const int _row = 4; //コマンドの列数

	int _cur = cursorNum;

	if (Input::Trg(InputConfig::input["down"])) if (cursorNum + _row < (int)eCommand::Max) cursorNum += _row;
	if (Input::Trg(InputConfig::input["up"])) if (cursorNum - _row >= 0) cursorNum -= _row;
	if (Input::Trg(InputConfig::input["right"])) cursorNum = CountUpInRange(cursorNum, (int)eCommand::Max - 1);
	if (Input::Trg(InputConfig::input["left"]))	cursorNum = CountDownInRange(cursorNum, (int)eCommand::Max - 1);

	//変更があれば↓
	if (_cur != cursorNum) {
		//セレクトカーソル位置変更
		pSelectCursor->transform->SetPosition(pCommand[cursorNum]->transform->position.x + 45.0f,
			pCommand[cursorNum]->transform->position.y + 12.5f);
		//テキスト切り替え
		pCommandTexts[_cur]->SetObjEnable(false);
		pCommandTexts[cursorNum]->SetObjEnable(true);
	}
}

//ターン状態遷移
void CityPanel::TransOtherState() {
	if (pCity->GetActCount() >= pCity->GetActEnableNum()) return;
	if (Input::Trg(InputConfig::input["decide"])) {
		pSoundManager->Play("decide"); //決定音
		//遷移
		noDel_ptr<GameManager> _pGM = gameObject->FindGameObject("gameManager")->GetComponent<GameManager>();
		if (cursorNum == (int)eCommand::MoneyMove) _pGM->SetTurnState(eTurnState::SelMoneyMoveToCity);
		else if (cursorNum == (int)eCommand::CharaMove) _pGM->SetTurnState(eTurnState::SelCharaMoveToCity);
		else if (cursorNum == (int)eCommand::Battle) _pGM->SetTurnState(eTurnState::SelBattleCity);
		else if (cursorNum == (int)eCommand::CharaEnhance) _pGM->SetTurnState(eTurnState::CharaEnhance);
		else if (cursorNum == (int)eCommand::Economic) _pGM->SetTurnState(eTurnState::InvEconomic);
		else if (cursorNum == (int)eCommand::Political) _pGM->SetTurnState(eTurnState::InvPolitical);
		else if (cursorNum == (int)eCommand::Disaster) _pGM->SetTurnState(eTurnState::InvDisaster);
		else if (cursorNum == (int)eCommand::Support) _pGM->SetTurnState(eTurnState::InvSupport);
		Close();
	}
}