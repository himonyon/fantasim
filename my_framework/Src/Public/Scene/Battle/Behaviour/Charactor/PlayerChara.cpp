#include "../../../../../../framework.h"
#include "../../../../../../environment.h"
#include "PlayerChara.h"

using namespace nsBattle;

void PlayerChara::Update() {
	//移動処理
	if (charaState == eCharaState::Move) {
		if (fCharaFunc(*this) == false) {
			charaState = eCharaState::None;
			Turn::curState = eFieldState::Field;
		}
	}
}

//移動常態に遷移
void PlayerChara::StartMoveFunc() {
	//選択エリア表示
	pSearcher.ShowMoveRange(pCurSquare, pCharaInfo->move);
	fCharaFunc = &PlayerChara::SelectMoveSquare;
	charaState = eCharaState::Move;
}

bool PlayerChara::SelectMoveSquare() {
	//選択
	//移動常態を解除(セレクトオブジェクトを消す)
	if (Input::Trg(InputConfig::input["cancel"])) {
		pSearcher.Reset();
		return false;
	}

	//移動選択状態に移行
	if (Input::Trg(InputConfig::input["decide"])) {
		for (auto& selectSq : pSearcher.vCloseList) {
			if (selectSq->GetSelectSquareHover() && selectSq->GetSelectEnable()) {
				pSearcher.GetCulcedStartSquare(selectSq, GetCurrentSquare());
				fCharaFunc = &PlayerChara::Move;
				break;
			}
		}
	}

	return true;
}

//移動処理
bool PlayerChara::Move() {
	//移動できない場合は処理しない
	if (pSearcher.vCloseList.size() == 0) fCharaFunc = &PlayerChara::SelectMoveSquare;
	else {
		if (!BattleChara::Move(pSearcher.vCloseList.back())) {
			return false;
		}
	}

	return true;
}


//攻撃処理遷移
void PlayerChara::StartAttackFunc(noDel_ptr<AttackSkill> skill) {
	//選択エリア表示
	pSearcher.ShowMoveRange(pCurSquare, pCharaInfo->move);
	fCharaFunc = &PlayerChara::SelectMoveSquare;
	charaState = eCharaState::Move;
}

//回復スキル処理遷移
void PlayerChara::StartHealFunc(noDel_ptr<AttackSkill> skill) {

}

//バフスキル処理遷移
void PlayerChara::StartBuffFunc(noDel_ptr<AttackSkill> skill) {

}

bool PlayerChara::SelectEnemy() {
	return true;
}

bool PlayerChara::SelectFriend() {
	return true;
}


