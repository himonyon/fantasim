#include "../../../../../../framework.h"
#include "../../../../../../environment.h"

using namespace nsStrategy;

void InfoPanel::Awake() {
	//サウンドマネージャー
	pSoundManager = gameObject->FindGameObject("soundManager")->GetComponent<SoundManager>();

	//テキスト作成
	CreateText(pInfoText, SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER);
	pInfoText->SetTextAlignment(eTextAlignment::Center);

	Close();
}

void InfoPanel::Update() {
	if (Input::Trg(InputConfig::input["decide"])) {
		pSoundManager->Play("decide");
		//終了時にはタイトルへ遷移
		if (gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->IsEndGame()) {
			SceneManager::SwitchScene(eSceneTable::Title);
		}
		Close();
	}
}

void InfoPanel::Open(std::wstring text) {

	//操作説明テキスト変更
	noDel_ptr<Operation> _opr = gameObject->FindGameObject("operation")->GetComponent<Operation>();
	_opr->ResetOperation();
	_opr->AddOperation("decide", L"戻る");

	Panel::Open();

	pInfoText->Print(L"%s", text.c_str());
}