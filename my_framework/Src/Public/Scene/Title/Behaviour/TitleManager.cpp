#include "../../../../../framework.h"
#include "../../../../../environment.h"

#include "TitleManager.h"

using namespace nsTitle;


void TitleManager::Awake() {
	pNewText = gameObject->CreateObject(SCREEN_WIDTH_CENTER, 300, 100);
	pNewText->AddComponent<Font>();
	pNewText_font = pNewText->GetComponent<Font>();
	pNewText_font->SetTextAlignment(eTextAlignment::Center);
	pNewText_font->Print(L"はじめから");

	pContinueText = gameObject->CreateObject(SCREEN_WIDTH_CENTER, 370, 100);
	pContinueText->AddComponent<Font>();
	pContinueText_font = pContinueText->GetComponent<Font>();
	pContinueText_font->SetTextAlignment(eTextAlignment::Center);
	pContinueText_font->Print(L"つづきから");

	pOpening_sd = std::make_unique<Sound>(L"Data/Sound/Title/title_bgm.wav");
	pOpening_sd->Play();

	gameObject->GetComponent<ImageRenderer>()->SetRenderPriority(-10);
}

void TitleManager::Update() {
	//ストラテジーシーンへ遷移
	if (Input::Trg(InputConfig::input["decide"])) {
		SceneManager::SwitchScene(eSceneTable::Strategy);
	}

	//サウンドのループ
	if (pOpening_sd->isPlaying() == false) pOpening_sd->Play();

	//はじめから・続きから選択
	if (Input::Trg(InputConfig::input["up"])) cursor = CountUpInRange(cursor, 1);
	if (Input::Trg(InputConfig::input["down"])) cursor = CountDownInRange(cursor, 1);

	pNewText_font->SetFontSize(cursor == 0 ? 22.0f : 18.0f);
	pContinueText_font->SetFontSize(cursor == 1 ? 22.0f : 18.0f);
}