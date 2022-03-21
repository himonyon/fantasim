#include "../../../../framework.h"
#include "../../../../environment.h"

void Operation::Awake() {
	//�{�^���̍쐬
	CreateKeyMouButton();
	CreateJoyButton();
	
	//��������I�u�W�F�N�g�쐬
	CreateOperation();
}


void Operation::Open() {
	gameObject->SetObjEnable(true);
}
void Operation::Hide() {
	gameObject->SetObjEnable(false);
}

void Operation::CreateOperation() {
	//�����I�u�W�F�N�g�����锠
	for (int i = 0; i < OprNum; i++) {
		noDel_ptr<GameObject> _pTextObj = gameObject->CreateObject(0, 0, 0, transform);
		_pTextObj->AddComponent<Font>();
		oprText[i].trigger = false;
		oprText[i].pText = _pTextObj->GetComponent<Font>();
		oprText[i].pText->gameObject->SetRenderOrder((int)eRenderOrder::FrontUI + 1);
		oprText[i].pText->SetFontSize(24.0f);
		oprText[i].pText->SetTextAlignment(eTextAlignment::Right);
	}
}

void Operation::AddOperation(std::string type, std::wstring text) {
	//y��
	float _pivotY = transform->position.y - 12.0f;
	//x��
	//�e�L�X�g���Ƃ�X�]��
	float _padding = 30;
	float _pivotX = SCREEN_WIDTH - 100;

	std::wstring _button;

	if (UserSetting::device == (int)eDeviceType::Keyboard)_button = umKeyMouButton[type];
	else _button = umJoyButton[type];
	

	//�󂢂Ă���ꏊ�Ƀe�L�X�g�ݒ�
	for (int i = 0; i < OprNum; i++) {
		//�g�p�ς݂Ȃ�X�L�b�v
		if (oprText[i].trigger) continue;
		//�󂢂Ă���΃e�L�X�g�ݒ�
		oprText[i].trigger = true;
		oprText[i].pText->Print(L"(%s) : %s", _button.c_str(), text.c_str());
		oprText[i].pText->transform->position.y = _pivotY;
		//��O�̃e�L�X�g������΂��̒������{�]�������炷
		oprText[i].pText->transform->position.x = _pivotX; //0�ڂ͂����ŏI��
		if (i == 0) return;
		float _befPosX = oprText[i - 1].pText->transform->position.x;
		float _befTextLen = oprText[i - 1].pText->GetTextLength();
		oprText[i].pText->transform->position.x = _befPosX - (_befTextLen + _padding);
		return;
	}
}

void Operation::ResetOperation() {
	for (auto& opr : oprText) {
		opr.trigger = false;
		opr.pText->Print(L"");
	}
	
}

//����{�^���ꗗ�쐬�i�R���g���[���[�j
void Operation::CreateJoyButton() {
	umJoyButton["decide"] = L"�Z";
	umJoyButton["cancel"] = L"�~";
	umJoyButton["option"] = L"�I�v�V����";
	umJoyButton["changeTab"] = L"��";
}
//����{�^���ꗗ�쐬�i�L�[�}�E�X)
void Operation::CreateKeyMouButton() {
	umKeyMouButton["decide"] = L"SPACE";
	umKeyMouButton["cancel"] = L"Q";
	umKeyMouButton["option"] = L"P";
	umKeyMouButton["changeTab"] = L"/";
}