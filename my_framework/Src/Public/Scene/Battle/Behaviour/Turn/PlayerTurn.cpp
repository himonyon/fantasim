#include "../../../../../../framework.h"
#include "../../../../../../environment.h"
#include "PlayerTurn.h"

using namespace nsBattle;

void PlayerTurn::Start() {
	//ステータスパネルの検索
	pStatusPanel = gameObject->FindGameObject("statusPanel");
	//カーソルの検索
	pCursor = gameObject->FindGameObject("cursor")->GetComponent<Cursor>();

	//コマンドパネル
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
	//カーソル動かす
	if (pCursor->IsEnable() == false) pCursor->SetEnable(true);

	//キャラの情報を出す
	ShowCharaInfo();

	if (Input::Trg(InputConfig::input["decide"])) {
		SelectChara();
	}

	MoveCamera();

	//キャラ選択されたらコマンドを開く
	if (pSelectChara != NULL) {
		noDel_ptr<PlayerChara> _playerC = dynamic_noDel_cast<PlayerChara>(pSelectChara);
		if (_playerC != NULL) {
			//ステータスパネルは閉じてコマンドパネルを表示
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