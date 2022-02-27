#include "../../../../../../framework.h"
#include "../../../../../../environment.h"

using namespace nsStrategy;

void InfoPanel::Awake() {
	//�T�E���h�}�l�[�W���[
	pSoundManager = gameObject->FindGameObject("soundManager")->GetComponent<SoundManager>();

	//�e�L�X�g�쐬
	CreateText(pInfoText, SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER);
	pInfoText->SetTextAlignment(eTextAlignment::Center);

	Close();
}

void InfoPanel::Update() {
	if (Input::Trg(InputConfig::input["decide"])) {
		pSoundManager->Play("decide");
		//�I�����ɂ̓^�C�g���֑J��
		if (gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->IsEndGame()) {
			SceneManager::SwitchScene(eSceneTable::Title);
		}
		Close();
	}
}

void InfoPanel::Open(std::wstring text) {

	//��������e�L�X�g�ύX
	noDel_ptr<Operation> _opr = gameObject->FindGameObject("operation")->GetComponent<Operation>();
	_opr->ResetOperation();
	_opr->AddOperation("decide", L"�߂�");

	Panel::Open();

	pInfoText->Print(L"%s", text.c_str());
}