#include "../../../../../../framework.h"
#include "../../../../../../environment.h"

using namespace nsStrategy;

void OptionMenu::Awake() {
	//�T�E���h�}�l�[�W���[
	pSoundManager = gameObject->FindGameObject("soundManager")->GetComponent<SoundManager>();

	//�ʒu�ݒ�
	transform->SetPosition(SCREEN_WIDTH - 100.0f, 50.0f);

	//�w�i�摜
	noDel_ptr<Sprite> pBackGround_sp = CreateSprite(new Sprite(L"Data/Image/Common/menu_button.spr"));

	//�Z���N�g�J�[�\��
	pSelecter = gameObject->CreateImageObject(0, 0, 150.0f, 80.0f,
		CreateSprite(new Sprite(L"Data/Image/Common/square.spr")), transform);
	pSelecter->GetComponent<ImageRenderer>()->SetRenderPriority((int)eRenderOrder::FrontUI+1);
	pSelecter->GetComponent<ImageRenderer>()->SetColor(1, 1, 1, 0.6f);

	//�^�[���I��
	CreateCommand(L"�^�[���I��", pBackGround_sp, eOptionMenu::TurnEnd);

	//�I�v�V����
	CreateCommand(L"�ݒ�", pBackGround_sp, eOptionMenu::Option);

	//�Z�[�u
	CreateCommand(L"�Z�[�u", pBackGround_sp, eOptionMenu::Save);

	Close();
}

void OptionMenu::Update() {
	MoveSelecter();
	SelectMenu();

	if (Input::Trg(InputConfig::input["cancel"])) {
		pSoundManager->Play("cancel");
		Close();
		gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->SetTurnState(eTurnState::Back);
	}
}

void OptionMenu::Open() {
	//������
	selectNum = 0;

	//��������e�L�X�g�ύX
	noDel_ptr<Operation> _opr = gameObject->FindGameObject("operation")->GetComponent<Operation>();
	_opr->ResetOperation();
	_opr->AddOperation("decide", L"�I��");
	_opr->AddOperation("cancel", L"�߂�");

	//�I���ʒu�ݒ�
	pSelecter->transform->position = vMenu[selectNum]->pBackGorund->transform->position;

	Panel::Open();
}

//�R�}���h�쐬
void OptionMenu::CreateCommand(const WCHAR* text, noDel_ptr<Sprite> bgImage, eOptionMenu type) {
	float sizeX = 150.0f;
	float sizeY = 80.0f;

	float _localY = sizeY * (int)vMenu.size();

	//�R�}���h�쐬
	std::shared_ptr<stOptionMenu> _command = std::make_shared<stOptionMenu>();

	_command->pBackGorund = gameObject->CreateImageObject(0, 0, sizeX, sizeY, bgImage, transform);
	_command->pBackGorund->GetComponent<ImageRenderer>()->SetRenderPriority((int)eRenderOrder::FrontUI);
	_command->pText = gameObject->CreateObject(0, 0, 0, _command->pBackGorund->transform);
	_command->pBackGorund->transform->SetLocalPosition(0, _localY);
	_command->pText->transform->SetLocalPosition(0, -10);
	_command->pText->AddComponent<Font>();
	noDel_ptr<Font> _font = _command->pText->GetComponent<Font>();
	_font->SetTextAlignment(eTextAlignment::Center);
	_font->SetFontSize(22.0f);
	_font->Print(text);
	_font->SetRenderPriority((int)eRenderOrder::FrontUI + 2);

	_command->type = type;

	vMenu.emplace_back(_command);
}

//�R�}���h�I��
void OptionMenu::MoveSelecter() {
	if (Input::Trg(InputConfig::input["up"])) {
		selectNum = CountDownInRange(selectNum, (int)vMenu.size() - 1);
		//�I���ʒu�ݒ�
		pSelecter->transform->position = vMenu[selectNum]->pBackGorund->transform->position;
	}

	if (Input::Trg(InputConfig::input["down"])) {
		selectNum = CountUpInRange(selectNum, (int)vMenu.size() - 1);
		//�I���ʒu�ݒ�
		pSelecter->transform->position = vMenu[selectNum]->pBackGorund->transform->position;
	}
}
void OptionMenu::SelectMenu() {
	if (Input::Trg(InputConfig::input["decide"]) == false) return;
	pSoundManager->Play("decide");
	if (vMenu[selectNum]->type == eOptionMenu::TurnEnd) {
		gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->SetTurnState(eTurnState::TurnEnd);
	}
	if (vMenu[selectNum]->type == eOptionMenu::Option) {
		gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->SetTurnState(eTurnState::Option);
	}
	if (vMenu[selectNum]->type == eOptionMenu::Save) {
		gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->SetTurnState(eTurnState::Save);
	}

	Close();
}