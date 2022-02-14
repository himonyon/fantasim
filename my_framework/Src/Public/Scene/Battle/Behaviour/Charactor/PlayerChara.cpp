#include "../../../../../../framework.h"
#include "../../../../../../environment.h"
#include "PlayerChara.h"

using namespace nsBattle;

void PlayerChara::Start() {
	fMoveFunc = &PlayerChara::ShowMoveArea;
	fSkillFunc = &PlayerChara::ShowSkillRange;
}

//�ړ�����
bool PlayerChara::Move() {
	if(fMoveFunc == NULL) fMoveFunc = &PlayerChara::ShowMoveArea;
	if (fMoveFunc(*this) == false) {
		pSearcher.Reset();
		return false;
	}
	return true;
}
//�ړ��͈͂̕\��
bool PlayerChara::ShowMoveArea() {
	//���݂̈ʒu���i�[
	SetBeforeSquare();
	//�I���G���A�\��
	pSearcher.ShowRange(pCurSquare, pCharaInfo->move, eAstarFunc::MoveRange);
	fMoveFunc = &PlayerChara::SelectMoveSquare;

	return true;
}
//�ړ��}�X�̑I��
bool PlayerChara::SelectMoveSquare() {
	//�I��
	//�ړ���Ԃ�����(�Z���N�g�I�u�W�F�N�g������)
	if (Input::Trg(InputConfig::input["cancel"])) {
		pSearcher.Reset();
		gameObject->FindGameObject("fieldManager")->
			GetComponent<FieldManager>()->SetTurnState(eTurnState::Back);
		//�|�C���^������
		fMoveFunc = &PlayerChara::ShowMoveArea;
		return false;
	}

	//�ړ��I����ԂɈڍs
	if (Input::Trg(InputConfig::input["decide"])) {
		for (auto& selectSq : pSearcher.vCloseList) {
			//�I�����ꂽ�}�X���ړ��\�}�X�Ȃ�ŒZ�o�H���v�Z����
			if (selectSq->GetSelectSquareHover() && selectSq->GetSelectEnable()) {
				pSearcher.GetCulcedStartSquare(selectSq, GetCurrentSquare());
				fMoveFunc = &PlayerChara::MoveToTarget;
				break;
			}
		}
	}

	return true;
}
//�ړI�n�ֈړ�
bool PlayerChara::MoveToTarget() {
	//�ړ��ł��Ȃ��ꍇ�͏������Ȃ�
	if (pSearcher.vCloseList.size() == 0) fMoveFunc = &PlayerChara::SelectMoveSquare;
	else {
		if (!BattleChara::Move(pSearcher.vCloseList.back())) {
			//�^�[���̏�ԑJ��
			gameObject->FindGameObject("fieldManager")->
				GetComponent<FieldManager>()->SetTurnState(eTurnState::AfterCommand);
			//�ړ������̊֐��|�C���^��������Ԃɖ߂�
			fMoveFunc = &PlayerChara::ShowMoveArea;
			return false;
		}
	}
	
	return true;
}


//�X�L������
bool PlayerChara::SkillFunc() {
	if (fSkillFunc == NULL) fSkillFunc = &PlayerChara::ShowSkillRange;
	if (fSkillFunc(*this) == false) {
		pSearcher.Reset();
		return false;
	}
	return true;
}
//�X�L���͈͂̕\��
bool PlayerChara::ShowSkillRange() {
	//�I���G���A�\��
	if (pSelectSkill->GetSkillType() == eSkillType::Attack) {
		pSearcher.ShowRange(pCurSquare, pSelectSkill->GetMaxDis(), eAstarFunc::FindEnemyRange);
		fSkillFunc = &PlayerChara::SelectEnemy;
	}
	else{
		pSearcher.ShowRange(pCurSquare, pSelectSkill->GetMaxDis(), eAstarFunc::FindFriendRange);
		fSkillFunc = &PlayerChara::SelectFriend;
	}

	//�L�����̓����蔻����J�n
	BattleChara::SetOnHitFunc(true);

	return true;
}
//�G�̑I��
bool PlayerChara::SelectEnemy() {
	//�߂鏈��
	if (Input::Trg(InputConfig::input["cancel"])) {
		pSearcher.Reset();
		gameObject->FindGameObject("fieldManager")->
			GetComponent<FieldManager>()->SetTurnState(eTurnState::Back);
		//�|�C���^������
		fSkillFunc = &PlayerChara::ShowSkillRange;
		return false;
	}

	//�U����ԂɈڍs
	if (Input::Trg(InputConfig::input["decide"])) {
		for (auto& selectSq : pSearcher.vCloseList) {
			if (selectSq->GetSelectSquareHover() && selectSq->GetSelectEnable()) {
				if (BattleChara::pCollisionChara == NULL) break;
				//�G�L�����ł͂Ȃ��ꍇ�������Ȃ�
				noDel_ptr<EnemyChara> _enemy = dynamic_noDel_cast<EnemyChara>(BattleChara::pCollisionChara);
				if (_enemy == NULL) break;
				
				//�G�̃X�L���I��
				_enemy->pTargetChara = noDel_ptr<BattleChara>(this);
				_enemy->ChooseSkill(eSkillType::Attack);

				//�^�[�Q�b�g�L�����̐ݒ�
				pTargetChara = static_noDel_cast<BattleChara>(_enemy);

				//�L�����̍s���I���ݒ�
				SetActionEnable(false);

				//�^�[���̏�Ԃ��o�g����Ԃ֑J��
				gameObject->FindGameObject("fieldManager")->
					GetComponent<FieldManager>()->SetTurnState(eTurnState::Battle);

				//�����蔻����~�߂�
				BattleChara::SetOnHitFunc(false);

				//�|�C���^������
				fSkillFunc = &PlayerChara::ShowSkillRange;
				return false;
			}
		}
	}
	return true;
}
//�����̑I��
bool PlayerChara::SelectFriend() {
	bool _endFlag = false;

	//�߂鏈��
	if (Input::Trg(InputConfig::input["cancel"])) {
		pSearcher.Reset();
		gameObject->FindGameObject("fieldManager")->
			GetComponent<FieldManager>()->SetTurnState(eTurnState::Back);
		//�|�C���^������
		fSkillFunc = &PlayerChara::ShowSkillRange;
		return false;
	}

	//�ړ��I����ԂɈڍs
	if (Input::Trg(InputConfig::input["decide"])) {
		for (auto& selectSq : pSearcher.vCloseList) {
			if (selectSq->GetSelectSquareHover() && selectSq->GetSelectEnable()) {
				if (BattleChara::pCollisionChara == NULL) break;
				//�G�L�����ł͂Ȃ��ꍇ�������Ȃ�
				noDel_ptr<PlayerChara> _friend = dynamic_noDel_cast<PlayerChara>(BattleChara::pCollisionChara);
				if (_friend == NULL) break;

				//�^�[�Q�b�g�L�����̐ݒ�
				pTargetChara = static_noDel_cast<BattleChara>(_friend);

				//����
				//�S�̏���
				if (pSelectSkill->IsAllRange()) {
					for (auto& selectSq : pSearcher.vCloseList) {
						//�v���C���[�L�����̂���}�X�Ȃ炻�̃L����������
						noDel_ptr<PlayerChara> _charaP = dynamic_noDel_cast<PlayerChara>(selectSq->GetOnChara());
						if (_charaP == NULL) continue;
						pTargetChara = static_noDel_cast<BattleChara>(_charaP);;
						if (pSelectSkill->GetSkillType() == eSkillType::Heal) Heal();
						else Buff();
					}
				}
				//�P�̏���
				else {
					if (pSelectSkill->GetSkillType() == eSkillType::Heal) Heal();
					else Buff();
				}

				//�����蔻����~�߂�
				BattleChara::SetOnHitFunc(false);

				//�L�����̍s���I���ݒ�
				SetActionEnable(false);

				//�^�[���̏�Ԃ��t�B�[���h��Ԃ֑J��
				gameObject->FindGameObject("fieldManager")->
					GetComponent<FieldManager>()->SetTurnState(eTurnState::Field);

				//�|�C���^������
				fSkillFunc = &PlayerChara::ShowSkillRange;

				_endFlag = true;

				return false;
			}
		}
	}
	return true;
}


void PlayerChara::BackBeforePos() {
	SetCurrentSquare(pBeforeSquare);
	transform->position = pBeforeSquare->transform->position;
}

