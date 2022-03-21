#include "../../../../../../framework.h"
#include "../../../../../../environment.h"

using namespace nsStrategy;

void OptionPanel::Awake() {
	//サウンドマネージャー
	pSoundManager = gameObject->FindGameObject("soundManager")->GetComponent<SoundManager>();

	//情報を取得
	sound = UserSetting::sound;
	deviceType = (eDeviceType)UserSetting::device;

	//親サイズ
	noDel_ptr<ImageRenderer> _renderer = gameObject->GetComponent<ImageRenderer>();
	float _sizeX = _renderer->size.x;
	float _sizeY = _renderer->size.y;

	float _textX = -(_sizeX / 2) + 60.0f;

	//テキスト作成
	CreateText(pSoundText[0], 0, 0);
	pSoundText[0]->transform->SetLocalPosition(_textX, -40);
	pSoundText[0]->Print(L"サウンド :");
	CreateText(pSoundText[1], 0, 0);
	pSoundText[1]->transform->SetLocalPosition(_textX + 100, -40);
	CreateText(pDeviceText[0], 0, 0);
	pDeviceText[0]->transform->SetLocalPosition(_textX, 0);
	pDeviceText[0]->Print(L"操作 :");
	CreateText(pDeviceText[1], 0, 0);
	pDeviceText[1]->transform->SetLocalPosition(_textX + 100, 0);
	CreateText(pDeviceText[2], 0, 0);
	pDeviceText[2]->transform->SetLocalPosition(_textX, 20);
	pDeviceText[2]->Print(L"(閉じた後に反映)");

	Close();
}

void OptionPanel::Update() {
	//選択
	MoveColSelecter();
	MoveRowSelecter();

	//戻る
	if (Input::Trg(InputConfig::input["cancel"])) {
		pSoundManager->Play("cancel");
		Close();
		gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->SetTurnState(eTurnState::Back);
	}
}

void OptionPanel::Open() {
	rowSelecter = 0;
	colSelecter = 0;

	//操作説明テキスト変更
	noDel_ptr<Operation> _opr = gameObject->FindGameObject("operation")->GetComponent<Operation>();
	_opr->ResetOperation();
	_opr->AddOperation("cancel", L"戻る");

	sound = UserSetting::sound;
	deviceType = (eDeviceType)UserSetting::device;

	SoundSetting(sound);
	DeviceSetting((int)deviceType);

	pSoundText[0]->SetColor(0xffffff22);
	pDeviceText[0]->SetColor(0xffffffff);

	Panel::Open();
}

void OptionPanel::Close() {
	if (deviceType == eDeviceType::Both) Input::SetBothDevice();
	else if (deviceType == eDeviceType::Keyboard) Input::SetKeyboardDevice();
	else if (deviceType == eDeviceType::Joystick)Input::SetJoystickDevice();

	//ジョイスティックが無効ならキーボード限定にする
	if (Joystick::IsValid() == false) {
		Input::SetKeyboardDevice();
	}

	UserSetting::sound = sound;
	UserSetting::device = (int)deviceType;
	UserSetting::Save();

	Panel::Close();
}

//オプション項目を選択
void OptionPanel::MoveColSelecter() {
	int _bef = colSelecter;

	if (Input::Trg(InputConfig::input["up"])) colSelecter--;
	if (Input::Trg(InputConfig::input["down"])) colSelecter++;

	if (colSelecter == _bef) return;

	if (colSelecter < 0) colSelecter = (int)eOptionSeed::Max - 1;
	if (colSelecter == (int)eOptionSeed::Max) colSelecter = 0;

	pSoundText[0]->SetColor(0xffffffff);
	pDeviceText[0]->SetColor(0xffffffff);
	if (colSelecter == (int)eOptionSeed::Sound) pSoundText[0]->SetColor(0xffffff22);
	else if (colSelecter == (int)eOptionSeed::Device) pDeviceText[0]->SetColor(0xffffff22);
}
void OptionPanel::MoveRowSelecter() {
	int _bef = rowSelecter;

	if (Input::Trg(InputConfig::input["left"])) rowSelecter--;
	if (Input::Trg(InputConfig::input["right"])) rowSelecter++;

	if (rowSelecter == _bef) return;

	if (colSelecter == (int)eOptionSeed::Sound) {
		if (rowSelecter < 0) rowSelecter = 1;
		if (rowSelecter > 1) rowSelecter = 0;
		SoundSetting(!sound);

	}
	else if (colSelecter == (int)eOptionSeed::Device) {
		if (rowSelecter < 0) rowSelecter = (int)eDeviceType::Max - 1;
		if (rowSelecter == (int)eDeviceType::Max) rowSelecter = 0;
		DeviceSetting(rowSelecter);
	}
}

//サウンド項目の選択
void OptionPanel::SoundSetting(bool sound) {
	this->sound = sound;
	if (sound) {
		pSoundText[1]->Print(L"ON");
		pSoundManager->SetVolume(1);
		pSoundManager->SetVolume("bgm", 0.5f);
	}
	else {
		pSoundText[1]->Print(L"OFF");
		pSoundManager->SetVolume(0);
	}
}
//デバイス項目の選択
void OptionPanel::DeviceSetting(int type) {
	deviceType = (eDeviceType)type;
	if (deviceType == eDeviceType::Both) pDeviceText[1]->Print(L"両方");
	else if (deviceType == eDeviceType::Keyboard) pDeviceText[1]->Print(L"キーボード");
	else if (deviceType == eDeviceType::Joystick) pDeviceText[1]->Print(L"コントローラー");
}
