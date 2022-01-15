#include "../../../../../../framework.h"
#include "../../../../../../environment.h"
#include "PlayerTurn.h"

using namespace nsBattle;

void PlayerTurn::Start() {
	//�X�e�[�^�X�p�l���̌���
	pStatusPanel = gameObject->FindGameObject("statusPanel");
	//�J�[�\���̌���
	pCursor = gameObject->FindGameObject("cursor")->GetComponent<Cursor>();

	//�R�}���h�p�l��
	noDel_ptr<GameObject> obj = gameObject->CreateObject(0, 0, 0);
	obj->AddComponent<CommandPanel>();
	pCommandPanel = obj->GetComponent<CommandPanel>();
}

void PlayerTurn::Update() {
	if (curState == eFieldState::Field) {
		NormalFunc();
	}
}

void PlayerTurn::SelectChara() {
	for (auto& chara : vBattleChara) {
		if (chara->isCursorHovered) {
			pSelectChara = chara;
			break;
		}
	}
}

void PlayerTurn::NormalFunc() {
	//�J�[�\��������
	if (pCursor->IsEnable() == false) pCursor->SetEnable(true);

	//�L�����̏����o��
	ShowCharaInfo();

	if (Input::Trg(InputConfig::input["decide"])) {
		SelectChara();
	}

	MoveCamera();

	//�L�����I�����ꂽ��R�}���h���J��
	if (pSelectChara != NULL) {
		noDel_ptr<PlayerChara> _playerC = dynamic_noDel_cast<PlayerChara>(pSelectChara);
		if (_playerC != NULL) {
			//�X�e�[�^�X�p�l���͕��ăR�}���h�p�l����\��
			pSelectChara = NULL;
			pStatusPanel->GetComponent<StatusPanel>()->Close();
			pCommandPanel->Open(_playerC);
			curState = eFieldState::Command;
		}
		else {
			pSelectChara = NULL;
		}
	}
}

void PlayerTurn::ShowCharaInfo() {
	for (auto& chara : vBattleChara) {
		if (chara->isCursorHovered) {
			if (pStatusPanel->IsObjEnable() == false) {
				pStatusPanel->GetComponent<StatusPanel>()->Open(chara);
			}
			return;
		}
	}

	if (pStatusPanel->IsObjEnable()) {
		pStatusPanel->GetComponent<StatusPanel>()->Close();
	}
}