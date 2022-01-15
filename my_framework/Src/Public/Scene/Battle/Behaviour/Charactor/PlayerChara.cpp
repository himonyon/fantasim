#include "../../../../../../framework.h"
#include "../../../../../../environment.h"
#include "PlayerChara.h"

using namespace nsBattle;

void PlayerChara::Update() {
	//�ړ�����
	if (charaState == eCharaState::Move) {
		if (fCharaFunc(*this) == false) {
			charaState = eCharaState::None;
			Turn::curState = eFieldState::Field;
		}
	}
}

//�ړ���ԂɑJ��
void PlayerChara::StartMoveFunc() {
	//�I���G���A�\��
	pSearcher.ShowMoveRange(pCurSquare, pCharaInfo->move);
	fCharaFunc = &PlayerChara::SelectMoveSquare;
	charaState = eCharaState::Move;
}

bool PlayerChara::SelectMoveSquare() {
	//�I��
	//�ړ���Ԃ�����(�Z���N�g�I�u�W�F�N�g������)
	if (Input::Trg(InputConfig::input["cancel"])) {
		pSearcher.Reset();
		return false;
	}

	//�ړ��I����ԂɈڍs
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

//�ړ�����
bool PlayerChara::Move() {
	//�ړ��ł��Ȃ��ꍇ�͏������Ȃ�
	if (pSearcher.vCloseList.size() == 0) fCharaFunc = &PlayerChara::SelectMoveSquare;
	else {
		if (!BattleChara::Move(pSearcher.vCloseList.back())) {
			return false;
		}
	}

	return true;
}


//�U�������J��
void PlayerChara::StartAttackFunc(noDel_ptr<AttackSkill> skill) {
	//�I���G���A�\��
	pSearcher.ShowMoveRange(pCurSquare, pCharaInfo->move);
	fCharaFunc = &PlayerChara::SelectMoveSquare;
	charaState = eCharaState::Move;
}

//�񕜃X�L�������J��
void PlayerChara::StartHealFunc(noDel_ptr<AttackSkill> skill) {

}

//�o�t�X�L�������J��
void PlayerChara::StartBuffFunc(noDel_ptr<AttackSkill> skill) {

}

bool PlayerChara::SelectEnemy() {
	return true;
}

bool PlayerChara::SelectFriend() {
	return true;
}


