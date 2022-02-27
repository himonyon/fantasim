#include "../../../../../../framework.h"
#include "../../../../../../environment.h"

using namespace nsStrategy;

void Turn::SetTurnState(eTurnState state, int backNum) {
	//戻る状態が送られたら一つ前の状態に戻す
	if (state == eTurnState::Back) {
		for (int i = 0; i < backNum; i++) {
			turnState = turnState->bef.lock();
		}
		return;
	}

	//一覧にない場合、新規登録
	if (umTurnStates.find(state) == umTurnStates.end()) {
		std::shared_ptr<stTurnState> _temp = std::make_shared<stTurnState>(stTurnState(state));
		umTurnStates[state] = _temp;
	}
	//現在の状態をセット
	if (turnState != NULL) umTurnStates[state]->bef = turnState; //以前の状態をセット
	turnState = umTurnStates[state]; //現在の状態をセット
}

//ゲーム終了
void Turn::EndGame(bool isClear) {
	if (pInfoPanel == NULL) pInfoPanel = gameObject->FindGameObject("infoPanel")->GetComponent<InfoPanel>();
	if (isClear) {
		pInfoPanel->Open(L"ゲームクリア！！");
	}
	else {
		pInfoPanel->Open(L"ゲームオーバー、、、");
	}
}