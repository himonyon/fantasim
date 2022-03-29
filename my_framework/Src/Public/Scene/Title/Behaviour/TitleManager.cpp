#include "../../../../../framework.h"
#include "../../../../../environment.h"

#include "TitleManager.h"

using namespace nsTitle;


void TitleManager::Awake() {
	//���[�U�[���ǂݍ���
	UserSetting::SetUpUserSetting();

	pOpening_sd = std::make_unique<Sound>(L"Data/Sound/Title/title_bgm.wav");
	pOpening_sd->Play();

	gameObject->SetRenderOrder(-10);

	//���[�U�[��񔽉f
	LoadSetting();

	//���߂���e�L�X�g
	pNewText = gameObject->CreateObject(SCREEN_WIDTH_CENTER, 300, 100);
	pNewText->AddComponent<Font>();
	pNewText_font = pNewText->GetComponent<Font>();
	pNewText_font->SetTextAlignment(eTextAlignment::Center);
	pNewText_font->Print(L"�͂��߂���");

	//�Z�[�u�f�[�^�����邩�m�F--------------------
	bool _saveData = false;
	char _key[256] = { 0 };
	FILE* fp = NULL;
	fopen_s(&fp, "Data/SaveData/base.txt", "r");
	if (fp != NULL) {
		fscanf_s(fp, "%s", _key, (int)sizeof(_key));
		if (strcmp(_key, "save") == 0) _saveData = true;
	}
	if (fp != NULL) fclose(fp);
	//-------------------------------------------

	cursor = 0;
	pNewText_font->SetFontSize(22.0f);
	pNewText_font->SetColor(0xffffff22);

	if (_saveData == false) return;

	//��������e�L�X�g
	pContinueText = gameObject->CreateObject(SCREEN_WIDTH_CENTER, 370, 100);
	pContinueText->AddComponent<Font>();
	pContinueText_font = pContinueText->GetComponent<Font>();
	pContinueText_font->SetTextAlignment(eTextAlignment::Center);
	pContinueText_font->Print(L"�Â�����");


	cursor = 1;
	pContinueText_font->SetFontSize(22.0f);
	pContinueText_font->SetColor(0xffffff22);
	pNewText_font->SetFontSize(18.0f);
	pNewText_font->SetColor(0xffffffff);
}

void TitleManager::Update() {
	//�X�g���e�W�[�V�[���֑J��
	if (Keyboard::Trg(DIK_SPACE) || Joystick::Trg(JOY_CIRCLE)) {
		TransScene();
	}

	//�T�E���h�̃��[�v
	if (pOpening_sd->isPlaying() == false) pOpening_sd->Play();

	if (stopFunc) return;

	MoveCursor();
}

//�J�[�\���ړ�
void TitleManager::MoveCursor() {
	if (pContinueText == NULL) return;

	int _bef = cursor;
	
	//�͂��߂���E��������I��
	if (Input::Trg(InputConfig::input["up"])) cursor = CountUpInRange(cursor, 1);
	if (Input::Trg(InputConfig::input["down"])) cursor = CountDownInRange(cursor, 1);

	if (_bef == cursor) return;

	pNewText_font->SetFontSize(cursor == 0 ? 22.0f : 18.0f);
	pNewText_font->SetColor(cursor == 0 ? 0xffffff22 : 0xffffffff);

	pContinueText_font->SetFontSize(cursor == 1 ? 22.0f : 18.0f);
	pContinueText_font->SetColor(cursor == 1 ? 0xffffff22 : 0xffffffff);
}

//�J��
void TitleManager::TransScene() {
	//���߂���̏ꍇ�f�[�^������
	if (cursor == 0) {
		remove("Data/SaveData/base.txt");
	}

	SceneManager::SwitchScene(eSceneTable::Strategy);

	stopFunc = true;
}

void TitleManager::LoadSetting() {
	eDeviceType _device = (eDeviceType)UserSetting::device;
	if (_device == eDeviceType::Both) Input::SetBothDevice();
	else if (_device == eDeviceType::Keyboard) Input::SetKeyboardDevice();
	else if (_device == eDeviceType::Joystick) Input::SetJoystickDevice();

	//�W���C�X�e�B�b�N�������Ȃ�L�[�{�[�h����ɂ���
	if (Joystick::IsValid() == false) {
		Input::SetKeyboardDevice();
	}

	if (UserSetting::sound) pOpening_sd->SetVolume(0.1f);
	else pOpening_sd->SetVolume(0);
}