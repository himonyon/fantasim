#include "../../../../../../framework.h"
#include "../../../../../../environment.h"
#include "Command.h"

using namespace nsBattle;

void CommandPanel::Awake() {

	//�e�̈ʒu�ݒ�
	transform->SetPosition(SCREEN_WIDTH - 100.0f, 50.0f);

	//�w�i�摜
	noDel_ptr<Sprite> pBackGround_sp = CreateSprite(new Sprite(L"Data/Image/Common/menu_button.spr"));

	//�Z���N�g�J�[�\��
	pSelecter = gameObject->CreateImageObject(0, 0, 150.0f, 80.0f,
		CreateSprite(new Sprite(L"Data/Image/Common/square.spr")), transform);
	pSelecter->GetComponent<ImageRenderer>()->SetRenderPriority(5);
	pSelecter->GetComponent<ImageRenderer>()->SetColor(1, 1, 1, 0.6f);

	//�ړ�
	CreateCommand(L"�ړ�", pBackGround_sp, eCommand::Move);

	//�U��
	CreateCommand(L"�X�L��", pBackGround_sp, eCommand::Skill);

	//�ҋ@
	CreateCommand(L"�ҋ@", pBackGround_sp, eCommand::Wait);

	//�߂�
	CreateCommand(L"�߂�", pBackGround_sp, eCommand::Back);

	//�����͔�\��
	gameObject->SetObjEnable(false);
}

void CommandPanel::Update() {
	//�I������
	if (commandState == eCommand::Select) {
		if (Input::Trg(InputConfig::input["cancel"])) {
			BackFunc();
		}

		//�R�}���h�J�[�\���̈ړ�
		MoveSelecter();

		//�R�}���h�̑I��
		if (Input::Trg(InputConfig::input["decide"])) SelectCommand();
	}

	//�ړ�����
	if (commandState == eCommand::Move) {
		gameObject->FindGameObject("fieldManager")->
			GetComponent<FieldManager>()->SetTurnState(eTurnState::Move);
		Close();
	}

	//�X�L������
	if (commandState == eCommand::Skill) {
		gameObject->FindGameObject("fieldManager")->
			GetComponent<FieldManager>()->SetTurnState(eTurnState::SelectSkill);
		Close();
	}

	//�ҋ@����
	if (commandState == eCommand::Wait) {
		pSelectChara->SetActionEnable(false);
		gameObject->FindGameObject("fieldManager")->
			GetComponent<FieldManager>()->SetTurnState(eTurnState::Field);
		Close();
	}

	//�߂鏈��
	if (commandState == eCommand::Back) {
		BackFunc();
	}
}

void CommandPanel::Open(noDel_ptr<PlayerChara> chara, eCommandPanelType type) {
	Panel::Open();

	//������
	pSelectChara = chara;
	commandState = eCommand::Select;
	selectNum = 0;
	for (auto& com : umCommand) com.second->SetComEnable(false);
	vCommand.clear();

	//�R�}���h�쐬
	panelType = type;

	//�p�l���̎�ނɉ����ăR�}���h���Z�b�g����
	if (type == eCommandPanelType::MoveBefore) {
		vCommand.emplace_back(umCommand[eCommand::Move]);
		vCommand.emplace_back(umCommand[eCommand::Skill]);
		vCommand.emplace_back(umCommand[eCommand::Back]);
	}
	else if (type == eCommandPanelType::MoveAfter) {
		vCommand.emplace_back(umCommand[eCommand::Skill]);
		vCommand.emplace_back(umCommand[eCommand::Wait]);
	}

	//�R�}���h�\��
	for (auto& com : vCommand) com->SetComEnable(true);

	//�R�}���h�ʒu�ݒ�
	SetCommandPos();

	//�J�[�\���ړ����~�߂�
	gameObject->FindGameObject("cursor")->GetComponent<Cursor>()->SetEnable(false);
}

void CommandPanel::MoveSelecter() {
	if (Input::Trg(InputConfig::input["up"])) {
		selectNum = CountDownInRange(selectNum, (int)vCommand.size() - 1);
		//�I���ʒu�ݒ�
		pSelecter->transform->position = vCommand[selectNum]->pBackGorund->transform->position;
	}

	if (Input::Trg(InputConfig::input["down"])) {
		selectNum = CountUpInRange(selectNum, (int)vCommand.size() - 1);
		//�I���ʒu�ݒ�
		pSelecter->transform->position = vCommand[selectNum]->pBackGorund->transform->position;
	}
}

void CommandPanel::SelectCommand() {
	if(vCommand[selectNum]->type == eCommand::Move) commandState = eCommand::Move;
	if(vCommand[selectNum]->type == eCommand::Skill) commandState = eCommand::Skill;
	if(vCommand[selectNum]->type == eCommand::Wait) commandState = eCommand::Wait;
	if(vCommand[selectNum]->type == eCommand::Back) commandState = eCommand::Back;

	//�J�[�\���ړ��ĊJ
	if(commandState != eCommand::Select) gameObject->FindGameObject("cursor")->
		GetComponent<Cursor>()->SetEnable(true);
}

void CommandPanel::CreateCommand(const WCHAR* text,noDel_ptr<Sprite> bgImage, eCommand type) {
	float sizeX = 150.0f;
	float sizeY = 80.0f;

	float _localY = sizeY * (int)vCommand.size();

	//�R�}���h�쐬
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

	//�J�[�\���ʒu�ݒ�
	pSelecter->transform->SetLocalPosition(0, 0);
}

void CommandPanel::BackFunc() {
	//�ړ���Ȃ猳�̈ʒu�ɖ߂�
	if (panelType == eCommandPanelType::MoveAfter) pSelectChara->BackBeforePos();
	//�t�B�[���h�}�l�[�W���[�ɏ�ԕύX�𑗂�
	noDel_ptr<FieldManager> _fm = gameObject->FindGameObject("fieldManager")->
		GetComponent<FieldManager>();
	if (panelType == eCommandPanelType::MoveBefore) _fm->SetTurnState(eTurnState::Back);
	else _fm->SetTurnState(eTurnState::Back, 2);
	//����
	Close();
}