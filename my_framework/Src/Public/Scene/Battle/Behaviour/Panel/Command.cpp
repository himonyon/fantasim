#include "../../../../../../framework.h"
#include "../../../../../../environment.h"
#include "Command.h"

using namespace nsBattle;

void CommandPanel::Awake() {

	//カーソル取得
	pCursor = gameObject->FindGameObject("cursor")->GetComponent<Cursor>();

	//スキル選択画面
	pSkillSelectPanel = gameObject->CreateImageObject(300, 250, 500, 350, 
		CreateSprite(new Sprite(L"Data/Image/Battle/skill_select_bg.spr")));
	pSkillSelectPanel->AddComponent<SelectSkillPanel>();

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
	CreateCommand(L"移動", pBackGround_sp, eCommandState::Move);

	//攻撃
	CreateCommand(L"スキル", pBackGround_sp, eCommandState::Skill);

	//戻る
	CreateCommand(L"戻る", pBackGround_sp, eCommandState::Back);

	//初期は非表示
	gameObject->SetObjEnable(false);
}

void CommandPanel::Update() {
	//選択処理
	if (commandState == eCommandState::Select) {
		if (Input::Trg(InputConfig::input["cancel"])) {
			Turn::curState = eFieldState::Field;
			Close();
		}

		//コマンドカーソルの移動
		MoveSelecter();

		//コマンドの選択
		if (Input::Trg(InputConfig::input["decide"])) SelectCommand();
	}

	//移動処理
	if (commandState == eCommandState::Move) {
		pSelectChara->StartMoveFunc();
		Close();
	}

	if (commandState == eCommandState::Skill) {
		pSkillSelectPanel->GetComponent<SelectSkillPanel>()->Open(pSelectChara);
		Close();
	}

	//戻る処理
	if (commandState == eCommandState::Back) {
		Turn::curState = eFieldState::Field;
		Close();
	}
	
}

void CommandPanel::Open(noDel_ptr<PlayerChara> chara) {
	Panel::Open();

	//初期化
	pSelectChara = NULL;
	commandState = eCommandState::Select;

	//カーソル位置設定
	selectNum = 0;
	pSelecter->transform->position = vCommand[selectNum]->pBackGorund->transform->position;
	pSelectChara = chara;

	//カーソル移動を止める
	pCursor->SetEnable(false);
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
	if(vCommand[selectNum]->type == eCommandState::Move) commandState = eCommandState::Move;
	if(vCommand[selectNum]->type == eCommandState::Skill) commandState = eCommandState::Skill;
	if(vCommand[selectNum]->type == eCommandState::Back) commandState = eCommandState::Back;

	//カーソル移動再開
	if(commandState != eCommandState::Select) pCursor->SetEnable(true);
}

void CommandPanel::CreateCommand(const WCHAR* text,noDel_ptr<Sprite> bgImage, eCommandState type) {
	float sizeX = 150.0f;
	float sizeY = 80.0f;

	float _localY = sizeY * (int)vCommand.size();

	//コマンド作成
	std::shared_ptr<stCommand> _command = std::make_unique<stCommand>();

	_command->pBackGorund = gameObject->CreateImageObject(0, 0, sizeX, sizeY, bgImage, transform);
	_command->pBackGorund->transform->SetLocalPosition(0, _localY);
	_command->pText = gameObject->CreateObject(0, 0, 0, _command->pBackGorund->transform);
	_command->pText->transform->SetLocalPosition(0, -10);
	_command->pText->AddComponent<Font>();
	noDel_ptr<Font> _font = _command->pText->GetComponent<Font>();
	_font->SetTextAlignment(eTextAlignment::Center);
	_font->SetFontSize(22.0f);
	_font->Print(text);
	_font->SetRenderPriority(10);

	_command->type = type;

	vCommand.emplace_back(_command);
}