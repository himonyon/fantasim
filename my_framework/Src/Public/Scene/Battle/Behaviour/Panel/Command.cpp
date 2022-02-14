#include "../../../../../../framework.h"
#include "../../../../../../environment.h"
#include "Command.h"

using namespace nsBattle;

void CommandPanel::Awake() {

	//親の位置設定
	transform->SetPosition(SCREEN_WIDTH - 100.0f, 50.0f);

	//背景画像
	noDel_ptr<Sprite> pBackGround_sp = CreateSprite(new Sprite(L"Data/Image/Common/menu_button.spr"));

	//セレクトカーソル
	pSelecter = gameObject->CreateImageObject(0, 0, 150.0f, 80.0f,
		CreateSprite(new Sprite(L"Data/Image/Common/square.spr")), transform);
	pSelecter->GetComponent<ImageRenderer>()->SetRenderPriority(5);
	pSelecter->GetComponent<ImageRenderer>()->SetColor(1, 1, 1, 0.6f);

	//移動
	CreateCommand(L"移動", pBackGround_sp, eCommand::Move);

	//攻撃
	CreateCommand(L"スキル", pBackGround_sp, eCommand::Skill);

	//待機
	CreateCommand(L"待機", pBackGround_sp, eCommand::Wait);

	//戻る
	CreateCommand(L"戻る", pBackGround_sp, eCommand::Back);

	//初期は非表示
	gameObject->SetObjEnable(false);
}

void CommandPanel::Update() {
	//選択処理
	if (commandState == eCommand::Select) {
		if (Input::Trg(InputConfig::input["cancel"])) {
			BackFunc();
		}

		//コマンドカーソルの移動
		MoveSelecter();

		//コマンドの選択
		if (Input::Trg(InputConfig::input["decide"])) SelectCommand();
	}

	//移動処理
	if (commandState == eCommand::Move) {
		gameObject->FindGameObject("fieldManager")->
			GetComponent<FieldManager>()->SetTurnState(eTurnState::Move);
		Close();
	}

	//スキル処理
	if (commandState == eCommand::Skill) {
		gameObject->FindGameObject("fieldManager")->
			GetComponent<FieldManager>()->SetTurnState(eTurnState::SelectSkill);
		Close();
	}

	//待機処理
	if (commandState == eCommand::Wait) {
		pSelectChara->SetActionEnable(false);
		gameObject->FindGameObject("fieldManager")->
			GetComponent<FieldManager>()->SetTurnState(eTurnState::Field);
		Close();
	}

	//戻る処理
	if (commandState == eCommand::Back) {
		BackFunc();
	}
}

void CommandPanel::Open(noDel_ptr<PlayerChara> chara, eCommandPanelType type) {
	Panel::Open();

	//初期化
	pSelectChara = chara;
	commandState = eCommand::Select;
	selectNum = 0;
	for (auto& com : umCommand) com.second->SetComEnable(false);
	vCommand.clear();

	//コマンド作成
	panelType = type;

	//パネルの種類に応じてコマンドをセットする
	if (type == eCommandPanelType::MoveBefore) {
		vCommand.emplace_back(umCommand[eCommand::Move]);
		vCommand.emplace_back(umCommand[eCommand::Skill]);
		vCommand.emplace_back(umCommand[eCommand::Back]);
	}
	else if (type == eCommandPanelType::MoveAfter) {
		vCommand.emplace_back(umCommand[eCommand::Skill]);
		vCommand.emplace_back(umCommand[eCommand::Wait]);
	}

	//コマンド表示
	for (auto& com : vCommand) com->SetComEnable(true);

	//コマンド位置設定
	SetCommandPos();

	//カーソル移動を止める
	gameObject->FindGameObject("cursor")->GetComponent<Cursor>()->SetEnable(false);
}

void CommandPanel::MoveSelecter() {
	if (Input::Trg(InputConfig::input["up"])) {
		selectNum = CountDownInRange(selectNum, (int)vCommand.size() - 1);
		//選択位置設定
		pSelecter->transform->position = vCommand[selectNum]->pBackGorund->transform->position;
	}

	if (Input::Trg(InputConfig::input["down"])) {
		selectNum = CountUpInRange(selectNum, (int)vCommand.size() - 1);
		//選択位置設定
		pSelecter->transform->position = vCommand[selectNum]->pBackGorund->transform->position;
	}
}

void CommandPanel::SelectCommand() {
	if(vCommand[selectNum]->type == eCommand::Move) commandState = eCommand::Move;
	if(vCommand[selectNum]->type == eCommand::Skill) commandState = eCommand::Skill;
	if(vCommand[selectNum]->type == eCommand::Wait) commandState = eCommand::Wait;
	if(vCommand[selectNum]->type == eCommand::Back) commandState = eCommand::Back;

	//カーソル移動再開
	if(commandState != eCommand::Select) gameObject->FindGameObject("cursor")->
		GetComponent<Cursor>()->SetEnable(true);
}

void CommandPanel::CreateCommand(const WCHAR* text,noDel_ptr<Sprite> bgImage, eCommand type) {
	float sizeX = 150.0f;
	float sizeY = 80.0f;

	float _localY = sizeY * (int)vCommand.size();

	//コマンド作成
	std::shared_ptr<stCommand> _command = std::make_unique<stCommand>();

	_command->pBackGorund = gameObject->CreateImageObject(0, 0, sizeX, sizeY, bgImage, transform);
	_command->pText = gameObject->CreateObject(0, 0, 0, _command->pBackGorund->transform);
	_command->pText->AddComponent<Font>();
	noDel_ptr<Font> _font = _command->pText->GetComponent<Font>();
	_font->SetTextAlignment(eTextAlignment::Center);
	_font->SetFontSize(22.0f);
	_font->Print(text);
	_font->SetRenderPriority(10);

	_command->type = type;

	umCommand[type] = _command;
}

void CommandPanel::SetCommandPos() {
	float sizeX = 150.0f;
	float sizeY = 80.0f;
	
	int count = 0;
	for (auto& com : vCommand) {
		float _localY = sizeY * count;

		com->pBackGorund->transform->SetLocalPosition(0, _localY);
		com->pText->transform->SetLocalPosition(0, -10);

		count++;
	}

	//カーソル位置設定
	pSelecter->transform->SetLocalPosition(0, 0);
}

void CommandPanel::BackFunc() {
	//移動後なら元の位置に戻す
	if (panelType == eCommandPanelType::MoveAfter) pSelectChara->BackBeforePos();
	//フィールドマネージャーに状態変更を送る
	noDel_ptr<FieldManager> _fm = gameObject->FindGameObject("fieldManager")->
		GetComponent<FieldManager>();
	if (panelType == eCommandPanelType::MoveBefore) _fm->SetTurnState(eTurnState::Back);
	else _fm->SetTurnState(eTurnState::Back, 2);
	//閉じる
	Close();
}