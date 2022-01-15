#include "../../../../../../framework.h"
#include "../../../../../../environment.h"
#include "Command.h"

using namespace nsBattle;

void CommandPanel::Awake() {

	//�J�[�\���擾
	pCursor = gameObject->FindGameObject("cursor")->GetComponent<Cursor>();

	//�X�L���I�����
	pSkillSelectPanel = gameObject->CreateImageObject(300, 250, 500, 350, 
		CreateSprite(new Sprite(L"Data/Image/Battle/skill_select_bg.spr")));
	pSkillSelectPanel->AddComponent<SelectSkillPanel>();

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
	CreateCommand(L"�ړ�", pBackGround_sp, eCommandState::Move);

	//�U��
	CreateCommand(L"�X�L��", pBackGround_sp, eCommandState::Skill);

	//�߂�
	CreateCommand(L"�߂�", pBackGround_sp, eCommandState::Back);

	//�����͔�\��
	gameObject->SetObjEnable(false);
}

void CommandPanel::Update() {
	//�I������
	if (commandState == eCommandState::Select) {
		if (Input::Trg(InputConfig::input["cancel"])) {
			Turn::curState = eFieldState::Field;
			Close();
		}

		//�R�}���h�J�[�\���̈ړ�
		MoveSelecter();

		//�R�}���h�̑I��
		if (Input::Trg(InputConfig::input["decide"])) SelectCommand();
	}

	//�ړ�����
	if (commandState == eCommandState::Move) {
		pSelectChara->StartMoveFunc();
		Close();
	}

	if (commandState == eCommandState::Skill) {
		pSkillSelectPanel->GetComponent<SelectSkillPanel>()->Open(pSelectChara);
		Close();
	}

	//�߂鏈��
	if (commandState == eCommandState::Back) {
		Turn::curState = eFieldState::Field;
		Close();
	}
	
}

void CommandPanel::Open(noDel_ptr<PlayerChara> chara) {
	Panel::Open();

	//������
	pSelectChara = NULL;
	commandState = eCommandState::Select;

	//�J�[�\���ʒu�ݒ�
	selectNum = 0;
	pSelecter->transform->position = vCommand[selectNum]->pBackGorund->transform->position;
	pSelectChara = chara;

	//�J�[�\���ړ����~�߂�
	pCursor->SetEnable(false);
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
	if(vCommand[selectNum]->type == eCommandState::Move) commandState = eCommandState::Move;
	if(vCommand[selectNum]->type == eCommandState::Skill) commandState = eCommandState::Skill;
	if(vCommand[selectNum]->type == eCommandState::Back) commandState = eCommandState::Back;

	//�J�[�\���ړ��ĊJ
	if(commandState != eCommandState::Select) pCursor->SetEnable(true);
}

void CommandPanel::CreateCommand(const WCHAR* text,noDel_ptr<Sprite> bgImage, eCommandState type) {
	float sizeX = 150.0f;
	float sizeY = 80.0f;

	float _localY = sizeY * (int)vCommand.size();

	//�R�}���h�쐬
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